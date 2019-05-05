#include "CompleteColorPostProcessBuffer.h"

GeometryEngine::GeometryBuffer::CompleteColorPostProcessBuffer::CompleteColorPostProcessBuffer()
{
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_AMBIENT);
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_DIFFUSE);
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_REFLECTIVE);
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_EMMISSIVE);
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_POSITION);
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_NORMAL);
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_TEXCOORD);
}

GeometryEngine::GeometryBuffer::CompleteColorPostProcessBuffer::CompleteColorPostProcessBuffer(const CompleteColorPostProcessBuffer & ref)
{
	copy(ref);
}

void GeometryEngine::GeometryBuffer::CompleteColorPostProcessBuffer::copy(const GBuffer & ref)
{
	GBuffer::copy(ref);
}
