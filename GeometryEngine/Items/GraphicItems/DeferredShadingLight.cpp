#include "../CommonItemParameters.h"
#include "../GeometryItem.h"
#include "LightUtils\LightFunctionalities.h"
#include "LightUtils\LightRenderTechniques\BoundingGeometryLighting.h"
#include "DeferredShadingLight.h"

GeometryEngine::GeometryWorldItem::GeometryLight::DeferredShadingLight::DeferredShadingLight(GeometryItem::GeometryItem * boundingBox, const QVector3D & diffuse, const QVector3D & ambient,
	const QVector3D & specular, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, const LightUtils::LightFunctionalities* const manager, WorldItem * parent) : 
	Light(diffuse, ambient, specular, pos, rot, scale, manager, parent), mpBoundingBox(nullptr)
{
	if (boundingBox != nullptr)
	{
		mpBoundingBox = boundingBox->Clone();
	}
}

GeometryEngine::GeometryWorldItem::GeometryLight::DeferredShadingLight::~DeferredShadingLight()
{
	if (mpBoundingBox != nullptr)
	{
		delete(mpBoundingBox);
		mpBoundingBox = nullptr;
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::DeferredShadingLight::checkLightFunctionalities()
{
	Light::checkLightFunctionalities();
	checkDeferredShadingTechnique();
}

void GeometryEngine::GeometryWorldItem::GeometryLight::DeferredShadingLight::checkDeferredShadingTechnique()
{
	assert(mpFunctionalitiesManager != nullptr && "No light funtionalitites manager found");
	{
		if (!mpFunctionalitiesManager->ContainsTechnique(LightUtils::BOUNDING_GEOMETRY))
		{
			mpFunctionalitiesManager->AddNewLightTechnique< LightUtils::BoundingGeometryLighting<DeferredShadingLight> >(LightUtils::BOUNDING_GEOMETRY);
			mpFunctionalitiesManager->SetTargetLight(this, LightUtils::BOUNDING_GEOMETRY);
		}
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::DeferredShadingLight::ScaleBoundingBox(const QVector3D & attenuation)
{
	if (this->mpBoundingBox != nullptr)
	{
		QVector3D maxColorIntensity = this->mColorDiffuse + this->mColorAmbient + this->mColorSpecular;
		float maxIntensity = std::max(std::max(maxColorIntensity.x(), maxColorIntensity.y()), maxColorIntensity.z());
		if (maxIntensity > 1) maxIntensity = 1.0f;

		float ret = (-attenuation.y() + sqrtf(attenuation.y() * attenuation.y() - 4 * attenuation.z() * (attenuation.x() - 256 * maxIntensity)))
			/
			(2 * attenuation.z());

		mpBoundingBox->Scale(ret * QVector3D(1.0f, 1.0f, 1.0f));
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::DeferredShadingLight::copy(const DeferredShadingLight & ref)
{
	Light::copy(ref);
	this->mpBoundingBox = nullptr;
	if (ref.mpBoundingBox != nullptr)
	{
		this->mpBoundingBox = ref.mpBoundingBox->Clone();
	}
}
