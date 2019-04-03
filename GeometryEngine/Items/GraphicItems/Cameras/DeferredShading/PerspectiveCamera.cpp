#include "PerspectiveCamera.h"

GeometryEngine::GeometryWorldItem::GeometryCamera::PerspectiveCamera::PerspectiveCamera(const GeometryBuffer::GBuffer& gbuffer,
	const QVector4D & viewportSize, GLdouble fovy, GLdouble aspectRatio, bool autoResize, GLdouble zNear, GLdouble zFar, const QVector3D & pos, 
	const QVector3D & rot, const QVector3D & scale, WorldItem * parent) : DeferredShadingCamera(gbuffer, viewportSize, autoResize, zNear, zFar, pos, rot, scale, parent),
	mFoView(fovy), mAspectRatio(aspectRatio)
{
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::PerspectiveCamera::CalculateProjectionMatrix()
{
	ResetCameraBeforeCalculation();
	mProjection.perspective(mFoView, mAspectRatio, mZNear, mZFar);
	ApplyCameraModelMatrix();
}
