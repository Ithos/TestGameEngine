#include "../Items/CommonItemParameters.h"
#include "Items\GraphicItems\Light.h"
#include "Items\GeometryItem.h"
#include "Items\Materials\Material.h"
#include "Items\GraphicItems\Camera.h"
#include"../GBuffer.h"
#include "../ShadingBuffer.h"
#include "../RenderBuffersData.h"
#include "Items\GraphicItems\LightUtils\LightComponentManager.h"
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
		//if (item->CastsShadows())
		{
			//if (!item->GetMaterialPtr()->IsTranslucent()) 
				calculateItemShadowMap(item, light);
		}
	}

	if (mFrontFaceCulling) // return to the previous state
	{
		glCullFace(GL_FRONT);
	}
}

/*
void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::calculateSingleLightColorMap(GeometryWorldItem::GeometryLight::Light * light, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* translucentItems)
{
	for (auto it = translucentItems->begin(); it != translucentItems->end(); ++it)
	{
		GeometryWorldItem::GeometryItem::GeometryItem* item = (*it);
		calculateItemTranslucentShadowing(item, light);
	}
}

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::calculateSingleLightTranslucentShadowMap(GeometryWorldItem::GeometryLight::Light * light, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* translucentItems)
{
	if (mFrontFaceCulling) // Backface culling leads to weird effects when calculating shadows with transparencies
	{
		glCullFace(GL_BACK);
	}

	for (auto it = translucentItems->begin(); it != translucentItems->end(); ++it)
	{
		GeometryWorldItem::GeometryItem::GeometryItem* item = (*it);
		if (item->CastsShadows())
		{
			if (!item->GetMaterialPtr()->IsTranslucent()) calculateItemShadowMap(item, light);
		}
	}

	if (mFrontFaceCulling) // return to the previous state
	{
		glCullFace(GL_FRONT);
	}
}

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::CalculateShadowMap(GeometryWorldItem::GeometryCamera::Camera * cam, 
	std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* shadowedLights, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items)
{
	std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem* > translucentItems;

	GeometryEngine::GeometryBuffer::GBuffer* geomBuffer = cam->GetGBuffer();
	GeometryEngine::GeometryBuffer::TranslucentBuffer* trBuffer = cam->GetRenderBufferData()->GetTranslucentBuffer();

	GBufferTextureInfo gbuff(geomBuffer->GetTextureSize());
	geomBuffer->FillGBufferInfo(gbuff);

	TBufferTextureInfo tbuff(trBuffer->GetTextureSize());
	trBuffer->FillGBufferInfo(tbuff);

	
	for (auto it = items->begin(); it != items->end(); ++it)
	{
		if ((*it)->GetMaterialPtr()->IsTranslucent()) translucentItems.insert((*it));
	}


	for (auto it = shadowedLights->begin(); it != shadowedLights->end(); ++it)
	{
		GeometryWorldItem::GeometryLight::Light* shadowLight = (*it);

		trBuffer->StartFrame(); // clear translucent buffer for every light

		if ( shadowLight->GetLightFunctionalities()->ContainsFunction(LightUtils::DEFAULT_SHADOWMAP) )
		{
			initShadowStep(trBuffer);
			calculateSingleLightShadowMap(shadowLight, items);
			finishShadowStep(trBuffer);

			initTransparentShadowMap(trBuffer);
			calculateSingleLightTranslucentShadowMap(shadowLight, &translucentItems);
			finishTransparentShadowMap(trBuffer);

			initColorMapStep(trBuffer);
			calculateSingleLightColorMap(shadowLight, &translucentItems);
			finishColorMapStep(trBuffer);

			///TODO -- send textures to the lighting script 
		}
	}
}

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::calculateItemTranslucentShadowing(GeometryWorldItem::GeometryItem::GeometryItem * item, GeometryWorldItem::GeometryLight::Light * light)
{
	const GeometryEngine::LightingTransformationData* transf = light->GetLightTransformationMatrices(GeometryEngine::GeometryWorldItem::GeometryLight::LightTransformationMatrices::LIGHTSPACE_TRANSFORMATION_MATRICES);
	if (transf != nullptr && item->GetMaterialPtr()->GetCustomShaders() != nullptr && item->GetMaterialPtr()->GetCustomShaders()->ContainsStep(GeometryEngine::CustomShading::CUSTOM_COLORMAP))
	{
		item->GetMaterialPtr()->GetCustomShaders()->RenderStep(GeometryEngine::CustomShading::CUSTOM_COLORMAP, item->GetArrayBuffer(),
			item->GetIndexBuffer(), transf->ProjectionMatrix * transf->ViewMatrix * item->GetModelMatrix(), item->GetVertexNumber(), item->GetIndexNumber());
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

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::initColorMapStep(GeometryBuffer::TranslucentBuffer * buf)
{
	buf->BindColorMaps();
	// Do not update depth buffer
	glDepthMask(GL_FALSE);
}

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::finishColorMapStep(GeometryBuffer::TranslucentBuffer * buf)
{
	glDepthMask(GL_TRUE);
}

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::initTransparentShadowMap(GeometryBuffer::TranslucentBuffer * buf)
{
	buf->BindTexture(GeometryEngine::GeometryBuffer::TranslucentBuffer::TBUFFER_TEXTURE_TYPE_TRANSLUCENT_DEPTH_MAP);
	buf->BindShadowMapTextureWrite();
	glEnable(GL_DEPTH_TEST);
	if (mFrontFaceCulling)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}
}

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::finishTransparentShadowMap(GeometryBuffer::TranslucentBuffer * buf)
{
	if (mFrontFaceCulling)
	{
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
	}

	buf->UnbindTexture(GeometryEngine::GeometryBuffer::TranslucentBuffer::TBUFFER_TEXTURE_TYPE_TRANSLUCENT_DEPTH_MAP);
}

*/