#include "Items\GraphicItems\Camera.h"
#include "Items\GraphicItems\Light.h"
#include "Items\GeometryItem.h"
#include "RenderStep.h"

GeometryEngine::GeometryRenderStep::RenderStep::RenderStep()
{
	initializeOpenGLFunctions();
}
