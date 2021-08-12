#include "../SceneManager.h"
#include "../Render Utils/RenderSteps/TransparentGeometryPass.h"
#include "../Render Utils/RenderSteps/TransparentShadowedLightingPass.h"
#include "../Render Utils//RenderSteps/EmissiveLightingPass.h"
#include "../Render Utils/RenderSteps/ScreenRenderPass.h"
#include "../Render Utils/RenderSteps/PostProcessPass.h"
#include "../Render Utils/GlSetups/InitialSetup.h"

#include "../Items/Materials/ColorMaterial.h"
#include "../Items/Geometries/Quad.h"
#include "../Items/GraphicItems/Lights/EmissiveLighting.h"

#include "MultiViewportScene.h"

GeometryEngine::GeometryScene::MultiViewportScene::MultiViewportScene(SceneManager * manager, int viewportNumber, GLdouble fovy, GLdouble zNear, GLdouble zFar, QVector4D clearColor):
	GeometryScene(manager, fovy, zNear, zFar, clearColor)
{
	mpInitialSetup = new GeometryGlSetup::InitialSetup();

	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::TransparentGeometryPass());
	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass());

	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::EmissiveLightingPass(GeometryEngine::GeometryWorldItem::GeometryLight::EmissiveLighting(
		&GeometryEngine::GeometryWorldItem::GeometryItem::Quad(GeometryEngine::GeometryMaterial::ColorMaterial(), 3.0f, 3.0f))));

	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::PostProcessPass());
	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::ScreenRenderPass(viewportNumber));
}

GeometryEngine::GeometryScene::MultiViewportScene::MultiViewportScene(SceneManager * manager, const std::map<GeometryWorldItem::GeometryCamera::CameraTargets, QVector4D>& locationMap, GLdouble fovy, GLdouble zNear, GLdouble zFar, QVector4D clearColor) : 
	GeometryScene(manager, fovy, zNear, zFar, clearColor)
{
	mpInitialSetup = new GeometryGlSetup::InitialSetup();

	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::TransparentGeometryPass());
	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass());

	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::EmissiveLightingPass(GeometryEngine::GeometryWorldItem::GeometryLight::EmissiveLighting(
		&GeometryEngine::GeometryWorldItem::GeometryItem::Quad(GeometryEngine::GeometryMaterial::ColorMaterial(), 3.0f, 3.0f))));

	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::PostProcessPass());
	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::ScreenRenderPass(locationMap));
}
