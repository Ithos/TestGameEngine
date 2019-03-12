#include "GeometryScene.h"

GeometryEngine::GeometryScene::GeometryScene(SceneManager* manager, GLdouble fovy, GLdouble zNear, GLdouble zFar, QVector4D clearColor) : mClearColor(clearColor), mpInitialSetup(nullptr)
{
	mpParentManager = manager;
	mpParentManager->AddScene(this);
}

GeometryEngine::GeometryScene::~GeometryScene()
{
	delete(mpInitialSetup);

	for (auto iter = mRenderSteps.begin(); iter != mRenderSteps.end(); ++iter)
	{
		RenderStep* step = (*iter);
		delete(step);
	}

	mRenderSteps.clear();
}

void GeometryEngine::GeometryScene::InitializeGL()
{
	assert(mpInitialSetup != nullptr && "GeometryScene --> no intial setup found");
	mpInitialSetup->SetColor(mClearColor);
	mpInitialSetup->StartSetup();
}

void GeometryEngine::GeometryScene::ResizeScene(int w, int h, int formerW, int formerH)
{
	for (auto iter = mCameras.begin(); iter != mCameras.end(); ++iter)
	{
		if ((*iter)->IsAutoResize())
		{
			const QVector4D& viewport = (*iter)->GetViewportSize();
			float wRel = (float)w / (float)formerW;
			float hRel = (float)h / (float)formerH;

			(*iter)->SetViewport(QVector4D(viewport.x() * wRel, viewport.y() * hRel, viewport.z() * wRel, viewport.w() * hRel));
		}
	}
}

void GeometryEngine::GeometryScene::Draw()
{
	for (auto iter = mCameras.begin(); iter != mCameras.end(); ++iter)
	{
		Camera* cam = (*iter);
		for (auto it = mRenderSteps.begin(); it != mRenderSteps.end(); ++it)
		{
			RenderStep* current = (*it);
			current->Render(cam, &mItemList, &mLights);
		}
	}
}

bool GeometryEngine::GeometryScene::AddItem(GeometryItem* item)
{
	if (mItemList.find(item) != mItemList.end())
		return false;

	mItemList.insert(item);
	return true;
}

bool GeometryEngine::GeometryScene::RemoveItem(GeometryItem* item)
{
	if(mItemList.find(item) == mItemList.end())
		return false;

	mItemList.erase(item);
	return true;
}

bool GeometryEngine::GeometryScene::AddCamera(Camera * item)
{
	if (mCameras.find(item) != mCameras.end())
		return false;

	mCameras.insert(item);
	return true;
}

bool GeometryEngine::GeometryScene::RemoveCamera(Camera * item)
{
	if (mCameras.find(item) == mCameras.end())
		return false;

	mCameras.erase(item);
	return true;
}

bool GeometryEngine::GeometryScene::AddLight(Light * item)
{
	if(mLights.find(item) != mLights.end())
		return false;

	mLights.insert(item);
	return true;
}

bool GeometryEngine::GeometryScene::RemoveLight(Light * item)
{
	if(mLights.find(item) == mLights.end())
		return false;

	mLights.insert(item);
	return true;
}

bool GeometryEngine::GeometryScene::AddRenderStep(const RenderStep& step)
{
	mRenderSteps.push_back( step.Clone() );
	return true;
}

bool GeometryEngine::GeometryScene::RemoveRenderStep(int pos)
{


	if( mRenderSteps.size() >= pos)
		return false;

	std::list<RenderStep* >::iterator it = mRenderSteps.begin();
	std::advance(it, pos);
	mRenderSteps.remove((*it));
	return true;
}

bool GeometryEngine::GeometryScene::InsertRenderStep(const RenderStep& step, unsigned int pos)
{
	if (pos >= mRenderSteps.size())
		return false;

	std::list<RenderStep* >::iterator it = mRenderSteps.begin();
	std::advance(it, pos);
	mRenderSteps.emplace(it, step.Clone() );
	return true;
}


