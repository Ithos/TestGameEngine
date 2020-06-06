#include "Items\GraphicItems\Camera.h"
#include "Items\GeometryItem.h"
#include "Items\Materials\Material.h"
#include "../GBuffer.h"
#include "../Items/Item Utils/Viewport.h"
#include <map>
#include "TransparentGeometryPass.h"

void GeometryEngine::GeometryRenderStep::TransparentGeometryPass::renderGeometry(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items)
{
	assert(cam->GetGBuffer() != nullptr && "GeometryPass --> No geometry buffer found");

	std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> sortedTransparentItems;

	cam->GetGBuffer()->StartFrame();
	cam->GetGBuffer()->BindForGeomPass(); // Bind GBuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear GBuffer

	QVector4D viewport = cam->GetViewportSize();
	if (viewport.z() > 0 && viewport.w())
	{
		cam->GetViewport()->CalculateProjectionMatrix();

		for (auto it = items->begin(); it != items->end(); ++it)
		{
			if (!(*it)->GetMaterialPtr()->IsTransparent())
			{
				if ((*it)->IsVisible() && checkRenderGroups(cam, (*it))) drawItem(cam, (*it));
			}
			else
			{
				float distance = (*it)->GetPosition().distanceToPoint(cam->GetPosition());
				// We don't want to miss items because several sit at the same distance from the camera so we add a little differentiator
				while (sortedTransparentItems[distance] != nullptr) { distance += 1E-5f; }
				sortedTransparentItems[distance] = (*it);
			}
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (auto it = sortedTransparentItems.rbegin(); it != sortedTransparentItems.rend(); ++it)
		{
			if( (*it).second->IsVisible() && checkRenderGroups(cam, (*it).second ) ) drawItem(cam, (*it).second );
		}

		glDisable(GL_BLEND);
	}
}
