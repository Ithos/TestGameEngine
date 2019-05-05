#include "PostProcessScene.h"

GeometryEngine::GeometryScene::PostProcessScene::PostProcessScene(SceneManager * manager, GLdouble fovy, GLdouble zNear, GLdouble zFar, QVector4D clearColor) :
	GeometryScene(manager, fovy, zNear, zFar, clearColor)
{
	mpInitialSetup = new GeometryGlSetup::InitialSetup();

	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::GeometryPass());
	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::LightingPass());
	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::SinglePostProcessPass());
	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::DoublePostProcessPass());
	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::FinalPass());
}
