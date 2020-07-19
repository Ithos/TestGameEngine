#include "../Items/CommonItemParameters.h"
#include "Items\GraphicItems\Light.h"
#include "Items\GeometryItem.h"
#include "Items\Materials\Material.h"
#include "Items\GraphicItems\Camera.h"
#include"../GBuffer.h"
#include "../TranslucentBuffer.h"
#include "../RenderBuffersData.h"
#include "TransparentShadowedLightingPass.h"

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::calculateSingleLightShadowMap(GeometryWorldItem::GeometryLight::Light * light, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items)
{
	std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem* > transparentItems;
	std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem* > translucentItems;

	for (auto it = items->begin(); it != items->end(); ++it)
	{
		GeometryWorldItem::GeometryItem::GeometryItem* item = (*it);
		if (item->CastsShadows())
		{
			if (item->GetMaterialPtr()->IsTransparent())transparentItems.insert(item);
			else
			{
				calculateItemShadowMap(item, light);
			}
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
			//if (item->GetMaterialPtr()->IsTranslucent()) translucentItems.insert(item);
			//else 
			calculateItemShadowMap(item, light);
		}
	}

	if (mFrontFaceCulling) // return to the previous state
	{
		glCullFace(GL_FRONT);
	}


	// TODO
}
/*
void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::CalculateShadowMap(GeometryWorldItem::GeometryCamera::Camera * cam, 
	std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* shadowedLights, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items)
{
	std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*> transparentItems;

	GeometryEngine::GeometryBuffer::GBuffer* geomBuffer = cam->GetGBuffer();
	GeometryEngine::GeometryBuffer::TranslucentBuffer* trBuffer = cam->GetRenderBufferData()->GetTranslucentBuffer();

	GBufferTextureInfo gbuff(geomBuffer->GetTextureSize());
	geomBuffer->FillGBufferInfo(gbuff);

	TBufferTextureInfo tbuff(trBuffer->GetTextureSize());
	trBuffer->FillGBufferInfo(tbuff);

	

	for (auto it = shadowedLights->begin(); it != shadowedLights->end(); ++it)
	{
		GeometryWorldItem::GeometryLight::Light* shadowLight = (*it);

		trBuffer->StartFrame(); // clear translucent buffer for every light

		if (shadowLight->GetCastShadows())
		{
			initShadowStep(trBuffer);
			calculateSingleLightShadowMap(shadowLight, items);
			finishShadowStep(trBuffer);
		}

		// TODO

	}
}

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::initShadowStep(GeometryBuffer::TranslucentBuffer * buf)
{
	buf->BindTexture(GeometryEngine::GeometryBuffer::TranslucentBuffer::TBUFFER_TEXTURE_TYPE_SHADOW_MAP);
	buf->BindShadowMapTextureWrite();
	glEnable(GL_DEPTH_TEST);
	if (mFrontFaceCulling)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}
}

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::finishShadowStep(GeometryBuffer::TranslucentBuffer * buf)
{
	if (mFrontFaceCulling)
	{
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
	}

	buf->UnbindTexture(GeometryEngine::GeometryBuffer::TranslucentBuffer::TBUFFER_TEXTURE_TYPE_SHADOW_MAP);
}
*/