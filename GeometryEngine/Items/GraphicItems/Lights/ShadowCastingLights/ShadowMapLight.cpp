#include "../../../Item Utils/Viewport.h"
#include "../../../GeometryItem.h"
#include "ShadowMapLight.h"

const std::string GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapConstants::ShadowMapConstants::SHADOW_MAP_FRAGMENT_SHADER = "SHADOW_MAP_FRAGMENT_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapConstants::ShadowMapConstants::POSITION_VERTEX_SHADER = "POSITION_VERTEX_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapConstants::ShadowMapConstants::DIRECTIONAL_SHADOW_LIGHT_FRAGMENT_SHADER = "DIRECTIONAL_SHADOW_LIGHT_FRAGMENT_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapConstants::ShadowMapConstants::DIRECTIONAL_SHADOW_LIGHT_VERTEX_SHADER = "DIRECTIONAL_SHADOW_LIGHT_VERTEX_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapConstants::ShadowMapConstants::SPOTLIGHT_SHADOW_LIGHT_FRAGMENT_SHADER = "SPOTLIGHT_SHADOW_LIGHT_FRAGMENT_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapConstants::ShadowMapConstants::SPOTLIGHT_SHADOW_LIGHT_VERTEX_SHADER = "SPOTLIGHT_SHADOW_LIGHT_VERTEX_SHADER";

GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight::ShadowMapLight(const GeometryItemUtils::Viewport& viewport, const QVector3D& direction, GeometryItem::GeometryItem * boundingBox,
	const QVector3D & diffuse, const QVector3D & ambient, const QVector3D & specular, const QVector3D & pos, 
	const QVector3D & rot, const QVector3D & scale, WorldItem * parent) : mpViewport(nullptr), mDirection(direction),
	mpShadowMapProgram(nullptr), mSMapVertexShaderKey(""), mSMapFragmentShaderKey(""), 
	StencilTestLight(boundingBox, diffuse, ambient, specular, pos, rot, scale, parent)
{
	mpViewport = viewport.Clone();
}

GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight::~ShadowMapLight()
{
	if (mpShadowMapProgram != nullptr)
	{
		delete mpShadowMapProgram;
		mpShadowMapProgram = nullptr;
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight::CalculateShadowMap(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& modelMatrix, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	assert(mpShadowMapProgram != nullptr && "No shadow map program found");
	{
		// Bind shader pipeline for use
		if (!mpShadowMapProgram->bind())
		{
			assert(false && "Shadow map shader failed to bind");
		}

		setShadowProgramParameters(modelMatrix);
		renderShadowMap(vertexBuf, indexBuf, totalVertexNum, totalIndexNum);
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight::UpdateModelMatrix(bool updateChildren)
{
	DeferredShadingLight::UpdateModelMatrix(updateChildren);
	QVector3D direction = mRotation.rotatedVector(QVector3D(0.0f, 0.0f, -1.0f));
	QQuaternion secondRotation = QQuaternion::rotationTo(mDirection, direction );
	QMatrix3x3 rot = secondRotation.toRotationMatrix();
	mpViewport->UpdateViewMatrix(GetPosition(), secondRotation * mRotation, mScale);
}

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight::ResizeElements(int screenWidth, int screenHeight)
{
	mpViewport->SetViewportSize(QVector4D(0, 0, screenWidth, screenHeight));
}

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight::initShadow()
{
	if(mpConfInstance == nullptr) mpConfInstance = Configuration::ConfigurationManager::GetInstance();
	if (mpShaderManager == nullptr) mpShaderManager = ShaderFiles::ShaderManager::GetInstance(mpConfInstance->getVertexShaderFolder(), mpConfInstance->getFragmentShaderFolder(),
		mpConfInstance->getVertexShaderConfig(), mpConfInstance->getFragmentShaderConfig());

	mpShadowMapProgram = new QOpenGLShaderProgram();

	this->initShadowShaders();
	this->initShadowProgram();
}

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight::initShadowProgram()
{
	if (mSMapVertexShaderKey != "")
	{
		if (!mpShaderManager->IsLoaded(mSMapVertexShaderKey))
		{
			mpShaderManager->LoadShader(mSMapVertexShaderKey);
		}

		// Compile vertex shader
		if (!mpShadowMapProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, mpShaderManager->GetLoadedShaderContent(mSMapVertexShaderKey).c_str()))
		{
			assert(false && "Lighting vertex shader failed to compile");
		}
	}

	if (mSMapFragmentShaderKey != "")
	{
		if (!mpShaderManager->IsLoaded(mSMapFragmentShaderKey))
		{
			mpShaderManager->LoadShader(mSMapFragmentShaderKey);
		}

		// Compile fragment shader
		if (!mpShadowMapProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, mpShaderManager->GetLoadedShaderContent(mSMapFragmentShaderKey).c_str()))
		{
			assert(false && "Lighting fragment shader failed to compile");
		}
	}

	assert(mpShadowMapProgram != nullptr && "No shadow map program found");
	{
		// Link shader pipeline
		if (!mpShadowMapProgram->link())
		{
			assert(false && "Shadow map shader failed to link");
		}
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight::initShadowShaders()
{
	mSMapVertexShaderKey  = ShadowMapConstants::POSITION_VERTEX_SHADER;
	mSMapFragmentShaderKey = ShadowMapConstants::SHADOW_MAP_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight::setShadowProgramParameters(const QMatrix4x4& modelMatrix)
{
	assert(mpShadowMapProgram != nullptr && "Shading shadow map program not found");
	{
		UpdateModelMatrix(true);
		mpViewport->CalculateProjectionMatrix();
		QMatrix4x4 modelViewProjection = mpViewport->GetViewProjectionMatrix() * modelMatrix;

		mpShadowMapProgram->setUniformValue("mModelViewProjectionMatrix", modelViewProjection);
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight::renderShadowMap(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	// Tell OpenGL which VBOs to use
	vertexBuf->bind();
	indexBuf->bind();

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = mpShadowMapProgram->attributeLocation("posAttr");
	mpShadowMapProgram->enableAttributeArray(vertexLocation);
	mpShadowMapProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

	// Draw light
	glDrawElements(GL_TRIANGLE_STRIP, totalIndexNum, GL_UNSIGNED_SHORT, 0);
}
