#include "../Materials/Material.h"
#include "MaterialPostProcessStep.h"

GeometryEngine::CustomShading::MaterialPostProcessStep::MaterialPostProcessStep(MaterialPostProcessInterface * parent, MaterialPostProcessSteps step, unsigned int iterations) :
	mpProgram(nullptr), mpShaderManager(nullptr), mpTargetMaterial(nullptr), mpParentInterface(parent), mIterations(iterations)
{
	mpProgram = new QOpenGLShaderProgram();
	mpParentInterface->AddPostProcessStep(this, step);
}

GeometryEngine::CustomShading::MaterialPostProcessStep::~MaterialPostProcessStep()
{
	if (mpProgram != nullptr)
	{
		delete mpProgram;
		mpProgram = nullptr;
	}
}

void GeometryEngine::CustomShading::MaterialPostProcessStep::initCustomShading(ShaderFiles::ShaderManager * shaderManager)
{
	mpShaderManager = shaderManager;
	this->mpProgram = new QOpenGLShaderProgram();
	this->initShaders();
	this->initProgram();
}

void GeometryEngine::CustomShading::MaterialPostProcessStep::ApplyPostProcess(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, const QMatrix4x4 & modelViewProjection, 
	unsigned int totalVertexNum, unsigned int totalIndexNum, const GBufferTextureInfo & gBuffTexInfo)
{
	assert(mpProgram != nullptr && "No shader program found");
	{
		// Bind shader pipeline for use
		if (!mpProgram->bind())
		{
			assert(false && "Shader failed to bind");
		}

		setProgramParameters(modelViewProjection, gBuffTexInfo);
		modifyRenderShader(vertexBuf, indexBuf, totalVertexNum, totalIndexNum, gBuffTexInfo);
	}
}

void GeometryEngine::CustomShading::MaterialPostProcessStep::initProgram()
{
	if (mVertexShaderKey != "")
	{
		if (!mpShaderManager->IsLoaded(mVertexShaderKey))
		{
			mpShaderManager->LoadShader(mVertexShaderKey);
		}

		// Compile vertex shader
		if (!mpProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, mpShaderManager->GetLoadedShaderContent(mVertexShaderKey).c_str()))
		{
			assert(false && "Material --> VertexShader failed to compile");
		}
	}

	if (mFragmentShaderKey != "")
	{
		if (!mpShaderManager->IsLoaded(mFragmentShaderKey))
		{
			mpShaderManager->LoadShader(mFragmentShaderKey);
		}

		// Compile fragment shader
		if (!mpProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, mpShaderManager->GetLoadedShaderContent(mFragmentShaderKey).c_str()))
		{
			assert(false && "Material --> FragmentShader failed to compile");
		}
	}

	assert(mpProgram != nullptr && "No material shader program found");
	{
		// Link shader pipeline
		if (!mpProgram->link())
		{
			assert(false && "Material --> Program failed to link");
		}
	}
}

void GeometryEngine::CustomShading::MaterialPostProcessStep::AddToInterface(MaterialPostProcessInterface * parent, MaterialPostProcessSteps step)
{
	mpParentInterface = parent;
	mpParentInterface->AddPostProcessStep(this, step);
}

void GeometryEngine::CustomShading::MaterialPostProcessStep::modifyRenderShader(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum, const GBufferTextureInfo & gBuffTexInfo)
{
	if (mpTargetMaterial->GetDrawBacksideFaces()) glDisable(GL_CULL_FACE);
	{
		applyPostProcess(vertexBuf, indexBuf, totalVertexNum, totalIndexNum, gBuffTexInfo);
	}
	if (mpTargetMaterial->GetDrawBacksideFaces()) glEnable(GL_CULL_FACE);
}

void GeometryEngine::CustomShading::MaterialPostProcessStep::copy(const MaterialPostProcessStep & ref)
{
	this->mpParentInterface = nullptr;
	this->mpTargetMaterial = nullptr;
	this->mpProgram = nullptr;
	this->mIterations = ref.mIterations;
}
