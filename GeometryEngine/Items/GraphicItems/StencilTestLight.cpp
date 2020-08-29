#include "../GeometryItem.h"
#include "LightUtils\LightComponentManager.h"
#include "LightUtils\LightShadingComponents\StencilTesting.h"
#include "StencilTestLight.h"

GeometryEngine::GeometryWorldItem::GeometryLight::StencilTestLight::StencilTestLight(GeometryItem::GeometryItem * boundingBox, const QVector3D & diffuse, 
	const QVector3D & ambient, const QVector3D & specular, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, const LightUtils::LightComponentManager* const manager, WorldItem * parent):
	DeferredShadingLight(boundingBox, diffuse, ambient, specular, pos, rot, scale, manager, parent)
{
}

void GeometryEngine::GeometryWorldItem::GeometryLight::StencilTestLight::checkLightFunctionalities()
{
	DeferredShadingLight::checkLightFunctionalities();
	checkStencylTestFunctionality();
}

void GeometryEngine::GeometryWorldItem::GeometryLight::StencilTestLight::checkStencylTestFunctionality()
{
	assert(mpFunctionalitiesManager != nullptr && "No light funtionalitites manager found");
	{
		if (!mpFunctionalitiesManager->ContainsLightShadingComponent(LightUtils::STENCIL_TESTING))
		{
			mpFunctionalitiesManager->AddNewLightShadingComponent<LightUtils::StencilTesting<StencilTestLight> >(LightUtils::STENCIL_TESTING);
			mpFunctionalitiesManager->SetTargetLight(this, LightUtils::STENCIL_TESTING);
			mpFunctionalitiesManager->InitLightShadingComponent(LightUtils::STENCIL_TESTING);
		}
	}
}
