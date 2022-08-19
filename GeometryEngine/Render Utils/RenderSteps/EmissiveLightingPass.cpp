#include "Items\GraphicItems\Light.h"
#include "EmissiveLightingPass.h"

GeometryEngine::GeometryRenderStep::EmissiveLightingPass::EmissiveLightingPass(const GeometryWorldItem::GeometryLight::Light& emissiveLighting) : LightingPass()
{
	mpEmissiveLighting = emissiveLighting.Clone();
	mEmissiveLights.insert(mpEmissiveLighting);
}

GeometryEngine::GeometryRenderStep::EmissiveLightingPass::~EmissiveLightingPass()
{
	if (mpEmissiveLighting != nullptr)
	{
		delete mpEmissiveLighting;
		mpEmissiveLighting = nullptr;
	}
	mEmissiveLights.clear();
}

void GeometryEngine::GeometryRenderStep::EmissiveLightingPass::Render(GeometryWorldItem::GeometryCamera::Camera * cam, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems, std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* lights)
{
	assert(cam != nullptr && "LightingPass --> No camera found");
	initStep();
	renderLights(cam, &mEmissiveLights);
	finishStep();
}
