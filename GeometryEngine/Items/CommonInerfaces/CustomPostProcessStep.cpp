#include "CustomPostProcessStep.h"

GeometryEngine::CustomShading::CustomPostProcessStep::CustomPostProcessStep(CustomPostProcessStepInterface * parent, CustomPostProcessSteps step) : mpProgram(nullptr), mpShaderManager(nullptr),
mpTargetProcess(nullptr), mpParentInterface(parent)
{
	mpProgram = new QOpenGLShaderProgram();
	mpParentInterface->AddPostProcessStep(this, step);
}

GeometryEngine::CustomShading::CustomPostProcessStep::~CustomPostProcessStep()
{
	if (mpProgram != nullptr)
	{
		delete mpProgram;
		mpProgram = nullptr;
	}
}

void GeometryEngine::CustomShading::CustomPostProcessStep::initCustomShading(ShaderFiles::ShaderManager * shaderManager)
{
	mpShaderManager = shaderManager;
	this->mpProgram = new QOpenGLShaderProgram();
	this->initShaders();
	this->initProgram();
}

void GeometryEngine::CustomShading::CustomPostProcessStep::ApplyPostProcessStep(const GBufferTextureInfo & gBuffTexInfo)
{
	assert(mpProgram != nullptr && "No shader program found");
	{
		// Bind shader pipeline for use
		if (!mpProgram->bind())
		{
			assert(false && "Shader failed to bind");
		}

		setProgramParameters(gBuffTexInfo);
		applyFilter();
	}
}

void GeometryEngine::CustomShading::CustomPostProcessStep::initProgram()
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

void GeometryEngine::CustomShading::CustomPostProcessStep::AddToInterface(CustomPostProcessStepInterface * parent, CustomPostProcessSteps step)
{
	mpParentInterface = parent;
	mpParentInterface->AddPostProcessStep(this, step);
}

void GeometryEngine::CustomShading::CustomPostProcessStep::copy(const CustomPostProcessStep & ref)
{
	this->mpParentInterface = nullptr;
	this->mpTargetProcess = nullptr;
	this->mpProgram = nullptr;
}


