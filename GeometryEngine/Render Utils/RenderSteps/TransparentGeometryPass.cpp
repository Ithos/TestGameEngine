#include "Items\GraphicItems\Camera.h"
#include "Items\GeometryItem.h"
#include "Items\Materials\Material.h"
#include "../GBuffer.h"
#include "../Items/Item Utils/Viewport.h"
#include <map>
#include "TransparentGeometryPass.h"

void GeometryEngine::GeometryRenderStep::TransparentGeometryPass::renderGeometry(GeometryWorldItem::GeometryCamera::Camera * cam, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems)
{
	assert(cam->GetGBuffer() != nullptr && "GeometryPass --> No geometry buffer found");

	cam->GetGBuffer()->StartFrame();
	cam->GetGBuffer()->BindForGeomPass(); // Bind GBuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear GBuffer

	std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> sortedTransparentItems;

	QVector4D viewport = cam->GetViewportSize();
	if (viewport.z() > 0 && viewport.w())
	{
		cam->GetViewport()->CalculateProjectionMatrix();

		for (auto it = orderedItems->rbegin(); it != orderedItems->rend(); ++it)
		{
			GeometryWorldItem::GeometryItem::GeometryItem* item = (*it).second;

			if (!item->GetMaterialPtr()->IsTransparent())
			{
				if (item->IsVisible() && checkRenderGroups(cam, item)) drawItem(cam, item);
			}
			else
			{
				sortedTransparentItems[(*it).first] = item;
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
