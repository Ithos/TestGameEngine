#include "../Items/CommonItemParameters.h"
#include "Items\GraphicItems\Camera.h"
#include "Items\GraphicItems\Light.h"
#include "Items\GeometryItem.h"
#include "../GBuffer.h"
#include "../Items/Item Utils/Viewport.h"
#include "LightingPass.h"

void GeometryEngine::GeometryRenderStep::LightingPass::Render(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items, 
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

	for (auto iter = lights->begin(); iter != lights->end(); ++iter)
	{
		GeometryWorldItem::GeometryLight::Light* l = (*iter);

		assert(l->GetBoundingGeometry() != nullptr && "LightingPass --> Light without bounding geometry");

		applySingleLight(cam, gbuff, l);

	}

	cam->GetGBuffer()->UnbindBuffer();
}

void GeometryEngine::GeometryRenderStep::LightingPass::initStep()
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
}

void GeometryEngine::GeometryRenderStep::LightingPass::applySingleLight(GeometryWorldItem::GeometryCamera::Camera * cam, const GBufferTextureInfo & gBuf, GeometryWorldItem::GeometryLight::Light * light)
{
	if (light->GetStencilTest())
	{
		prepareStencilPass(cam);
		stencilPass(light, cam);
		cam->GetGBuffer()->BindForLightPass();
		setStencilLight();
	}

	prepareLightPass();

	light->LightFromBoundignGeometry(cam->GetProjectionMatrix(), cam->GetViewMatrix(), gBuf, cam->GetPosition());

	if (light->GetStencilTest())
	{
		finishStencilPass();
	}

	finishLightPass();
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
	assert(light->GetStencilTest() && "LightingPass --> No stencil test found diring stencil pass");
	light->CalculateStencil(cam->GetProjectionMatrix(), cam->GetViewMatrix());
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
