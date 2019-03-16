#include "FinalPass.h"

void GeometryEngine::GeometryRenderStep::FinalPass::Render(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items, 
	std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* lights)
{
	assert(cam != nullptr && "FinalPass -> Camera not found");

	renderToScreen(cam);
}

void GeometryEngine::GeometryRenderStep::FinalPass::renderToScreen(GeometryWorldItem::GeometryCamera::Camera * cam)
{
	assert(cam->GetGBuffer() != nullptr && "FinalPass --> Geometry buffer not found");

	cam->GetGBuffer()->BindForFinalPass();
	QVector2D texSize = cam->GetGBuffer()->GetTextureSize();
	QVector4D viewport = cam->GetViewportSize();
	glBlitFramebuffer(
		0, 0, texSize.x(), texSize.y(),
		viewport.x(), viewport.y(), viewport.z(), viewport.w(),
		GL_COLOR_BUFFER_BIT, GL_LINEAR
	);
}
