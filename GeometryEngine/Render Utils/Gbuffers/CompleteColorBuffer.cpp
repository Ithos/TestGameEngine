#include "CompleteColorBuffer.h"

GeometryEngine::GeometryBuffer::CompleteColorBuffer::CompleteColorBuffer()
{
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_AMBIENT);
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_DIFFUSE);
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_REFLECTIVE);
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_EMMISSIVE);
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_POSITION);
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_NORMAL);
}

GeometryEngine::GeometryBuffer::CompleteColorBuffer::CompleteColorBuffer(const CompleteColorBuffer & ref)
{
	copy(ref);
}

void GeometryEngine::GeometryBuffer::CompleteColorBuffer::copy(const GBuffer & ref)
{
	GBuffer::copy(ref);
}
