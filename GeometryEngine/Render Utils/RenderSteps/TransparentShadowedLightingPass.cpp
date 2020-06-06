#include "Items\GraphicItems\Light.h"
#include "Items\GeometryItem.h"
#include "Items\Materials\Material.h"
#include "TransparentShadowedLightingPass.h"

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::calculateSingleLightShadowMap(GeometryWorldItem::GeometryLight::Light * light, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items)
{
	std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem* > transparentItems;

	for (auto it = items->begin(); it != items->end(); ++it)
	{
		GeometryWorldItem::GeometryItem::GeometryItem* item = (*it);
		if (item->CastsShadows())
		{
			if (item->GetMaterialPtr()->IsTransparent())transparentItems.insert(item);
			else calculateItemShadowMap(item, light);
		}
	}

	if (mFrontFaceCulling) // Backface culling leads to weird effects when calculating shadows with transparencies
	{
		glCullFace(GL_BACK);
	}

	for (auto it = transparentItems.begin(); it != transparentItems.end(); ++it)
	{
		GeometryWorldItem::GeometryItem::GeometryItem* item = (*it);
		if (item->CastsShadows())
		{
			calculateItemShadowMap(item, light);
		}
	}

	if (mFrontFaceCulling) // return to the previous state
	{
		glCullFace(GL_FRONT);
	}
}
