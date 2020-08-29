#include "GBuffer.h"
#include "ShadingBuffer.h"
#include "RenderBuffersData.h"

GeometryEngine::GeometryRenderData::RenderBuffersData::RenderBuffersData(const GeometryBuffer::GBuffer & geometryBuffer, const GeometryBuffer::ShadingBuffer * const shadingBuffer):
	mpGeometryBuffer(nullptr), mpShadingBuffer(nullptr)
{
	mpGeometryBuffer = geometryBuffer.Clone();
	if (shadingBuffer != nullptr) mpShadingBuffer = shadingBuffer->Clone();
}

GeometryEngine::GeometryRenderData::RenderBuffersData::~RenderBuffersData()
{
	if (mpGeometryBuffer != nullptr)
	{
		delete mpGeometryBuffer;
		mpGeometryBuffer = nullptr;
	}

	if(mpShadingBuffer != nullptr)
	{
		delete mpShadingBuffer;
		mpShadingBuffer = nullptr;
	}
}

void GeometryEngine::GeometryRenderData::RenderBuffersData::Init(unsigned int MaxWindowWidth, unsigned int MaxWindowHeight)
{
	if (mpGeometryBuffer != nullptr)mpGeometryBuffer->Init(MaxWindowWidth, MaxWindowHeight);
	if(mpShadingBuffer != nullptr)mpShadingBuffer->Init(MaxWindowWidth, MaxWindowHeight);
}

void GeometryEngine::GeometryRenderData::RenderBuffersData::Resize(unsigned int WindowWidth, unsigned int WindowHeight)
{
	if (mpGeometryBuffer != nullptr)mpGeometryBuffer->Resize(WindowWidth, WindowHeight);
	if (mpShadingBuffer != nullptr)mpShadingBuffer->Resize(WindowWidth, WindowHeight);
}

void GeometryEngine::GeometryRenderData::RenderBuffersData::copy(const RenderBuffersData & ref)
{
	mpGeometryBuffer = ref.mpGeometryBuffer == nullptr ? nullptr : ref.mpGeometryBuffer->Clone();
	mpShadingBuffer = ref.mpShadingBuffer == nullptr ? nullptr : ref.mpShadingBuffer->Clone();
}
