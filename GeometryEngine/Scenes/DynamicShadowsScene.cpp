#include "DynamicShadowsScene.h"

GeometryEngine::GeometryScene::DynamicShadowsScene::DynamicShadowsScene(SceneManager * manager, GLdouble fovy, GLdouble zNear, GLdouble zFar, QVector4D clearColor) :
	GeometryScene(manager, fovy, zNear, zFar, clearColor)
{
	mpInitialSetup = new GeometryGlSetup::InitialSetup();

	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::GeometryPass());
	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::ShadowedLightingPass());
	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::PostProcessPass());
	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::FinalPass());
}
