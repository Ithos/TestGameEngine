#include "LightFunction.h"

GeometryEngine::LightUtils::LightFunction::LightFunction(LightFunctionalities * parent, LightFunctions step) : mpProgram(nullptr), mpShaderManager(nullptr),
mpTargetLight(nullptr), mpParentInterface(parent)
{
	mpProgram = new QOpenGLShaderProgram();
	mpParentInterface->AddNewLightFunction(this, step);
}

GeometryEngine::LightUtils::LightFunction::~LightFunction()
{
	if (mpProgram != nullptr)
	{
		delete mpProgram;
		mpProgram = nullptr;
	}
}

void GeometryEngine::LightUtils::LightFunction::InitCustomShading(ShaderFiles::ShaderManager * shaderManager)
{
	mpShaderManager = shaderManager;
	this->mpProgram = new QOpenGLShaderProgram();
	this->initShaders();
	this->initProgram();
}

void GeometryEngine::LightUtils::LightFunction::ApplyFunction(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, 
	const QMatrix4x4& modelMatrix, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	assert(mpProgram != nullptr && "No shader program found");
	{
		// Bind shader pipeline for use
		if (!mpProgram->bind())
		{
			assert(false && "Shader failed to bind");
		}

		setProgramParameters(projectionMatrix, viewMatrix, modelMatrix);
		applyShader(vertexBuf, indexBuf, totalVertexNum, totalIndexNum);
	}
}

void GeometryEngine::LightUtils::LightFunction::initProgram()
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

void GeometryEngine::LightUtils::LightFunction::AddToInterface(LightFunctionalities * parent, LightFunctions step)
{
	mpParentInterface = parent;
	mpParentInterface->AddNewLightFunction(this, step);
}

void GeometryEngine::LightUtils::LightFunction::copy(const LightFunction & ref)
{
	this->mpParentInterface = nullptr;
	this->mpTargetLight = nullptr;
	this->mpProgram = nullptr;
}
