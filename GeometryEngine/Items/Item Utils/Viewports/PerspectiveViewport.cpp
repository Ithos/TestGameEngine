#include "PerspectiveViewport.h"

GeometryEngine::GeometryItemUtils::PerspectiveViewport::PerspectiveViewport(const QVector4D & viewportSize, GLdouble fovy, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar) :
	mFoView(fovy), mAspectRatio(aspectRatio), Viewport(viewportSize, zNear, zFar)
{
}

GeometryEngine::GeometryItemUtils::PerspectiveViewport::PerspectiveViewport(const PerspectiveViewport & ref) : Viewport()
{
	copy(ref);
}

GeometryEngine::GeometryItemUtils::PerspectiveViewport * GeometryEngine::GeometryItemUtils::PerspectiveViewport::Clone() const
{
	return new PerspectiveViewport((*this));
}

void GeometryEngine::GeometryItemUtils::PerspectiveViewport::CalculateProjectionMatrix()
{
	ResetCameraBeforeCalculation();
	mProjection.perspective(mFoView, mAspectRatio, mZNear, mZFar);
	ApplyCameraModelMatrix();
}

void GeometryEngine::GeometryItemUtils::PerspectiveViewport::copy(const PerspectiveViewport & ref)
{
	Viewport::copy(ref);
	mFoView = ref.mFoView;
	mAspectRatio = ref.mAspectRatio;
}
