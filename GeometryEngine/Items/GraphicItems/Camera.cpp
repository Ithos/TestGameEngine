#include "Camera.h"

#include "Render Utils\RenderStep.h"

GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::Camera(const QVector4D& viewportSize, bool autoResize, GLdouble zNear, GLdouble zFar, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent) :
	WorldItem(pos, rot, scale, parent), mViewportSize(viewportSize), mZNear(zNear), mZFar(zFar), mAutoResize(autoResize), mpGBuffer(nullptr)
{
	mProjection.setToIdentity();
	CalculateModelMatrix();
}

GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::~Camera()
{
	if (mpGBuffer != nullptr)
	{
		delete mpGBuffer;
		mpGBuffer = nullptr;
	}

	ClearCustomRenderSteps();
	ClearPostProcess();
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::UpdateModelMatrix(bool updateChildren)
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

void GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::ResetCameraBeforeCalculation()
{
	mProjection.setToIdentity();
	glViewport(mViewportSize.x(), mViewportSize.y(), mViewportSize.z(), mViewportSize.w());
}

bool GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::AddCustomRenderStep(const GeometryRenderStep::RenderStep & step)
{
	mCustomRenderSteps.push_back(step.Clone());
	return true;
}

bool GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::RemoveCustomRenderStep(int pos)
{
	if (mCustomRenderSteps.size() >= pos)
		return false;

	std::list<GeometryRenderStep::RenderStep* >::iterator it = mCustomRenderSteps.begin();
	std::advance(it, pos);
	mCustomRenderSteps.remove((*it));
	return true;
}

bool GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::InsertCustomRenderStep(const GeometryRenderStep::RenderStep & step, unsigned int pos)
{
	if (pos >= mCustomRenderSteps.size())
		return false;

	std::list<GeometryRenderStep::RenderStep* >::iterator it = mCustomRenderSteps.begin();
	std::advance(it, pos);
	mCustomRenderSteps.emplace(it, step.Clone());
	return true;
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::ClearCustomRenderSteps()
{
	for (auto iter = mCustomRenderSteps.begin(); iter != mCustomRenderSteps.end(); ++iter)
	{
		GeometryRenderStep::RenderStep* step = (*iter);
		delete(step);
	}
	mCustomRenderSteps.clear();
}

bool GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::AddPostProcess(const GeometryPostProcess::PostProcess & process)
{
	mPostProcess.push_back(process.Clone());
	return true;
}

bool GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::RemovePostProcess(int pos)
{
	if (mPostProcess.size() >= pos)
		return false;

	std::list<GeometryPostProcess::PostProcess* >::iterator it = mPostProcess.begin();
	std::advance(it, pos);
	mPostProcess.remove((*it));
	return true;
}

bool GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::InsertPostProcess(const GeometryPostProcess::PostProcess & process, unsigned int pos)
{
	if (pos >= mPostProcess.size())
		return false;

	std::list<GeometryPostProcess::PostProcess* >::iterator it = mPostProcess.begin();
	std::advance(it, pos);
	mPostProcess.emplace(it, process.Clone());
	return true;
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::ClearPostProcess()
{
	for (auto iter = mPostProcess.begin(); iter != mPostProcess.end(); ++iter)
	{
		GeometryPostProcess::PostProcess* process = (*iter);
		delete(process);
	}
	mPostProcess.clear();
}
