#include "Viewport.h"

GeometryEngine::GeometryItemUtils::Viewport::Viewport(const QVector4D & viewportSize, GLdouble zNear, GLdouble zFar) : mZNear(zNear), mZFar(zFar), mViewportSize(viewportSize)
{
	mProjection.setToIdentity();
	mViewProjection.setToIdentity();
	mViewMatrix.setToIdentity();
}

GeometryEngine::GeometryItemUtils::Viewport::Viewport(const Viewport & ref)
{
	copy(ref);
}

GeometryEngine::GeometryItemUtils::Viewport::~Viewport()
{
}

const QMatrix4x4 & GeometryEngine::GeometryItemUtils::Viewport::UpdateViewMatrix(const QVector3D & pos, const QQuaternion & rot, const QVector3D & scale)
{
	// The transformations on the camera are really the opposite transformations on the world
	mViewMatrix.setToIdentity();
	mViewMatrix.rotate(-rot);
	mViewMatrix.translate(-pos);
	mViewMatrix.scale(scale);
	return mViewMatrix;
}

void GeometryEngine::GeometryItemUtils::Viewport::ResetCameraBeforeCalculation()
{
	mProjection.setToIdentity();
	glViewport(mViewportSize.x(), mViewportSize.y(), mViewportSize.z(), mViewportSize.w());
}

void GeometryEngine::GeometryItemUtils::Viewport::copy(const Viewport & ref)
{
	this->mZNear = ref.mZNear;
	this->mZFar = ref.mZFar;
	this->mViewportSize = ref.mViewportSize;
	this->mProjection = ref.mProjection;
	this->mViewProjection = ref.mViewProjection;
	this->mViewMatrix = ref.mViewMatrix;
}
