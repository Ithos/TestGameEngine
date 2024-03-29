#include "../Items/CommonItemParameters.h"
#include "Items\GraphicItems\Camera.h"
#include "Items\GraphicItems\Light.h"
#include "Items\GeometryItem.h"
#include "../GBuffer.h"
#include "../Items/Item Utils/Viewport.h"
#include "Items\GraphicItems\LightUtils\LightComponentManager.h"
#include "LightingPass.h"

void GeometryEngine::GeometryRenderStep::LightingPass::Render(GeometryWorldItem::GeometryCamera::Camera * cam, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems,
	std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* lights)
{
	assert(cam != nullptr && "LightingPass --> No camera found");
	assert(lights != nullptr && "LightingPass --> No lights list found");

	initStep();
	renderLights(cam, lights);
	finishStep();
}

void GeometryEngine::GeometryRenderStep::LightingPass::renderLights(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* lights)
{
	assert(cam->GetGBuffer() != nullptr && "LightingPass --> No geometry buffer found");

	QVector4D viewport = cam->GetViewportSize();
	if (viewport.z() > 0 && viewport.w())
	{
		cam->GetViewport()->CalculateProjectionMatrix();
		applyLight(cam, lights);
	}
}

void GeometryEngine::GeometryRenderStep::LightingPass::applyLight(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights)
{
	GBufferTextureInfo gbuff(cam->GetGBuffer()->GetTextureSize());
	cam->GetGBuffer()->FillGBufferInfo(gbuff);
	cam->GetGBuffer()->BindForLightPass();

	BuffersInfo buff; 
	buff.GeometryBufferInfo = &gbuff;

	for (auto iter = lights->begin(); iter != lights->end(); ++iter)
	{
		GeometryWorldItem::GeometryLight::Light* l = (*iter);
		applySingleLight(cam, buff, l);

	}

	cam->GetGBuffer()->UnbindBuffer();
}

void GeometryEngine::GeometryRenderStep::LightingPass::initStep()
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
}

void GeometryEngine::GeometryRenderStep::LightingPass::applySingleLight(GeometryWorldItem::GeometryCamera::Camera * cam, const BuffersInfo & buff, GeometryWorldItem::GeometryLight::Light * light)
{
	bool stencilTest = light->GetLightFunctionalities()->ContainsLightShadingComponent(LightUtils::STENCIL_TESTING);

	if (stencilTest)
	{
		prepareStencilPass(cam);
		stencilPass(light, cam);
		cam->GetGBuffer()->BindForLightPass();
		setStencilLight();
	}

	prepareLightPass();

	calculateLighting(light, cam->GetProjectionMatrix(), cam->GetViewMatrix(), buff, cam->GetPosition());

	if (stencilTest)
	{
		finishStencilPass();
	}

	finishLightPass();
}

void GeometryEngine::GeometryRenderStep::LightingPass::calculateLighting(GeometryWorldItem::GeometryLight::Light* light, const QMatrix4x4 & projectionMatrix, const QMatrix4x4 & viewMatrix, const BuffersInfo & buffTexInfo, const QVector3D & viewPos)
{
	LightUtils::LightComponentManager* lightFuntionManager = light->GetLightFunctionalities();

	assert(lightFuntionManager != nullptr && lightFuntionManager->ContainsLightRenderComponent(LightUtils::LightRender::BOUNDING_GEOMETRY) && "Deferred lighting bounding geometry technique not found" );
	{
		lightFuntionManager->ApplyLightRender(LightUtils::LightRender::BOUNDING_GEOMETRY, projectionMatrix, viewMatrix, buffTexInfo, viewPos);
	}
}

void GeometryEngine::GeometryRenderStep::LightingPass::finishStep()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void GeometryEngine::GeometryRenderStep::LightingPass::prepareStencilPass(GeometryWorldItem::GeometryCamera::Camera * cam)
{
	cam->GetGBuffer()->BindForStencilPass();
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);

	glClear(GL_STENCIL_BUFFER_BIT);

	// We need the stencil test to be enabled but we want it
	// to succeed always. Only the depth test matters.
	glStencilFunc(GL_ALWAYS, 0, 0);

	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
}

void GeometryEngine::GeometryRenderStep::LightingPass::stencilPass(GeometryWorldItem::GeometryLight::Light * light, GeometryWorldItem::GeometryCamera::Camera * cam)
{
	LightUtils::LightComponentManager* lightFuntionManager = light->GetLightFunctionalities();
	assert(lightFuntionManager != nullptr && lightFuntionManager->ContainsLightShadingComponent(LightUtils::STENCIL_TESTING) && "Stencil testing light function not found");
	{
		lightFuntionManager->ApplyLightShading(LightUtils::STENCIL_TESTING, nullptr, nullptr, cam->GetProjectionMatrix(), cam->GetViewMatrix(), QMatrix4x4(), 0, 0);
	}
}

void GeometryEngine::GeometryRenderStep::LightingPass::setStencilLight()
{
	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDisable(GL_DEPTH_TEST);
}

void GeometryEngine::GeometryRenderStep::LightingPass::prepareLightPass()
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
}

void GeometryEngine::GeometryRenderStep::LightingPass::finishLightPass()
{
	glDisable(GL_BLEND);
}

void GeometryEngine::GeometryRenderStep::LightingPass::finishStencilPass()
{
	glCullFace(GL_BACK);
	glDisable(GL_STENCIL_TEST);
}
