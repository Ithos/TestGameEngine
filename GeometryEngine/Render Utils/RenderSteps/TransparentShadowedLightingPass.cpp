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


void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::calculateSingleLightColorMap(GeometryWorldItem::GeometryLight::Light * light, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * translucentItems)
{
	for (auto it = translucentItems->begin(); it != translucentItems->end(); ++it)
	{
		GeometryWorldItem::GeometryItem::GeometryItem* item = (*it).second;
		calculateItemTranslucentShadowing(item, light);
	}
}

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::calculateSingleLightTranslucentShadowMap(GeometryWorldItem::GeometryLight::Light * light, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * translucentItems)
{
	for (auto it = translucentItems->begin(); it != translucentItems->end(); ++it)
	{
		GeometryWorldItem::GeometryItem::GeometryItem* item = (*it).second;
		if (item->CastsShadows())
		{
			calculateItemShadowMap(item, light);
		}
	}
}

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::CalculateShadowMap(GeometryWorldItem::GeometryCamera::Camera * cam, 
	std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* shadowedLights, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems)
{
	std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> opaqueItems;
	std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> translucentItems;

	GeometryEngine::GeometryBuffer::GBuffer* geomBuffer = cam->GetGBuffer();
	GeometryEngine::GeometryBuffer::ShadingBuffer* trBuffer = cam->GetRenderBufferData()->GetShadingBuffer();

	GBufferTextureInfo gbuff(geomBuffer->GetTextureSize());
	geomBuffer->FillGBufferInfo(gbuff);

	ShadingBufferTextureInfo tbuff(trBuffer->GetTextureSize());
	trBuffer->FillShadingBufferInfo(tbuff);

	trBuffer->StartFrame();

	BuffersInfo buff;
	buff.GeometryBufferInfo = &gbuff;
	buff.ShadingBufferInfo = &tbuff;

	
	for (auto it = orderedItems->rbegin(); it != orderedItems->rend(); ++it)
	{
		if ((*it).second->GetMaterialPtr()->IsTranslucent()) translucentItems[(*it).first] = (*it).second;
		//if ((*it)->GetMaterialPtr()->IsTransparent()) translucentItems.insert((*it));
		//if(dynamic_cast<GeometryEngine::GeometryWorldItem::GeometryItem::Cube*>((*it)) != nullptr )translucentItems.insert((*it));
		else opaqueItems[(*it).first] = (*it).second;
	}


	for (auto it = shadowedLights->begin(); it != shadowedLights->end(); ++it)
	{
		GeometryWorldItem::GeometryLight::Light* shadowLight = (*it);

		trBuffer->StartFrame(); // clear shadowing buffer for every light

		if ( shadowLight->GetLightFunctionalities()->ContainsLightShadingComponent(LightUtils::DEFAULT_SHADOWMAP) )
		{
			initShadowStep(trBuffer);
			calculateSingleLightShadowMap(shadowLight, &opaqueItems);
			finishShadowStep(trBuffer);

			initTransparentShadowMap(trBuffer);
			calculateSingleLightTranslucentShadowMap(shadowLight, &translucentItems);
			finishTransparentShadowMap(trBuffer);

			initColorMap(trBuffer);
			calculateSingleLightColorMap(shadowLight, &translucentItems);
			finishColorMap(trBuffer);
		}

		//Apply light
		LightingPass::initStep();
		bindRenderTextures(geomBuffer, trBuffer);

		LightingPass::applySingleLight(cam, buff, shadowLight);
		LightingPass::finishStep();

	}

	geomBuffer->UnbindBuffer();
}

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::calculateItemTranslucentShadowing(GeometryWorldItem::GeometryItem::GeometryItem * item, GeometryWorldItem::GeometryLight::Light * light)
{
	const GeometryEngine::LightingTransformationData* transf = 
		light->GetLightTransformationMatrices(GeometryEngine::GeometryWorldItem::GeometryLight::LightTransformationMatrices::LIGHTSPACE_TRANSFORMATION_MATRICES);
	CustomShading::MultiShadingInterface* sdInterf = item->GetMaterialPtr()->GetShadingInterface();
	CustomShading::CustomShadingInterface* customSd = (sdInterf != nullptr && sdInterf->ContainsList(CustomShading::ShadingLists::SHADING_LIST)) ?
		sdInterf->GetList(CustomShading::ShadingLists::SHADING_LIST) : nullptr;

	if (transf != nullptr && customSd != nullptr && customSd->ContainsStep(GeometryEngine::CustomShading::CUSTOM_COLORMAP))
	{
		customSd->RenderStep(GeometryEngine::CustomShading::CUSTOM_COLORMAP, item->GetArrayBuffer(),
			item->GetIndexBuffer(), transf->ProjectionMatrix * transf->ViewMatrix * item->GetModelMatrix(), item->GetVertexNumber(), item->GetIndexNumber());
	}
}

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::initTransparentShadowMap(GeometryBuffer::ShadingBuffer * buf)
{
	buf->DetachDepthTexture();
	buf->ClearColorTexture(GeometryEngine::GeometryBuffer::ShadingBuffer::SHADINGBUFFER_TEXTURE_TYPE_TRANSLUCENT_DEPTH_MAP);
	buf->BindTranslucentDepthMapWrite();
	
	glEnable(GL_DEPTH_TEST);
	if (mFrontFaceCulling)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	glClear(GL_DEPTH_BUFFER_BIT);
}

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::finishTransparentShadowMap(GeometryBuffer::ShadingBuffer * buf)
{
	if (mFrontFaceCulling)
	{
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
	}

	buf->UnbindTexture(GeometryEngine::GeometryBuffer::ShadingBuffer::SHADINGBUFFER_TEXTURE_TYPE_TRANSLUCENT_DEPTH_MAP);
	buf->AttachDepthTexture();
}

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::initColorMap(GeometryBuffer::ShadingBuffer * buf)
{

	buf->BindColorMapsWrite();
	// Do not update depth buffer
	glDepthMask(GL_FALSE);

	// enable multiplicative blending
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_DST_COLOR, GL_ZERO);
}

void GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass::finishColorMap(GeometryBuffer::ShadingBuffer * buf)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);

	glDepthMask(GL_TRUE);

	buf->UnbindTexture(GeometryEngine::GeometryBuffer::ShadingBuffer::SHADINGBUFFER_TEXTURE_TYPE_DIFFUSE_MAP);
	buf->UnbindTexture(GeometryEngine::GeometryBuffer::ShadingBuffer::SHADINGBUFFER_TEXTURE_TYPE_SPECULAR_MAP);
}
