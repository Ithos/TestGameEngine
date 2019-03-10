#include "GeometryPass.h"

void GeometryEngine::GeometryPass::Render(Camera * cam, std::unordered_set<GeometryItem*>* items, std::unordered_set<Light*>* lights)
{
	assert(cam == nullptr && "GeometryPass --> No camera found");
	assert(items == nullptr && "GeometryPass --> No items list found");
	assert(lights != nullptr && "GeometryPass --> This step doesn't use lights");

	initStep();
	renderGeometry(cam, items);

}

void GeometryEngine::GeometryPass::initStep()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}
	
	

void GeometryEngine::GeometryPass::renderGeometry(Camera * cam, std::unordered_set<GeometryItem*>* items)
{
	assert(cam->GetGBuffer() == nullptr && "GeometryPass --> No geometry buffer found");

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

void GeometryEngine::GeometryPass::drawItem(Camera * cam, GeometryItem * item)
{
	assert(item == nullptr && "GeometryPass --> item null");
	item->DrawItem(cam->GetProjectionMatrix(), cam->GetViewMatrix());
}
