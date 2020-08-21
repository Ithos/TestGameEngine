#include "../GeometryItem.h"
#include "LightUtils\LightFunctionalities.h"
#include "LightUtils\LightFunctions\StencilTesting.h"
#include "StencilTestLight.h"

GeometryEngine::GeometryWorldItem::GeometryLight::StencilTestLight::StencilTestLight(GeometryItem::GeometryItem * boundingBox, const QVector3D & diffuse, 
	const QVector3D & ambient, const QVector3D & specular, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, const LightUtils::LightFunctionalities* const manager, WorldItem * parent):
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
		if (!mpFunctionalitiesManager->ContainsFunction(LightUtils::STENCIL_TESTING))
		{
			mpFunctionalitiesManager->AddNewLightFunction<LightUtils::StencilTesting<StencilTestLight> >(LightUtils::STENCIL_TESTING);
			mpFunctionalitiesManager->SetTargetLight(this, LightUtils::STENCIL_TESTING);
			mpFunctionalitiesManager->InitLightFunction(LightUtils::STENCIL_TESTING);
		}
	}
}
