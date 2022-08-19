#include "Items\GraphicItems\Camera.h"
#include "Items\GraphicItems\Light.h"
#include "Items\GeometryItem.h"
#include "RenderStep.h"

GeometryEngine::GeometryRenderStep::RenderStep::RenderStep()
{
	initializeOpenGLFunctions();
}

bool GeometryEngine::GeometryRenderStep::RenderStep::checkRenderGroups(GeometryWorldItem::GeometryCamera::Camera * cam, GeometryWorldItem::GeometryItem::GeometryItem * item)
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
