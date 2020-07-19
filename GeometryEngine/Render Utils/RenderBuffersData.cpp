#include "GBuffer.h"
#include "TranslucentBuffer.h"
#include "RenderBuffersData.h"

GeometryEngine::GeometryRenderData::RenderBuffersData::RenderBuffersData(const GeometryBuffer::GBuffer & geometryBuffer, const GeometryBuffer::TranslucentBuffer * const translucentBuffer):
	mpGeometryBuffer(nullptr), mpTranslucentBuffer(nullptr)
{
	mpGeometryBuffer = geometryBuffer.Clone();
	if (translucentBuffer != nullptr) mpTranslucentBuffer = translucentBuffer->Clone();
}

GeometryEngine::GeometryRenderData::RenderBuffersData::~RenderBuffersData()
{
	if (mpGeometryBuffer != nullptr)
	{
		delete mpGeometryBuffer;
		mpGeometryBuffer = nullptr;
	}

	if(mpTranslucentBuffer != nullptr)
	{
		delete mpTranslucentBuffer;
		mpTranslucentBuffer = nullptr;
	}
}

void GeometryEngine::GeometryRenderData::RenderBuffersData::Init(unsigned int MaxWindowWidth, unsigned int MaxWindowHeight)
{
	if (mpGeometryBuffer != nullptr)mpGeometryBuffer->Init(MaxWindowWidth, MaxWindowHeight);
	if(mpTranslucentBuffer != nullptr)mpTranslucentBuffer->Init(MaxWindowWidth, MaxWindowHeight);
}

void GeometryEngine::GeometryRenderData::RenderBuffersData::Resize(unsigned int WindowWidth, unsigned int WindowHeight)
{
	if (mpGeometryBuffer != nullptr)mpGeometryBuffer->Resize(WindowWidth, WindowHeight);
	if (mpTranslucentBuffer != nullptr)mpTranslucentBuffer->Resize(WindowWidth, WindowHeight);
}

void GeometryEngine::GeometryRenderData::RenderBuffersData::copy(const RenderBuffersData & ref)
{
	mpGeometryBuffer = ref.mpGeometryBuffer == nullptr ? nullptr : ref.mpGeometryBuffer->Clone();
	mpTranslucentBuffer = ref.mpTranslucentBuffer == nullptr ? nullptr : ref.mpTranslucentBuffer->Clone();
}
