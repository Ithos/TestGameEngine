#include "../Materials/Material.h"
#include "CustomShadingStep.h"

GeometryEngine::CustomShading::CustomShadingStep::CustomShadingStep(CustomShadingInterface * parent, CustomShadingSteps step) : mpProgram(nullptr), mpShaderManager(nullptr), 
mpTargetMaterial(nullptr), mpParentInterface(parent)
{
	mpProgram = new QOpenGLShaderProgram();
	mpParentInterface->AddShadingStep(this, step);
}

GeometryEngine::CustomShading::CustomShadingStep::~CustomShadingStep()
{
	if (mpProgram != nullptr)
	{
		delete mpProgram;
		mpProgram = nullptr;
	}
}

void GeometryEngine::CustomShading::CustomShadingStep::initCustomShading(ShaderFiles::ShaderManager * shaderManager)
{
	mpShaderManager = shaderManager;
	this->mpProgram = new QOpenGLShaderProgram();
	this->initShaders();
	this->initProgram();
}

void GeometryEngine::CustomShading::CustomShadingStep::RenderShaders(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& modelViewProjection, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	assert(mpProgram != nullptr && "No shader program found");
	{
		// Bind shader pipeline for use
		if (!mpProgram->bind())
		{
			assert(false && "Shader failed to bind");
		}

		setProgramParameters(modelViewProjection);
		modifyRenderShader(vertexBuf, indexBuf, totalVertexNum, totalIndexNum);
	}
}

void GeometryEngine::CustomShading::CustomShadingStep::initProgram()
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

void GeometryEngine::CustomShading::CustomShadingStep::AddToInterface(CustomShadingInterface * parent, CustomShadingSteps step)
{
	mpParentInterface = parent;
	mpParentInterface->AddShadingStep(this, step);
}

void GeometryEngine::CustomShading::CustomShadingStep::modifyRenderShader(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	if ( mpTargetMaterial->GetDrawBacksideFaces() ) glDisable(GL_CULL_FACE);
	{
		drawShader(vertexBuf, indexBuf, totalVertexNum, totalIndexNum);
	}
	if (mpTargetMaterial->GetDrawBacksideFaces()) glEnable(GL_CULL_FACE);
}

void GeometryEngine::CustomShading::CustomShadingStep::copy(const CustomShadingStep & ref)
{
	this->mpParentInterface = nullptr;
	this->mpTargetMaterial = nullptr;
	this->mpProgram = nullptr;
}
