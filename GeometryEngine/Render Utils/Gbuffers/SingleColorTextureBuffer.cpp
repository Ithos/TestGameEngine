#include "SingleColorTextureBuffer.h"

GeometryEngine::GeometryBuffer::SingleColorTextBuffer::SingleColorTextBuffer() : GBuffer()
{
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_DIFFUSE);
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_POSITION);
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_NORMAL);
	mActiveTextures.insert(GBUFFER_TEXTURE_TYPE_TEXCOORD);
}

GeometryEngine::GeometryBuffer::SingleColorTextBuffer::SingleColorTextBuffer(const SingleColorTextBuffer & ref)
{
	copy(ref);
}

void GeometryEngine::GeometryBuffer::SingleColorTextBuffer::copy(const GBuffer & ref)
{
	GBuffer::copy(ref);
}
