#include "Render Utils\GBuffer.h"
#include "Render Utils\RenderBuffersData.h"
#include "DeferredShadingCamera.h"

GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera::DeferredShadingCamera(const GeometryRenderData::RenderBuffersData& gbuffer, const GeometryItemUtils::Viewport& viewport,
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
	mpGBufferData->Resize(size.z(), size.w()); 
}

GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera * GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera::Clone() const
{
	return new DeferredShadingCamera((*this));
}

GeometryEngine::GeometryBuffer::GBuffer * GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera::GetGBuffer()
{
	return mpGBufferData->GetGeometryBuffer();
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera::init(const GeometryRenderData::RenderBuffersData& gbuffer)
{
	mpGBufferData = gbuffer.Clone();
	QRect rec = QApplication::desktop()->screenGeometry();
	mpGBufferData->Init(rec.width(), rec.height());
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera::copy(const DeferredShadingCamera & ref)
{
	Camera::copy(ref);
}
