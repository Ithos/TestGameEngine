#include <map>
#include "Items\GraphicItems\Camera.h"
#include "Items\GraphicItems\Light.h"
#include "Items\GeometryItem.h"
#include "../GBuffer.h"
#include "ScreenRenderPass.h"

GeometryEngine::GeometryRenderStep::ScreenRenderPass::ScreenRenderPass() : RenderStep()
{
	std::map<GeometryWorldItem::GeometryCamera::CameraTargets, QVector4D> tmpMap;
	tmpMap[GeometryWorldItem::GeometryCamera::CAM1] = QVector4D(0.0f, 0.0f, 1.0f, 1.0f);
	mViewportLocations = new ScreenViewportLocation( tmpMap );
}

GeometryEngine::GeometryRenderStep::ScreenRenderPass::ScreenRenderPass(int viewportNumber) : RenderStep()
{
	mViewportLocations = new ScreenViewportLocation(viewportNumber);
}

GeometryEngine::GeometryRenderStep::ScreenRenderPass::ScreenRenderPass(const std::map<GeometryWorldItem::GeometryCamera::CameraTargets, QVector4D>& locationMap) : RenderStep()
{
	mViewportLocations = new ScreenViewportLocation(locationMap);
}

GeometryEngine::GeometryRenderStep::ScreenRenderPass::ScreenRenderPass(const ScreenViewportLocation & viewportLocations) : mViewportLocations(nullptr), RenderStep()
{
	mViewportLocations = viewportLocations.Clone();
}

GeometryEngine::GeometryRenderStep::ScreenRenderPass::~ScreenRenderPass()
{
	delete mViewportLocations;
}

void GeometryEngine::GeometryRenderStep::ScreenRenderPass::Render(GeometryWorldItem::GeometryCamera::Camera * cam, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems,
	std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* lights)
{
	assert(cam != nullptr && "ScreenRenderPass -> Camera not found");

	renderToScreen(cam);
}

void GeometryEngine::GeometryRenderStep::ScreenRenderPass::renderToScreen(GeometryWorldItem::GeometryCamera::Camera * cam)
{
	assert(cam->GetGBuffer() != nullptr && "ScreenRenderPass --> Geometry buffer not found");

	if ( mViewportLocations->ContainsViewport(cam->GetRenderTarget()) )
	{
		const QVector4D& loc = mViewportLocations->GetViewportSize(cam->GetRenderTarget());
		cam->GetGBuffer()->BindForFinalPass();
		QVector2D texSize = cam->GetGBuffer()->GetTextureSize();
		QVector4D viewport = cam->GetViewportSize();
		QVector4D& targetQuad = ScreenViewportLocation::GetQuadVertexPositions(viewport, loc);
		glBlitFramebuffer(
			0, 0, texSize.x(), texSize.y(),
			targetQuad.x(), targetQuad.y(), targetQuad.z(), targetQuad.w(),
			GL_COLOR_BUFFER_BIT, GL_LINEAR
		);
	}
}
