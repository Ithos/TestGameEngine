#include "GDepthStencilRenderBuffer.h"

GeometryEngine::GeometryBuffer::GDepthStencilRenderBuffer::GDepthStencilRenderBuffer(const QVector2D & textureSize) : 
	GRenderTarget(textureSize)
{
	glGenRenderbuffers(1, &mIndex);
	Resize(textureSize);
}

GeometryEngine::GeometryBuffer::GDepthStencilRenderBuffer::GDepthStencilRenderBuffer(const QVector2D & textureSize, GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS attachment, 
	const GFramebufferObject* owner) : GDepthStencilRenderBuffer(textureSize)
{
	mOwner = owner;
	SetAttachment(attachment);
	Enable(true);
}

GeometryEngine::GeometryBuffer::GDepthStencilRenderBuffer::GDepthStencilRenderBuffer(const GDepthStencilRenderBuffer & ref) : GRenderTarget()
{
	copy(ref);
}

GeometryEngine::GeometryBuffer::GDepthStencilRenderBuffer::~GDepthStencilRenderBuffer()
{
	if (mIndex != 0)
	{
		glDeleteBuffers(1, &mIndex);
		mIndex = 0;
	}
}

void GeometryEngine::GeometryBuffer::GDepthStencilRenderBuffer::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, mIndex);
}

void GeometryEngine::GeometryBuffer::GDepthStencilRenderBuffer::Unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void GeometryEngine::GeometryBuffer::GDepthStencilRenderBuffer::Clear()
{
	GLuint arr[3] = { 0,0,0 };
	Clear(arr);
}

void GeometryEngine::GeometryBuffer::GDepthStencilRenderBuffer::Clear(GLuint * values)
{
	if (GRenderTarget::GetOwnerBound())
	{
		Bind();
		glClearBufferuiv(GL_DEPTH_STENCIL, mAttachment, values);
		Unbind();
	}
}

void GeometryEngine::GeometryBuffer::GDepthStencilRenderBuffer::Resize(const QVector2D & textureSize)
{
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, textureSize.x(), textureSize.y());
	mSize = textureSize;
	Unbind();
}

void GeometryEngine::GeometryBuffer::GDepthStencilRenderBuffer::SetAttachment(unsigned int attachmentIndex)
{
	GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS attachment = GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS::NONE;
	GFramebufferCommons::EnumCasts::CastDepthStencilAttachment(attachmentIndex, &attachment);
	SetAttachment(attachment);
}

void GeometryEngine::GeometryBuffer::GDepthStencilRenderBuffer::SetAttachment(GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS attachment)
{
	mAttachment = static_cast<GLenum>(attachment);
}

bool GeometryEngine::GeometryBuffer::GDepthStencilRenderBuffer::Enable(bool enable)
{
	if (GRenderTarget::GetOwnerBound())
	{
		if (mEnabled != enable)
		{
			Bind();
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, mAttachment, GL_RENDERBUFFER, enable ? mIndex : 0);
			mEnabled = enable;
			Unbind();
			return true;
		}
	}
	return false;
}

void GeometryEngine::GeometryBuffer::GDepthStencilRenderBuffer::copy(const GDepthStencilRenderBuffer & ref)
{
	GRenderTarget::copy(ref);
}
