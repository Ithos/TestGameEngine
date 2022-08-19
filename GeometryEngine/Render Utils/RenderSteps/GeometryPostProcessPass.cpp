/*
#include "Items\GraphicItems\Camera.h"
#include "Items\GraphicItems\Light.h"
#include "Items\GeometryItem.h"
#include "../GBuffer.h"
#include "../Items/Item Utils/Viewport.h"
#include "../Items/CommonItemParameters.h"
#include "../Items/PostProcess/DoublePassPostProcess.h"
#include "../Items/CommonInerfaces/CustomPostProcessStepInterface.h"
#include "../Items/CommonInerfaces/MultiShadingInterface.h"
#include "GeometryPostProcessPass.h"

void GeometryEngine::GeometryRenderStep::GeometryPostProcessPass::Render(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items, std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* lights)
{
	assert(cam != nullptr && "GeometryPass --> No camera found");
	assert(items != nullptr && "GeometryPass --> No items list found");
	initStep();
	renderGeometry(cam, items);
	endStep();
}

void GeometryEngine::GeometryRenderStep::GeometryPostProcessPass::initStep()
{
	glDisable(GL_DEPTH);
}

void GeometryEngine::GeometryRenderStep::GeometryPostProcessPass::endStep()
{
	glEnable(GL_DEPTH);
}

void GeometryEngine::GeometryRenderStep::GeometryPostProcessPass::renderGeometry(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items)
{
	GeometryBuffer::GBuffer * buf = cam->GetGBuffer();
	assert(buf != nullptr && "renderGeometry --> Geometry buffer not found");
	GBufferTextureInfo gbuff(buf->GetTextureSize());
	buf->FillGBufferInfo(gbuff);
	buf->BindTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	buf->ClearColorTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	buf->BindFinalTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_NUM_TEXTURES, gbuff);

	QVector4D viewport = cam->GetViewportSize();
	if (viewport.z() > 0 && viewport.w())
	{
		cam->GetViewport()->CalculateProjectionMatrix();

		for (auto it = items->begin(); it != items->end(); ++it)
		{
			GeometryWorldItem::GeometryItem::GeometryItem* item = (*it);
			if (item->IsVisible() && checkRenderGroups(cam, item)) drawItem(cam, item);
		}
	}

	buf->UnbindFinalTexture();
	buf->UnbindTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
}

void GeometryEngine::GeometryRenderStep::GeometryPostProcessPass::drawItem(GeometryWorldItem::GeometryCamera::Camera * cam, GeometryWorldItem::GeometryItem::GeometryItem * item)
{

}
*/