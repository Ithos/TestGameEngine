#include "Items/WorldItem.h"
#include "Items\GeometryItem.h"
#include "Items/GraphicItems/Camera.h"
#include "Items\GraphicItems\Light.h"
#include "Render Utils\RenderStep.h"
#include "Render Utils\GlSetup.h"
#include "SceneManager.h"

#include "GeometryScene.h"

GeometryEngine::GeometryScene::GeometryScene::GeometryScene(SceneManager* manager, GLdouble fovy, GLdouble zNear, GLdouble zFar, QVector4D clearColor) : mClearColor(clearColor), mpInitialSetup(nullptr)
{
	mpParentManager = manager;
	mpParentManager->AddScene(this);
}

GeometryEngine::GeometryScene::GeometryScene::~GeometryScene()
{
	delete(mpInitialSetup);
	ClearRenderSteps();
}

void GeometryEngine::GeometryScene::GeometryScene::InitializeGL()
{
	assert(mpInitialSetup != nullptr && "GeometryScene --> no intial setup found");
	mpInitialSetup->SetColor(mClearColor);
	mpInitialSetup->StartSetup();
}

void GeometryEngine::GeometryScene::GeometryScene::SetInitialSetup(const GeometryGlSetup::GlSetup & setup)
{
	mpInitialSetup = setup.Clone();
}

void GeometryEngine::GeometryScene::GeometryScene::ResizeScene(int w, int h, int formerW, int formerH)
{
	float wRel = (float)w / (float)formerW;
	float hRel = (float)h / (float)formerH;

	for (auto iter = mCameras.begin(); iter != mCameras.end(); ++iter)
	{
		if ((*iter)->IsAutoResize())
		{
			const QVector4D& viewport = (*iter)->GetViewportSize();
			(*iter)->SetViewportSize(QVector4D(viewport.x() * wRel, viewport.y() * hRel, viewport.z() * wRel, viewport.w() * hRel));
		}
	}

	for (auto iter = mLights.begin(); iter != mLights.end(); ++iter)
	{
		(*iter)->ResizeElements(w, h);
	}
}

void GeometryEngine::GeometryScene::GeometryScene::Draw()
{
	for (auto iter = mCameras.begin(); iter != mCameras.end(); ++iter)
	{
		GeometryWorldItem::GeometryCamera::Camera* cam = (*iter);
		// If the camera has a custom render pipeline we go with that if it doesn't we use the scene pipeline
		renderCamera(cam, cam->GetCustomRenderSteps().size() > 0 ? cam->GetCustomRenderSteps() : mRenderSteps);
	}
}

bool GeometryEngine::GeometryScene::GeometryScene::AddItem(GeometryWorldItem::GeometryItem::GeometryItem* item)
{
	if (mItemList.find(item) != mItemList.end())
		return false;

	mItemList.insert(item);
	return true;
}

bool GeometryEngine::GeometryScene::GeometryScene::RemoveItem(GeometryWorldItem::GeometryItem::GeometryItem* item)
{
	if(mItemList.find(item) == mItemList.end())
		return false;

	mItemList.erase(item);
	return true;
}

bool GeometryEngine::GeometryScene::GeometryScene::AddCamera(GeometryWorldItem::GeometryCamera::Camera * item)
{
	if (mCameras.find(item) != mCameras.end())
		return false;

	mCameras.insert(item);
	return true;
}

bool GeometryEngine::GeometryScene::GeometryScene::RemoveCamera(GeometryWorldItem::GeometryCamera::Camera * item)
{
	if (mCameras.find(item) == mCameras.end())
		return false;

	mCameras.erase(item);
	return true;
}

bool GeometryEngine::GeometryScene::GeometryScene::AddLight(GeometryWorldItem::GeometryLight::Light * item)
{
	if(mLights.find(item) != mLights.end())
		return false;

	mLights.insert(item);
	return true;
}

bool GeometryEngine::GeometryScene::GeometryScene::RemoveLight(GeometryWorldItem::GeometryLight::Light * item)
{
	if(mLights.find(item) == mLights.end())
		return false;

	mLights.insert(item);
	return true;
}

bool GeometryEngine::GeometryScene::GeometryScene::AddRenderStep(const GeometryRenderStep::RenderStep& step)
{
	mRenderSteps.push_back( step.Clone() );
	return true;
}

bool GeometryEngine::GeometryScene::GeometryScene::RemoveRenderStep(int pos)
{
	if( mRenderSteps.size() >= pos)
		return false;

	std::list<GeometryRenderStep::RenderStep* >::iterator it = mRenderSteps.begin();
	std::advance(it, pos);
	mRenderSteps.remove((*it));
	return true;
}

bool GeometryEngine::GeometryScene::GeometryScene::InsertRenderStep(const GeometryRenderStep::RenderStep& step, unsigned int pos)
{
	if (pos >= mRenderSteps.size())
		return false;

	std::list<GeometryRenderStep::RenderStep* >::iterator it = mRenderSteps.begin();
	std::advance(it, pos);
	mRenderSteps.emplace(it, step.Clone() );
	return true;
}

void GeometryEngine::GeometryScene::GeometryScene::ClearRenderSteps()
{
	for (auto iter = mRenderSteps.begin(); iter != mRenderSteps.end(); ++iter)
	{
		GeometryRenderStep::RenderStep* step = (*iter);
		delete(step);
	}

	mRenderSteps.clear();
}

void GeometryEngine::GeometryScene::GeometryScene::renderCamera(GeometryWorldItem::GeometryCamera::Camera * cam, const std::list<GeometryRenderStep::RenderStep*>& renderSteps)
{
	for (auto it = renderSteps.begin(); it != renderSteps.end(); ++it)
	{
		GeometryRenderStep::RenderStep* current = (*it);
		current->Render(cam, &mItemList, &mLights);
	}
}


