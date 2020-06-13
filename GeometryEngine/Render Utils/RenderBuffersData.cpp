#include "GBuffer.h"
#include "TranslucentBuffer.h"
#include "RenderBuffersData.h"

GeometryEngine::GeometryRenderData::RenderBuffersData::RenderBuffersData(const GeometryBuffer::GBuffer & geometryBuffer, const GeometryBuffer::TranslucentBuffer * const translucentBuffer):
	mpGeometryBuffer(nullptr), mpTranslucentBuffer(nullptr)
{
	mpGeometryBuffer = geometryBuffer.Clone();
	if (translucentBuffer != nullptr) mpTranslucentBuffer = translucentBuffer->Clone();
}
