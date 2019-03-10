#include "FinalPass.h"

void GeometryEngine::FinalPass::Render(Camera * cam, std::unordered_set<GeometryItem*>* items, std::unordered_set<Light*>* lights)
{
	assert(cam == nullptr && "FinalPass -> Camera not found");
	assert(items != nullptr && "FinalPass -> Items not needed");
	assert(lights != nullptr && "FinalPass -> lights not needed");

	renderToScreen(cam);
}

void GeometryEngine::FinalPass::renderToScreen(Camera * cam)
{
	assert(cam->GetGBuffer() == nullptr && "FinalPass --> Geometry buffer not found");

	cam->GetGBuffer()->BindForFinalPass();
	QVector2D texSize = cam->GetGBuffer()->GetTextureSize();
	QVector4D viewport = cam->GetViewportSize();
	glBlitFramebuffer(
		0, 0, texSize.x(), texSize.y(),
		viewport.x(), viewport.y(), viewport.z(), viewport.w(),
		GL_COLOR_BUFFER_BIT, GL_LINEAR
	);
}
