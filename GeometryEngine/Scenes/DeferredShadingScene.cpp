#include "DeferredShadingScene.h"

GeometryEngine::DeferredShadingScene::DeferredShadingScene(SceneManager * manager, GLdouble fovy, GLdouble zNear, GLdouble zFar, QVector4D clearColor) : 
	GeometryScene(manager, fovy, zNear, zFar, clearColor)
{
	mpInitialSetup = new InitialSetup();

	mRenderSteps.push_back( new GeometryEngine::GeometryPass() );
	mRenderSteps.push_back( new GeometryEngine::LightingPass() );
	mRenderSteps.push_back( new GeometryEngine::FinalPass() );
}
