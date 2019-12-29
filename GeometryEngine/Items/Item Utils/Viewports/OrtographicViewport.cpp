#include "OrtographicViewport.h"

GeometryEngine::GeometryItemUtils::OrtographicViewport::OrtographicViewport(const QVector4D & viewportSize, const QRect & orthoSize, GLdouble zNear, GLdouble zFar):
	mOrthoSize(orthoSize), Viewport(viewportSize, zNear, zFar)
{
}

GeometryEngine::GeometryItemUtils::OrtographicViewport::OrtographicViewport(const OrtographicViewport & ref): Viewport()
{
	copy(ref);
}

GeometryEngine::GeometryItemUtils::OrtographicViewport * GeometryEngine::GeometryItemUtils::OrtographicViewport::Clone() const
{
	return new OrtographicViewport((*this));
}

void GeometryEngine::GeometryItemUtils::OrtographicViewport::CalculateProjectionMatrix()
{
	ResetCameraBeforeCalculation();
	mProjection.ortho(mOrthoSize.x(), mOrthoSize.x() + mOrthoSize.width(), mOrthoSize.y(), mOrthoSize.y() + mOrthoSize.height(), mZNear, mZFar);
	ApplyCameraModelMatrix();
}

void GeometryEngine::GeometryItemUtils::OrtographicViewport::copy(const OrtographicViewport & ref)
{
	Viewport::copy(ref);
	mOrthoSize = ref.mOrthoSize;
}
