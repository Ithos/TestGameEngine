#include "StencilTestLight.h"

GeometryEngine::GeometryWorldItem::GeometryLight::StencilTestLight::StencilTestLight(GeometryItem::GeometryItem * boundingBox, const QVector3D & diffuse, 
	const QVector3D & ambient, const QVector3D & specular, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent):
	DeferredShadingLight(boundingBox, diffuse, ambient, specular, pos, rot, scale, parent), mpStencilProgram(nullptr)
{
	initStencilShaders();
}

GeometryEngine::GeometryWorldItem::GeometryLight::StencilTestLight::~StencilTestLight()
{
	if (mpStencilProgram != nullptr)
	{
		delete mpStencilProgram;
		mpStencilProgram = nullptr;
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::StencilTestLight::CalculateStencil (const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix)
{
	assert(GetBoundingGeometry() != nullptr && "Bounding geometry not found");
	assert(mpStencilProgram != nullptr && "Shading program not found");

	if (mpStencilProgram != nullptr)
	{
		// Link shader pipeline
		if (!mpStencilProgram->link())
		{
			assert(false && "StencilTestLight --> Shader program failed to link");
		}

		// Bind shader pipeline for use
		if (!mpStencilProgram->bind())
		{
			assert(false && "StencilTestLight --> Shader program failed to bind");
		}

		mpStencilProgram->setUniformValue("mModelViewProjectionMatrix", projectionMatrix * viewMatrix * mpBoundingBox->GetModelMatrix());

		runStencilProgram(mpBoundingBox->GetArrayBuffer(), mpBoundingBox->GetIndexBuffer(), mpBoundingBox->GetVertexNumber(), mpBoundingBox->GetIndexNumber());
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::StencilTestLight::initStencilShaders()
{
	mpConfInstance = Configuration::ConfigurationManager::GetInstance();
	mpShaderManager = ShaderFiles::ShaderManager::GetInstance(mpConfInstance->getVertexShaderFolder(), mpConfInstance->getFragmentShaderFolder(),
		mpConfInstance->getVertexShaderConfig(), mpConfInstance->getFragmentShaderConfig());

	mStencilVertexShaderKey = GeometryEngine::GeometryWorldItem::GeometryLight::LightShaderConstants::POSITION_VERTEX_SHADER;
	mStencilFragmentShaderKey = GeometryEngine::GeometryWorldItem::GeometryLight::LightShaderConstants::NULL_FRAGMENT_SHADER;

	mpStencilProgram = new QOpenGLShaderProgram();

	initStencilProgram();
}

void GeometryEngine::GeometryWorldItem::GeometryLight::StencilTestLight::initStencilProgram()
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
			assert(false && "StencilTestLight --> Failed to load vertex shader");
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
			assert(false && "StencilTestLight --> Failed to load fragment shader");
		}
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::StencilTestLight::runStencilProgram(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	// Tell OpenGL which VBOs to use
	vertexBuf->bind();
	indexBuf->bind();

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = mpStencilProgram->attributeLocation("posAttr");
	mpStencilProgram->enableAttributeArray(vertexLocation);
	mpStencilProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

	// Draw light
	glDrawElements(GL_TRIANGLE_STRIP, totalIndexNum, GL_UNSIGNED_SHORT, 0);
}
