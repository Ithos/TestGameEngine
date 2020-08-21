#include "../Items/CommonItemParameters.h"
#include "Items\GraphicItems\Camera.h"
#include "Items\GraphicItems\Light.h"
#include "Items\Materials\Material.h"
#include "Items\GeometryItem.h"
#include "Items\GraphicItems\LightUtils\LightFunctionalities.h"
#include "Items\GraphicItems\Lights\ShadowCastingLights\ShadowMapLight.h"
#include"../GBuffer.h"
#include "../TranslucentBuffer.h"
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
	//GeometryEngine::GeometryBuffer::TranslucentBuffer* trBuffer = cam->GetRenderBufferData()->GetTranslucentBuffer();

	GBufferTextureInfo gbuff(geomBuffer->GetTextureSize());
	cam->GetGBuffer()->FillGBufferInfo(gbuff);

	//TBufferTextureInfo tbuff(trBuffer->GetTextureSize());
	//trBuffer->FillGBufferInfo(tbuff);

	//trBuffer->StartFrame();

	BuffersInfo buff;
	buff.GeometryBufferInfo = &gbuff;
	//buff.TranslucentBufferInfo = &tbuff;

	for (auto it = shadowedLights->begin(); it != shadowedLights->end(); ++it)
	{
		GeometryWorldItem::GeometryLight::Light* shadowLight = (*it);
		
		if ( shadowLight->GetLightFunctionalities()->ContainsFunction(LightUtils::DEFAULT_SHADOWMAP) )
		{
			//Calculate shadow map
			initShadowStep(geomBuffer);
			calculateSingleLightShadowMap(shadowLight, items);
			finishShadowStep(geomBuffer);
		}

		//Apply light
		LightingPass::initStep();
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
	
	/*
	buf->BindTexture(GeometryEngine::GeometryBuffer::TranslucentBuffer::TBUFFER_TEXTURE_TYPE_SHADOW_MAP);
	buf->BindShadowMapTextureWrite();
	glEnable(GL_DEPTH_TEST);
	if (mFrontFaceCulling)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	glClear(GL_DEPTH_BUFFER_BIT);
	*/
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
	

	//buf->UnbindTexture(GeometryEngine::GeometryBuffer::TranslucentBuffer::TBUFFER_TEXTURE_TYPE_SHADOW_MAP);
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
		LightUtils::LightFunctionalities* lightFuntionManager = light->GetLightFunctionalities();
		assert(lightFuntionManager != nullptr && lightFuntionManager->ContainsFunction(LightUtils::DEFAULT_SHADOWMAP) && "Default shadow map not found");
		{
			lightFuntionManager->ApplyFunction(LightUtils::DEFAULT_SHADOWMAP, item->GetArrayBuffer(), item->GetIndexBuffer(), 
				QMatrix4x4(), QMatrix4x4(), item->GetModelMatrix(), item->GetVertexNumber(), item->GetIndexNumber());
		}

		//light->CalculateShadowMap(item->GetArrayBuffer(), item->GetIndexBuffer(), item->GetModelMatrix(), item->GetVertexNumber(), item->GetIndexNumber());
	}
}
