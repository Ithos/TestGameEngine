#include "GeometryPass.h"

void GeometryEngine::GeometryRenderStep::GeometryPass::Render(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items, 
	std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* lights)
{
	assert(cam != nullptr && "GeometryPass --> No camera found");
	assert(items != nullptr && "GeometryPass --> No items list found");

	initStep();
	renderGeometry(cam, items);

}

void GeometryEngine::GeometryRenderStep::GeometryPass::initStep()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}
	
	

void GeometryEngine::GeometryRenderStep::GeometryPass::renderGeometry(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items)
{
	assert(cam->GetGBuffer() != nullptr && "GeometryPass --> No geometry buffer found");

	cam->GetGBuffer()->StartFrame();
	cam->GetGBuffer()->BindForGeomPass(); // Bind GBuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear GBuffer

	QVector4D viewport = cam->GetViewportSize();
	if (viewport.z() > 0 && viewport.w())
	{
		cam->CalculateProjectionMatrix();

		for (auto it = items->begin(); it != items->end(); ++it)
		{
			drawItem(cam, (*it));
		}
	}
}

void GeometryEngine::GeometryRenderStep::GeometryPass::drawItem(GeometryWorldItem::GeometryCamera::Camera * cam, GeometryWorldItem::GeometryItem::GeometryItem * item)
{
	assert(item != nullptr && "GeometryPass --> item null");
	item->DrawItem(cam->GetProjectionMatrix(), cam->GetViewMatrix());
}
