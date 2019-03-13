#include "Camera.h"

#include "Render Utils\RenderStep.h"

GeometryEngine::Camera::Camera(const QVector4D& viewportSize, bool autoResize, GLdouble zNear, GLdouble zFar, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent) :
	WorldItem(pos, rot, scale, parent), mViewportSize(viewportSize), mZNear(zNear), mZFar(zFar), mAutoResize(autoResize), mpGBuffer(nullptr)
{
	mProjection.setToIdentity();
	CalculateModelMatrix();
}

GeometryEngine::Camera::~Camera()
{
	if (mpGBuffer != nullptr)
	{
		delete mpGBuffer;
		mpGBuffer = nullptr;
	}

	ClearCustomRenderSteps();
}

void GeometryEngine::Camera::UpdateModelMatrix(bool updateChildren)
{
	// The transformations on the camera are really the opposite transformations on the world
	mModelMatrix.setToIdentity();
	mModelMatrix.translate(-GetPosition());
	mModelMatrix.rotate(-mRotation);
	mModelMatrix.scale(mScale);

	if (updateChildren)
	{
		for (auto iter = mpChildren.begin(); iter != mpChildren.end(); ++iter)
		{
			(*iter)->UpdateModelMatrix(updateChildren);
		}
	}
}

void GeometryEngine::Camera::ResetCameraBeforeCalculation()
{
	mProjection.setToIdentity();
	glViewport(mViewportSize.x(), mViewportSize.y(), mViewportSize.z(), mViewportSize.w());
}

bool GeometryEngine::Camera::AddCustomRenderStep(const RenderStep & step)
{
	mCustomRenderSteps.push_back(step.Clone());
	return true;
}

bool GeometryEngine::Camera::RemoveCustomRenderStep(int pos)
{
	if (mCustomRenderSteps.size() >= pos)
		return false;

	std::list<RenderStep* >::iterator it = mCustomRenderSteps.begin();
	std::advance(it, pos);
	mCustomRenderSteps.remove((*it));
	return true;
}

bool GeometryEngine::Camera::InsertCustomRenderStep(const RenderStep & step, unsigned int pos)
{
	if (pos >= mCustomRenderSteps.size())
		return false;

	std::list<RenderStep* >::iterator it = mCustomRenderSteps.begin();
	std::advance(it, pos);
	mCustomRenderSteps.emplace(it, step.Clone());
	return true;
}

void GeometryEngine::Camera::ClearCustomRenderSteps()
{
	for (auto iter = mCustomRenderSteps.begin(); iter != mCustomRenderSteps.end(); ++iter)
	{
		RenderStep* step = (*iter);
		delete(step);
	}
	mCustomRenderSteps.clear();
}
