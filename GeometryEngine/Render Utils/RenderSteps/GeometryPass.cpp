#include "Items\GraphicItems\Camera.h"
#include "Items\GraphicItems\Light.h"
#include "Items\GeometryItem.h"
#include "../GBuffer.h"
#include "../Items/Item Utils/Viewport.h"
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

	std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> orderedItems;
	orderGeometry(cam, items, orderedItems);

	QVector4D viewport = cam->GetViewportSize();
	if (viewport.z() > 0 && viewport.w())
	{
		cam->GetViewport()->CalculateProjectionMatrix();

		for (auto it = orderedItems.rbegin(); it != orderedItems.rend(); ++it)
		{
			GeometryWorldItem::GeometryItem::GeometryItem* item = (*it).second;
			if(item->IsVisible() && checkRenderGroups(cam, item)) drawItem(cam, item);
		}
	}
}

void GeometryEngine::GeometryRenderStep::GeometryPass::orderGeometry(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*>& orderedItems)
{
	for (auto it = items->begin(); it != items->end(); ++it)
	{
		float distance = (*it)->GetPosition().distanceToPoint(cam->GetPosition());
		// We don't want to miss items because several sit at the same distance from the camera so we add a little differentiator
		while ( orderedItems[distance] != nullptr) { distance += 1E-5f; }
		orderedItems[distance] = (*it);
	}
}

void GeometryEngine::GeometryRenderStep::GeometryPass::drawItem(GeometryWorldItem::GeometryCamera::Camera * cam, GeometryWorldItem::GeometryItem::GeometryItem * item)
{
	assert(item != nullptr && "GeometryPass --> item null");
	item->DrawItem(cam->GetProjectionMatrix(), cam->GetViewMatrix());
}

bool GeometryEngine::GeometryRenderStep::GeometryPass::checkRenderGroups(GeometryWorldItem::GeometryCamera::Camera * cam, GeometryWorldItem::GeometryItem::GeometryItem * item)
{
	if (cam->GetRenderGroups().empty() && item->GetRenderGroups().empty())
		return true;

	for (auto itCam = cam->GetRenderGroups().begin(); itCam != cam->GetRenderGroups().end(); ++itCam)
	{
		if (item->GetRenderGroups().find((*itCam)) != item->GetRenderGroups().end())
			return true;
	}

	return false;
}
