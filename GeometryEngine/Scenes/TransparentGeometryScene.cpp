#include "../SceneManager.h"
#include "../Render Utils/RenderSteps/TransparentGeometryPass.h"
#include "../Render Utils/RenderSteps/TransparentShadowedLightingPass.h"
#include "../Render Utils//RenderSteps/EmissiveLightingPass.h"
#include "../Render Utils/RenderSteps/FinalPass.h"
#include "../Render Utils/RenderSteps/PostProcessPass.h"
#include "../Render Utils/GlSetups/InitialSetup.h"

#include "../Items/Materials/ColorMaterial.h"
#include "../Items/Geometries/Quad.h"
#include "../Items/GraphicItems/Lights/EmissiveLighting.h"

#include "TransparentGeometryScene.h"

GeometryEngine::GeometryScene::TransparentGeometryScene::TransparentGeometryScene(SceneManager * manager, GLdouble fovy, GLdouble zNear, GLdouble zFar, QVector4D clearColor) :
	GeometryScene(manager, fovy, zNear, zFar, clearColor)
{
	mpInitialSetup = new GeometryGlSetup::InitialSetup();

	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::TransparentGeometryPass());
	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::TransparentShadowedLightingPass());

	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::EmissiveLightingPass(GeometryEngine::GeometryWorldItem::GeometryLight::EmissiveLighting(
		&GeometryEngine::GeometryWorldItem::GeometryItem::Quad(GeometryEngine::GeometryMaterial::ColorMaterial(), 3.0f, 3.0f) )));

	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::PostProcessPass());
	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::FinalPass());
}
