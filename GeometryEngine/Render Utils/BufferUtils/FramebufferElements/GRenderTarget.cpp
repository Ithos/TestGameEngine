#include "GRenderTarget.h"
#include "../GFramebufferObject.h"

GeometryEngine::GeometryBuffer::GRenderTarget::GRenderTarget() : mSize(QVector2D(0, 0)), mAttachment(GL_NONE), mOwner(nullptr), mEnabled(false)
{
	init();
}

GeometryEngine::GeometryBuffer::GRenderTarget::GRenderTarget(const QVector2D & size) : mSize(size), mAttachment(GL_NONE), mOwner(nullptr), mEnabled(false)
{
	init();
}

GeometryEngine::GeometryBuffer::GRenderTarget::GRenderTarget(const GRenderTarget & ref)
{
	copy(ref);
}

void GeometryEngine::GeometryBuffer::GRenderTarget::copy(const GRenderTarget & ref)
{
	mSize = ref.mSize;
	mIndex = ref.mIndex;
}

bool GeometryEngine::GeometryBuffer::GRenderTarget::GetOwnerBound() const
{
	return (mOwner != nullptr && mOwner->IsBound());
}

void GeometryEngine::GeometryBuffer::GRenderTarget::init()
{
	initializeOpenGLFunctions();
}
