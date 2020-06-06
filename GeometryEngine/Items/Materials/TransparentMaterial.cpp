#include "TransparentMaterial.h"

GeometryEngine::GeometryMaterial::TransparentMaterial::TransparentMaterial(float thresholdValue, float globalAlphaValue, float shininess): Material(shininess), 
mGlobalAlphaValue(globalAlphaValue), mThresholdValue(thresholdValue)
{
	mGlobalAlphaValue = checkAlphaValue(mGlobalAlphaValue);
	mThresholdValue = checkAlphaValue(mThresholdValue);
	mApplyCustomShadowMap = true;
}

GeometryEngine::GeometryMaterial::TransparentMaterial::TransparentMaterial(const TransparentMaterial & ref)
{
	copy(ref);
}

GeometryEngine::GeometryMaterial::TransparentMaterial::~TransparentMaterial()
{
}

void GeometryEngine::GeometryMaterial::TransparentMaterial::CalculateCustomShadowMap(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, const QMatrix4x4 & modelViewProjection, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	assert(mpShadowMapProgram != nullptr && "No shadow map program found");
	{
		// Bind shader pipeline for use
		if (!mpShadowMapProgram->bind())
		{
			assert(false && "Shadow map shader failed to bind");
		}

		setShadowProgramParameters(modelViewProjection);
		modifyRenderShadowMap(vertexBuf, indexBuf, totalVertexNum, totalIndexNum);
	}
}

void GeometryEngine::GeometryMaterial::TransparentMaterial::modifyRenderShadowMap(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	if (mDrawBothFaces) glDisable(GL_CULL_FACE);
	{
		renderShadowMap(vertexBuf, indexBuf, totalVertexNum, totalIndexNum);
	}
	if (mDrawBothFaces) glEnable(GL_CULL_FACE);
}

void GeometryEngine::GeometryMaterial::TransparentMaterial::initMaterial()
{
	Material::initMaterial();
	mpShadowMapProgram = new QOpenGLShaderProgram();
	initShadowMapShaders();
	initShadowMapProgram();
}

void GeometryEngine::GeometryMaterial::TransparentMaterial::initShadowMapProgram()
{
	if (mShadowMapVertexShaderKey != "")
	{
		if (!mpShaderManager->IsLoaded(mShadowMapVertexShaderKey))
		{
			mpShaderManager->LoadShader(mShadowMapVertexShaderKey);
		}

		// Compile vertex shader
		if (!mpShadowMapProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, mpShaderManager->GetLoadedShaderContent(mShadowMapVertexShaderKey).c_str()))
		{
			assert(false && "TransparentMaterial --> VertexShader failed to compile");
		}
	}

	if (mShadowMapFragmentShaderKey != "")
	{
		if (!mpShaderManager->IsLoaded(mShadowMapFragmentShaderKey))
		{
			mpShaderManager->LoadShader(mShadowMapFragmentShaderKey);
		}

		// Compile fragment shader
		if (!mpShadowMapProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, mpShaderManager->GetLoadedShaderContent(mShadowMapFragmentShaderKey).c_str()))
		{
			assert(false && "TransparentMaterial --> FragmentShader failed to compile");
		}
	}

	assert(mpShadowMapProgram != nullptr && "No material shader program found");
	{
		// Link shader pipeline
		if (!mpShadowMapProgram->link())
		{
			assert(false && "TransparentMaterial --> Program failed to link");
		}
	}
}

void GeometryEngine::GeometryMaterial::TransparentMaterial::copy(const TransparentMaterial & mat)
{
	Material::copy(mat);
	mGlobalAlphaValue = mat.mGlobalAlphaValue;
	mThresholdValue = mat.mThresholdValue;
}

float GeometryEngine::GeometryMaterial::TransparentMaterial::checkAlphaValue(float alphaValue)
{
	if (alphaValue > 1.0f) return 1.0f; 
	if (alphaValue < 0.0f) return 0.0f; 
	return alphaValue; 
}
