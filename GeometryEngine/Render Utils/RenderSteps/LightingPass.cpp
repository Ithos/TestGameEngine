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
		cam->CalculateProjectionMatrix();
		applyLight(cam, lights);
	}
}

void GeometryEngine::GeometryRenderStep::LightingPass::applyLight(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights)
{
	GBufferTextureInfo gbuff(
		(unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_AMBIENT,
		(unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_DIFFUSE,
		(unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_REFLECTIVE,
		(unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_EMMISSIVE,
		(unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_POSITION,
		(unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_NORMAL,
		(unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_TEXCOORD,
		cam->GetGBuffer()->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_AMBIENT),
		cam->GetGBuffer()->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_DIFFUSE),
		cam->GetGBuffer()->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_REFLECTIVE),
		cam->GetGBuffer()->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_EMMISSIVE),
		cam->GetGBuffer()->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_POSITION),
		cam->GetGBuffer()->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_NORMAL),
		cam->GetGBuffer()->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_TEXCOORD),
		cam->GetGBuffer()->GetTextureSize());

	cam->GetGBuffer()->BindForLightPass();

	for (auto iter = lights->begin(); iter != lights->end(); ++iter)
	{
		GeometryWorldItem::GeometryLight::Light* l = (*iter);

		assert(l->GetBoundingGeometry() != nullptr && "LightingPass --> Light without bounding geometry");

		if (l->GetStencilTest())
		{
			prepareStencilPass(cam);
			stencilPass(l, cam);
			cam->GetGBuffer()->BindForLightPass();
			setStencilLight();
		}

		prepareLightPass();

		l->LightFromBoundignGeometry(cam->GetProjectionMatrix(), cam->GetViewMatrix(), gbuff, cam->GetPosition());

		if (l->GetStencilTest())
		{
			finishStencilPass();
		}

		finishLightPass();

	}
}

void GeometryEngine::GeometryRenderStep::LightingPass::initStep()
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
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
