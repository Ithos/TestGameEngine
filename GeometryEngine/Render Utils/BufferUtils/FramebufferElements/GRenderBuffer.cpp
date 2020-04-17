#include "GRenderBuffer.h"

GeometryEngine::GeometryBuffer::GRenderBuffer::GRenderBuffer(const QVector2D & textureSize) : GRenderTarget(textureSize)
{
	glGenRenderbuffers(1, &mIndex);
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB32F, textureSize.x(), textureSize.y());
	Unbind();
}

GeometryEngine::GeometryBuffer::GRenderBuffer::GRenderBuffer(const QVector2D & textureSize, GFramebufferCommons::G_COLOR_ATTACHMENTS attachment, const GFramebufferObject* owner) : 
	GRenderBuffer(textureSize)
{
	mOwner = owner;
	SetAttachment(attachment);
	Enable(true);
}

GeometryEngine::GeometryBuffer::GRenderBuffer::GRenderBuffer(const GRenderBuffer & ref) : GRenderTarget()
{
	copy(ref);
}

GeometryEngine::GeometryBuffer::GRenderBuffer::~GRenderBuffer()
{
	if (mIndex != 0)
	{
		glDeleteBuffers(1, &mIndex);
		mIndex = 0;
	}
}

void GeometryEngine::GeometryBuffer::GRenderBuffer::Bind()
{
	glBindRenderbuffer(GL_FRAMEBUFFER, mIndex);
}

void GeometryEngine::GeometryBuffer::GRenderBuffer::Unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void GeometryEngine::GeometryBuffer::GRenderBuffer::Clear()
{
	GLuint arr[3] = { 0,0,0 };
	Clear(arr);
}

void GeometryEngine::GeometryBuffer::GRenderBuffer::Clear(GLuint * values)
{
	if (GRenderTarget::GetOwnerBound())
	{
		Bind();
		glClearBufferuiv(GL_COLOR, mAttachment, values);
		Unbind();
	}
}

bool GeometryEngine::GeometryBuffer::GRenderBuffer::Enable(bool enable)
{
	if (GRenderTarget::GetOwnerBound())
	{
		if (mEnabled != enable)
		{
			Bind();
			glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, mAttachment, GL_RENDERBUFFER, enable ? mIndex : 0);
			mEnabled = enable;
			Unbind();
		}
		return true;
	}
	return false;
}

void GeometryEngine::GeometryBuffer::GRenderBuffer::Resize(const QVector2D & textureSize)
{
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB32F, textureSize.x(), textureSize.y());
	mSize = textureSize;
	Unbind();
}

void GeometryEngine::GeometryBuffer::GRenderBuffer::SetAttachment(unsigned int attachmentIndex)
{
	GFramebufferCommons::G_COLOR_ATTACHMENTS attachment = GFramebufferCommons::G_COLOR_ATTACHMENTS::NONE;
	GFramebufferCommons::EnumCasts::CastColorAttachment(attachmentIndex, &attachment);
	SetAttachment(attachment);
}

void GeometryEngine::GeometryBuffer::GRenderBuffer::SetAttachment(GFramebufferCommons::G_COLOR_ATTACHMENTS attachment)
{
	mAttachment = static_cast<GLenum>(attachment);
}

void GeometryEngine::GeometryBuffer::GRenderBuffer::copy(const GRenderBuffer & ref)
{
	GRenderTarget::copy(ref);
}
