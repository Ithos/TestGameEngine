#include "DeferredShadingCamera.h"

GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera::DeferredShadingCamera(const GeometryBuffer::GBuffer& gbuffer, const GeometryItemUtils::Viewport& viewport,
	bool autoResize, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent) :
	Camera(viewport, autoResize, pos, rot, scale, parent)
{
	init(gbuffer);
}

GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera::DeferredShadingCamera(const DeferredShadingCamera & ref) : Camera()
{
	copy(ref);
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera::SetViewportSize(const QVector4D & size)
{
	Camera::SetViewportSize(size);
	mpGBuffer->Resize(size.z(), size.w());
}

GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera * GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera::Clone() const
{
	return new DeferredShadingCamera((*this));
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera::init(const GeometryBuffer::GBuffer& gbuffer)
{
	mpGBuffer = gbuffer.Clone();
	QRect rec = QApplication::desktop()->screenGeometry();
	mpGBuffer->Init(rec.width(), rec.height());
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera::copy(const DeferredShadingCamera & ref)
{
	Camera::copy(ref);
}
