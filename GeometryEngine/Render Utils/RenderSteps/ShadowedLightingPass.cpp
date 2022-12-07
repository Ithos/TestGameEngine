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

void GeometryEngine::GeometryRenderStep::ShadowedLightingPass::Render(GeometryWorldItem::GeometryCamera::Camera * cam, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems, std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* lights)
{
	assert(cam != nullptr && "ShadowedLightningPass --> No camera found");
	assert(lights != nullptr && "ShadowedLightningPass --> No lights list found");
	assert(orderedItems != nullptr && "ShadowedLightningPass --> No itmes list found");

	//Apply shadowed lights
	CalculateShadowMap(cam, lights, orderedItems);
}

void GeometryEngine::GeometryRenderStep::ShadowedLightingPass::CalculateShadowMap(GeometryWorldItem::GeometryCamera::Camera * cam, 
	std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* shadowedLights,
	std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems)
{
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

	for (auto it = shadowedLights->begin(); it != shadowedLights->end(); ++it)
	{
		GeometryWorldItem::GeometryLight::Light* shadowLight = (*it);
		
		if ( shadowLight->GetLightFunctionalities()->ContainsLightShadingComponent(LightUtils::DEFAULT_SHADOWMAP) )
		{
			//Calculate shadow map
			initShadowStep(trBuffer);
			calculateSingleLightShadowMap(shadowLight, orderedItems);
			finishShadowStep(trBuffer);
		}

		//Apply light
		LightingPass::initStep();
		bindRenderTextures(geomBuffer, trBuffer);
		
		LightingPass::applySingleLight(cam, buff, shadowLight);
		LightingPass::finishStep();
	}

	geomBuffer->UnbindBuffer();
}

void GeometryEngine::GeometryRenderStep::ShadowedLightingPass::calculateSingleLightShadowMap(GeometryWorldItem::GeometryLight::Light * light, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems)
{
	for (auto it = orderedItems->rbegin(); it != orderedItems->rend(); ++it)
	{
		GeometryWorldItem::GeometryItem::GeometryItem* item = (*it).second;
		if (item->CastsShadows())calculateItemShadowMap(item, light);
	}
}

void GeometryEngine::GeometryRenderStep::ShadowedLightingPass::initShadowStep(GeometryBuffer::ShadingBuffer* buf)
{
	buf->ClearColorTexture(GeometryEngine::GeometryBuffer::ShadingBuffer::SHADINGBUFFER_TEXTURE_TYPE_SHADOW_MAP);
	buf->BindShadowMapTextureWrite();
	//buf->BindTexture(GeometryEngine::GeometryBuffer::ShadingBuffer::SHADINGBUFFER_TEXTURE_TYPE_SHADOW_MAP);
	
	
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

void GeometryEngine::GeometryRenderStep::ShadowedLightingPass::bindRenderTextures(GeometryBuffer::GBuffer * geomBuff, GeometryBuffer::ShadingBuffer * shadingBuff)
{
	shadingBuff->BindShadowMapTextureRead();
	shadingBuff->BindTranslucentDepthMapRead();
	shadingBuff->BindColorMapsRead();
	geomBuff->BindForLightPass();
}

void GeometryEngine::GeometryRenderStep::ShadowedLightingPass::calculateItemShadowMap(GeometryWorldItem::GeometryItem::GeometryItem * item, GeometryWorldItem::GeometryLight::Light* light)
{
	const GeometryEngine::LightingTransformationData* transf = 
		light->GetLightTransformationMatrices(GeometryEngine::GeometryWorldItem::GeometryLight::LightTransformationMatrices::LIGHTSPACE_TRANSFORMATION_MATRICES);

	CustomShading::MultiShadingInterface* sdInterf = item->GetMaterialPtr()->GetShadingInterface();
	CustomShading::CustomShadingInterface* customSd = (sdInterf != nullptr && sdInterf->GetShadingInterface() != nullptr) ? sdInterf->GetShadingInterface() : nullptr;

	if (transf != nullptr && customSd != nullptr && customSd->ContainsStep(GeometryEngine::CustomShading::CUSTOM_SHADOWMAP))
	{
		customSd->RenderStep(GeometryEngine::CustomShading::CUSTOM_SHADOWMAP, item->GetArrayBuffer(),
			item->GetIndexBuffer(), transf->ProjectionMatrix * transf->ViewMatrix * item->GetModelMatrix(), item->GetVertexNumber(), item->GetIndexNumber());
	}
	else
	{
		LightUtils::LightComponentManager* lightFunctionManager = light->GetLightFunctionalities();
		assert(lightFunctionManager != nullptr && lightFunctionManager->ContainsLightShadingComponent(LightUtils::DEFAULT_SHADOWMAP) && "Default shadow map not found");
		{
			lightFunctionManager->ApplyLightShading(LightUtils::DEFAULT_SHADOWMAP, item->GetArrayBuffer(), item->GetIndexBuffer(),
				QMatrix4x4(), QMatrix4x4(), item->GetModelMatrix(), item->GetVertexNumber(), item->GetIndexNumber());
		}

	}
}
