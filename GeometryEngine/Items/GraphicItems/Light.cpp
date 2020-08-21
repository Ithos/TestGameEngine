#include "../CommonItemParameters.h"
#include "LightUtils\LightFunctionalities.h"
#include "Light.h"

const std::string GeometryEngine::GeometryWorldItem::GeometryLight::LightShaderConstants::AMBIENT_LIGHT_VERTEX_SHADER = "AMBIENT_LIGHT_VERTEX_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::LightShaderConstants::AMBIENT_LIGHT_FRAGMENT_SHADER = "AMBIENT_LIGHT_FRAGMENT_SHADER";

const std::string GeometryEngine::GeometryWorldItem::GeometryLight::LightShaderConstants::DIRECTIONAL_LIGHT_VERTEX_SHADER = "DIRECTIONAL_LIGHT_VERTEX_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::LightShaderConstants::DIRECTIONAL_LIGHT_FRAGMENT_SHADER = "DIRECTIONAL_LIGHT_FRAGMENT_SHADER";

const std::string GeometryEngine::GeometryWorldItem::GeometryLight::LightShaderConstants::POINT_LIGHT_VERTEX_SHADER = "POINT_LIGHT_VERTEX_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::LightShaderConstants::POINT_LIGHT_FRAGMENT_SHADER = "POINT_LIGHT_FRAGMENT_SHADER";

const std::string GeometryEngine::GeometryWorldItem::GeometryLight::LightShaderConstants::FLASHLIGHT_VERTEX_SHADER = "FLASHLIGHT_VERTEX_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::LightShaderConstants::FLASHLIGHT_FRAGMENT_SHADER = "FLASHLIGHT_FRAGMENT_SHADER";

const std::string GeometryEngine::GeometryWorldItem::GeometryLight::LightShaderConstants::DEFERRED_SHADING_VERTEX_SHADER = "DEFERRED_SHADING_VERTEX_SHADER";

const std::string GeometryEngine::GeometryWorldItem::GeometryLight::LightShaderConstants::LightShaderConstants::NULL_FRAGMENT_SHADER = "NULL_FRAGMENT_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::LightShaderConstants::LightShaderConstants::POSITION_VERTEX_SHADER = "POSITION_VERTEX_SHADER";

const std::string GeometryEngine::GeometryWorldItem::GeometryLight::LightShaderConstants::LightShaderConstants::EMISSIVE_LIGHTING_FRAGMENT_SHADER = "EMISSIVE_LIGHTING_FRAGMENT_SHADER";

GeometryEngine::GeometryWorldItem::GeometryLight::Light::Light(const QVector3D & diffuse, const QVector3D & ambient, const QVector3D & specular, const QVector3D & pos,
	const QVector3D & rot, const QVector3D & scale, const LightUtils::LightFunctionalities* const manager, WorldItem * parent) : WorldItem(pos, rot, scale, parent), mColorDiffuse(diffuse),
	mColorAmbient(ambient), mColorSpecular(specular), mpProgram(nullptr), mVertexShaderKey(""), mFragmentShaderKey(""), mpConfInstance(nullptr), mpShaderManager(nullptr), mpFunctionalitiesManager(nullptr)
{
	if (manager != nullptr)
	{
		mpFunctionalitiesManager = manager->Clone();
		mpFunctionalitiesManager->SetTargetLight(this);
	}
	else
	{
		mpFunctionalitiesManager = new LightUtils::LightFunctionalities();
		mpFunctionalitiesManager->InitFunctions();
	}

	checkLightFunctionalities();
}

GeometryEngine::GeometryWorldItem::GeometryLight::Light::~Light()
{
	if (mpProgram != nullptr)
	{
		delete mpProgram;
		mpProgram = nullptr;
	}

	if (mpFunctionalitiesManager != nullptr)
	{
		delete mpFunctionalitiesManager;
		mpFunctionalitiesManager = nullptr;
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::Light::CalculateLighting(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const LightingTransformationData& transformData,
	const GBufferTextureInfo& gBuffTexInfo, const QVector3D& viewPos, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	assert(mpProgram != nullptr && "No light shader program found");
	{
		// Bind shader pipeline for use
		if (!mpProgram->bind())
		{
			assert(false && "Lighting shader failed to bind");
		}

		setProgramParameters(transformData, gBuffTexInfo, viewPos);

		calculateContribution(vertexBuf, indexBuf, totalVertexNum, totalIndexNum);
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::Light::initLight()
{
	mpConfInstance = Configuration::ConfigurationManager::GetInstance();
	mpShaderManager = ShaderFiles::ShaderManager::GetInstance(mpConfInstance->getVertexShaderFolder(), mpConfInstance->getFragmentShaderFolder(),
		mpConfInstance->getVertexShaderConfig(), mpConfInstance->getFragmentShaderConfig());

	mpProgram = new QOpenGLShaderProgram();

	this->initLightShaders();
	this->initLightProgram();

	checkLightFunctionalities();
}

void GeometryEngine::GeometryWorldItem::GeometryLight::Light::initLightProgram()
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
			assert(false && "Lighting vertex shader failed to compile");
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
			assert(false && "Lighting fragment shader failed to compile");
		}
	}

	assert(mpProgram != nullptr && "No light shader program found");
	{
		// Link shader pipeline
		if (!mpProgram->link())
		{
			assert(false && "Lighting shader failed to link");
		}
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::Light::copy(const Light & ref)
{
	this->mColorDiffuse = ref.mColorDiffuse;
	this->mColorAmbient = ref.mColorAmbient;;
	this->mColorSpecular = ref.mColorSpecular;
	this->mpProgram = nullptr;
	this->mpConfInstance = nullptr;
	this->mpShaderManager = nullptr;
	this->mVertexShaderKey = ref.mVertexShaderKey;
	this->mFragmentShaderKey = ref.mFragmentShaderKey;

	if (ref.mpFunctionalitiesManager != nullptr)
	{
		mpFunctionalitiesManager = ref.mpFunctionalitiesManager->Clone();
		mpFunctionalitiesManager->SetTargetLight(this);
	}
}
