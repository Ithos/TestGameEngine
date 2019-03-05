#include "StencilTestLight.h"

GeometryEngine::StencilTestLight::StencilTestLight(GeometryItem * boundingBox, const QVector3D & diffuse, const QVector3D & ambient, const QVector3D & specular, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent):
	DeferredShadingLight(boundingBox, diffuse, ambient, specular, pos, rot, scale, parent), mpStencilProgram(nullptr)
{
	initStencilShaders();
}

GeometryEngine::StencilTestLight::~StencilTestLight()
{
	if (mpStencilProgram != nullptr)
	{
		delete mpStencilProgram;
		mpStencilProgram = nullptr;
	}
}

void GeometryEngine::StencilTestLight::CalculateStencil (const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix)
{
	assert(GetBoundingGeometry() != nullptr && "Bounding geometry not found");
	assert(mpStencilProgram != nullptr && "Shading program not found");

	if (mpStencilProgram != nullptr)
	{
		// Link shader pipeline
		if (!mpStencilProgram->link())
		{
			/// TODO -- log error -- ///
			return;
		}

		// Bind shader pipeline for use
		if (!mpStencilProgram->bind())
		{
			/// TODO -- log error -- ///
			return;
		}

		mpStencilProgram->setUniformValue("mModelViewProjectionMatrix", projectionMatrix * viewMatrix * mpBoundingBox->GetModelMatrix());

		runStencilProgram(mpBoundingBox->GetArrayBuffer(), mpBoundingBox->GetIndexBuffer(), mpBoundingBox->GetVertexNumber(), mpBoundingBox->GetIndexNumber());
	}
}

void GeometryEngine::StencilTestLight::initStencilShaders()
{
	mpConfInstance = Configuration::ConfigurationManager::GetInstance();
	mpShaderManager = ShaderFiles::ShaderManager::GetInstance(mpConfInstance->getVertexShaderFolder(), mpConfInstance->getFragmentShaderFolder(),
		mpConfInstance->getVertexShaderConfig(), mpConfInstance->getFragmentShaderConfig());

	mStencilVertexShaderKey = GeometryEngine::LightShaderConstants::POSITION_VERTEX_SHADER;
	mStencilFragmentShaderKey = GeometryEngine::LightShaderConstants::NULL_FRAGMENT_SHADER;

	mpStencilProgram = new QOpenGLShaderProgram();

	initStencilProgram();
}

void GeometryEngine::StencilTestLight::initStencilProgram()
{
	if (mStencilVertexShaderKey != "")
	{
		if (!mpShaderManager->IsLoaded(mStencilVertexShaderKey))
		{
			mpShaderManager->LoadShader(mStencilVertexShaderKey);
		}

		// Compile vertex shader
		if (!mpStencilProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, mpShaderManager->GetLoadedShaderContent(mStencilVertexShaderKey).c_str()))
		{
			/// TODO -- log error -- ///
			return;
		}
	}

	if (mStencilFragmentShaderKey != "")
	{
		if (!mpShaderManager->IsLoaded(mStencilFragmentShaderKey))
		{
			mpShaderManager->LoadShader(mStencilFragmentShaderKey);
		}

		// Compile fragment shader
		if (!mpStencilProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, mpShaderManager->GetLoadedShaderContent(mStencilFragmentShaderKey).c_str()))
		{
			/// TODO -- log error -- ///
			return;
		}
	}
}

void GeometryEngine::StencilTestLight::runStencilProgram(QOpenGLBuffer * arrayBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	// Tell OpenGL which VBOs to use
	arrayBuf->bind();
	indexBuf->bind();

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = mpStencilProgram->attributeLocation("posAttr");
	mpStencilProgram->enableAttributeArray(vertexLocation);
	mpStencilProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

	// Draw light
	glDrawElements(GL_TRIANGLE_STRIP, totalIndexNum, GL_UNSIGNED_SHORT, 0);
}
