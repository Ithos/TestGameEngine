#include "DeferredShadingCamera.h"

GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera::DeferredShadingCamera(const GeometryBuffer::GBuffer& gbuffer, const QVector4D & viewportSize,
	bool autoResize, GLdouble zNear, GLdouble zFar, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent) :
	Camera(viewportSize, autoResize, zNear, zFar, pos, rot, scale, parent)
{
	init(gbuffer);
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera::SetViewport(const QVector4D & size)
{
	Camera::SetViewport(size);
	mpGBuffer->Resize(mViewportSize.z(), mViewportSize.w());
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera::init(const GeometryBuffer::GBuffer& gbuffer)
{
	mpGBuffer = gbuffer.Clone();
	QRect rec = QApplication::desktop()->screenGeometry();
	mpGBuffer->Init(rec.width(), rec.height());
}
