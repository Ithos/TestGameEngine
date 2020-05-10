#include "../Items/CommonItemParameters.h"
#include "Items\GraphicItems\Camera.h"
#include "Items\GraphicItems\Light.h"
#include "Items\GeometryItem.h"
#include "Items\GraphicItems\Lights\ShadowCastingLights\ShadowMapLight.h"
#include"../GBuffer.h"
#include "ShadowedLightingPass.h"

void GeometryEngine::GeometryRenderStep::ShadowedLightingPass::Render(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items, std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* lights)
{
	assert(cam != nullptr && "ShadowedLightningPass --> No camera found");
	assert(lights != nullptr && "ShadowedLightningPass --> No lights list found");
	assert(items != nullptr && "ShadowedLightningPass --> No itmes list found");

	//Apply shadowed lights
	CalculateShadowMap(cam, lights, items);
}

void GeometryEngine::GeometryRenderStep::ShadowedLightingPass::CalculateShadowMap(GeometryWorldItem::GeometryCamera::Camera * cam, 
	std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* shadowedLights,
	std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items)
{
	GBufferTextureInfo gbuff(cam->GetGBuffer()->GetTextureSize());
	cam->GetGBuffer()->FillGBufferInfo(gbuff);

	for (auto it = shadowedLights->begin(); it != shadowedLights->end(); ++it)
	{
		GeometryWorldItem::GeometryLight::Light* shadowLight = (*it);
		
		if (shadowLight->GetCastShadows())
		{
			//Calculate shadow map
			initShadowStep(cam->GetGBuffer());
			calculateSingleLightShadowMap(shadowLight, items);
			finishShadowStep(cam->GetGBuffer());
		}

		//Apply light
		LightingPass::initStep();
		cam->GetGBuffer()->BindForLightPass();
		LightingPass::applySingleLight(cam, gbuff, shadowLight);
		LightingPass::finishStep();
	}

	cam->GetGBuffer()->UnbindBuffer();
}

void GeometryEngine::GeometryRenderStep::ShadowedLightingPass::calculateSingleLightShadowMap(GeometryWorldItem::GeometryLight::Light * light, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items)
{
	for (auto it = items->begin(); it != items->end(); ++it)
	{
		GeometryWorldItem::GeometryItem::GeometryItem* item = (*it);
		if(item->CastsShadows())light->CalculateShadowMap(item->GetArrayBuffer(), item->GetIndexBuffer(), item->GetModelMatrix(), item->GetVertexNumber(), item->GetIndexNumber());
	}
}

void GeometryEngine::GeometryRenderStep::ShadowedLightingPass::initShadowStep(GeometryBuffer::GBuffer* buf)
{
	buf->BindTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	buf->BindTmpTextureWrite();
	buf->ClearColorTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	buf->DetachDepthBuffer();
	glEnable(GL_DEPTH_TEST);

	if (mFrontFaceCulling)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	glClear(GL_DEPTH_BUFFER_BIT);
}

void GeometryEngine::GeometryRenderStep::ShadowedLightingPass::finishShadowStep(GeometryBuffer::GBuffer* buf)
{
	if (mFrontFaceCulling)
	{
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
	}

	buf->AttachDepthBuffer();
	buf->UnbindTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
}
