#include "../Items/CommonItemParameters.h"
#include "Items\GraphicItems\Camera.h"
#include "Items\GraphicItems\Light.h"
#include "Items\Materials\Material.h"
#include "Items\GeometryItem.h"
#include "Items\GraphicItems\LightUtils\LightComponentManager.h"
#include "Items\GraphicItems\Lights\ShadowCastingLights\ShadowMapLight.h"
#include"../GBuffer.h"
#include "../ShadingBuffer.h"
#include "../RenderBuffersData.h"
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
	GeometryEngine::GeometryBuffer::GBuffer* geomBuffer = cam->GetGBuffer();
	GeometryEngine::GeometryBuffer::ShadingBuffer* trBuffer = cam->GetRenderBufferData()->GetShadingBuffer();

	GBufferTextureInfo gbuff(geomBuffer->GetTextureSize());
	cam->GetGBuffer()->FillGBufferInfo(gbuff);

	ShadingBufferTextureInfo tbuff(trBuffer->GetTextureSize());
	trBuffer->FillShadingBufferInfo(tbuff);

	trBuffer->StartFrame();

	BuffersInfo buff;
	buff.GeometryBufferInfo = &gbuff;
	buff.ShadingBufferInfo = &tbuff;

	for (auto it = shadowedLights->begin(); it != shadowedLights->end(); ++it)
	{
		GeometryWorldItem::GeometryLight::Light* shadowLight = (*it);
		
		if ( shadowLight->GetLightFunctionalities()->ContainsLightShadingComponent(LightUtils::DEFAULT_SHADOWMAP) )
		{
			//Calculate shadow map
			initShadowStep(trBuffer);
			calculateSingleLightShadowMap(shadowLight, items);
			finishShadowStep(trBuffer);
		}

		//Apply light
		LightingPass::initStep();
		trBuffer->BindShadowMapTextureRead();
		trBuffer->BindTexture(GeometryEngine::GeometryBuffer::ShadingBuffer::SHADINGBUFFER_TEXTURE_TYPE_SHADOW_MAP);
		cam->GetGBuffer()->BindForLightPass();
		
		LightingPass::applySingleLight(cam, buff, shadowLight);
		LightingPass::finishStep();
	}

	cam->GetGBuffer()->UnbindBuffer();
}

void GeometryEngine::GeometryRenderStep::ShadowedLightingPass::calculateSingleLightShadowMap(GeometryWorldItem::GeometryLight::Light * light, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items)
{
	for (auto it = items->begin(); it != items->end(); ++it)
	{
		GeometryWorldItem::GeometryItem::GeometryItem* item = (*it);
		if (item->CastsShadows())calculateItemShadowMap(item, light);
	}
}

void GeometryEngine::GeometryRenderStep::ShadowedLightingPass::initShadowStep(GeometryBuffer::ShadingBuffer* buf)
{
	buf->BindShadowMapTextureWrite();
	buf->ClearColorTexture(GeometryEngine::GeometryBuffer::ShadingBuffer::SHADINGBUFFER_TEXTURE_TYPE_SHADOW_MAP);
	buf->BindTexture(GeometryEngine::GeometryBuffer::ShadingBuffer::SHADINGBUFFER_TEXTURE_TYPE_SHADOW_MAP);
	
	
	glEnable(GL_DEPTH_TEST);
	if (mFrontFaceCulling)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	glClear(GL_DEPTH_BUFFER_BIT);
}

void GeometryEngine::GeometryRenderStep::ShadowedLightingPass::finishShadowStep(GeometryBuffer::ShadingBuffer* buf)
{
	if (mFrontFaceCulling)
	{
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
	}

	buf->UnbindTexture(GeometryEngine::GeometryBuffer::ShadingBuffer::SHADINGBUFFER_TEXTURE_TYPE_SHADOW_MAP);
}

void GeometryEngine::GeometryRenderStep::ShadowedLightingPass::calculateItemShadowMap(GeometryWorldItem::GeometryItem::GeometryItem * item, GeometryWorldItem::GeometryLight::Light* light)
{
	const GeometryEngine::LightingTransformationData* transf = light->GetLightTransformationMatrices(GeometryEngine::GeometryWorldItem::GeometryLight::LightTransformationMatrices::LIGHTSPACE_TRANSFORMATION_MATRICES);
	if (transf != nullptr && item->GetMaterialPtr()->GetCustomShaders() != nullptr && item->GetMaterialPtr()->GetCustomShaders()->ContainsStep(GeometryEngine::CustomShading::CUSTOM_SHADOWMAP))
	{
		item->GetMaterialPtr()->GetCustomShaders()->RenderStep(GeometryEngine::CustomShading::CUSTOM_SHADOWMAP, item->GetArrayBuffer(),
			item->GetIndexBuffer(), transf->ProjectionMatrix * transf->ViewMatrix * item->GetModelMatrix(), item->GetVertexNumber(), item->GetIndexNumber());
	}
	else
	{
		LightUtils::LightComponentManager* lightFuntionManager = light->GetLightFunctionalities();
		assert(lightFuntionManager != nullptr && lightFuntionManager->ContainsLightShadingComponent(LightUtils::DEFAULT_SHADOWMAP) && "Default shadow map not found");
		{
			lightFuntionManager->ApplyLightShading(LightUtils::DEFAULT_SHADOWMAP, item->GetArrayBuffer(), item->GetIndexBuffer(), 
				QMatrix4x4(), QMatrix4x4(), item->GetModelMatrix(), item->GetVertexNumber(), item->GetIndexNumber());
		}

	}
}
