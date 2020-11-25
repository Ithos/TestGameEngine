#include "../../../Item Utils/Viewport.h"
#include "../../../GeometryItem.h"
#include"../../../CommonItemParameters.h"
#include "..\..\LightUtils\LightComponentManager.h"
#include "..\..\LightUtils\LightShadingComponents\DefaultShadowMap.h"
#include "ShadowMapLight.h"

const std::string GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapConstants::ShadowMapConstants::SHADOW_MAP_FRAGMENT_SHADER = "SHADOW_MAP_FRAGMENT_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapConstants::ShadowMapConstants::POSITION_VERTEX_SHADER = "POSITION_VERTEX_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapConstants::ShadowMapConstants::DIRECTIONAL_SHADOW_LIGHT_FRAGMENT_SHADER = "DIRECTIONAL_SHADOW_LIGHT_FRAGMENT_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapConstants::ShadowMapConstants::DIRECTIONAL_SHADOW_LIGHT_VERTEX_SHADER = "DIRECTIONAL_SHADOW_LIGHT_VERTEX_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapConstants::ShadowMapConstants::SPOTLIGHT_SHADOW_LIGHT_FRAGMENT_SHADER = "SPOTLIGHT_SHADOW_LIGHT_FRAGMENT_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapConstants::ShadowMapConstants::SPOTLIGHT_SHADOW_LIGHT_VERTEX_SHADER = "SPOTLIGHT_SHADOW_LIGHT_VERTEX_SHADER";

const std::string GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapConstants::ShadowMapConstants::DIRECTIONAL_TRANSLUCENT_SHADOW_LIGHT_FRAGMENT_SHADER = "DIRECTIONAL_TRANSLUCENT_SHADOW_LIGHT_FRAGMENT_SHADER";
const std::string GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapConstants::ShadowMapConstants::SPOTLIGHT_TRANSLUCENT_SHADOW_LIGHT_FRAGMENT_SHADER = "SPOTLIGHT_TRANSLUCENT_SHADOW_LIGHT_FRAGMENT_SHADER";

GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight::ShadowMapLight(const GeometryItemUtils::Viewport& viewport, const QVector3D& direction,  GeometryItem::GeometryItem * boundingBox,
	const QVector3D & diffuse, const QVector3D & ambient, const QVector3D & specular, const QVector3D & pos, 
	const QVector3D & rot, float maxShadowBias, const QVector3D & scale, const LightUtils::LightComponentManager* const manager, WorldItem * parent) : mpViewport(nullptr), mDirection(direction), 
	mMaxShadowBias(maxShadowBias), StencilTestLight(boundingBox, diffuse, ambient, specular, pos, rot, scale, manager, parent)
{
	mpViewport = viewport.Clone();
	mMatricesMap[LightTransformationMatrices::LIGHTSPACE_TRANSFORMATION_MATRICES] =
		new LightingTransformationData(mpViewport->GetProjectionMatrix(), mpViewport->GetViewMatrix(), this->GetModelMatrix(), this->GetRotation());
}

GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight::~ShadowMapLight()
{
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

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight::checkLightFunctionalities()
{
	StencilTestLight::checkLightFunctionalities();
	checkShadowMapFuntionality();
}

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight::checkShadowMapFuntionality()
{
	assert(mpFunctionalitiesManager != nullptr && "No light funtionalitites manager found");
	{
		if (!mpFunctionalitiesManager->ContainsLightShadingComponent(LightUtils::DEFAULT_SHADOWMAP))
		{
			mpFunctionalitiesManager->AddNewLightShadingComponent< LightUtils::DefaultShadowMap <ShadowMapLight> >(LightUtils::DEFAULT_SHADOWMAP);
			mpFunctionalitiesManager->SetTargetLight(this, LightUtils::DEFAULT_SHADOWMAP);
			mpFunctionalitiesManager->InitLightShadingComponent(LightUtils::DEFAULT_SHADOWMAP);
		}
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight::copy(const ShadowMapLight & ref)
{
	StencilTestLight::copy(ref);
	this->mpViewport = nullptr;
	this->mDirection = ref.mDirection;

	if (ref.mpViewport != nullptr)
	{
		this->mpViewport = ref.mpViewport->Clone();
	}
}
