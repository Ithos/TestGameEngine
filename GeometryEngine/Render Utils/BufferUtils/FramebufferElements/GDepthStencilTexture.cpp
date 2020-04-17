#include "GDepthStencilTexture.h"

GeometryEngine::GeometryBuffer::GDepthStencilTexture::GDepthStencilTexture(const QVector2D & textureSize) : GRenderTarget(textureSize)
{
	glGenTextures(1, &mIndex);
	Resize(textureSize);
}

GeometryEngine::GeometryBuffer::GDepthStencilTexture::GDepthStencilTexture(const QVector2D & textureSize, GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS attachment, const GFramebufferObject* owner) : 
	GDepthStencilTexture(textureSize)
{
	mOwner = owner;
	SetAttachment(attachment);
	Enable(true);
}

GeometryEngine::GeometryBuffer::GDepthStencilTexture::GDepthStencilTexture(const GDepthStencilTexture & ref) : GRenderTarget()
{
	copy(ref);
}

GeometryEngine::GeometryBuffer::GDepthStencilTexture::~GDepthStencilTexture()
{
	if (mIndex != 0) {
		glDeleteTextures(1, &mIndex);
		mIndex = 0;
	}
}

void GeometryEngine::GeometryBuffer::GDepthStencilTexture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, mIndex);
}

void GeometryEngine::GeometryBuffer::GDepthStencilTexture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GeometryEngine::GeometryBuffer::GDepthStencilTexture::Clear()
{
	GLuint arr[3] = { 0,0,0 };
	Clear(arr);
}

void GeometryEngine::GeometryBuffer::GDepthStencilTexture::Clear(GLuint * values)
{
	if (GRenderTarget::GetOwnerBound())
	{
		Bind();
		glClearBufferuiv(GL_DEPTH_STENCIL, mAttachment, values);
		Unbind();
	}
}

void GeometryEngine::GeometryBuffer::GDepthStencilTexture::Resize(const QVector2D & textureSize)
{
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, textureSize.x(), textureSize.y(), 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
	mSize = textureSize;
	Unbind();
}

void GeometryEngine::GeometryBuffer::GDepthStencilTexture::SetAttachment(unsigned int attachmentIndex)
{
	GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS attachment = GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS::NONE;
	GFramebufferCommons::EnumCasts::CastDepthStencilAttachment(attachmentIndex, &attachment);
	SetAttachment(attachment);
}

void GeometryEngine::GeometryBuffer::GDepthStencilTexture::SetAttachment(GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS attachment)
{
	mAttachment = static_cast<GLenum>(attachment);
}

bool GeometryEngine::GeometryBuffer::GDepthStencilTexture::Enable(bool enable)
{
	if (GRenderTarget::GetOwnerBound())
	{
		if (mEnabled != enable)
		{
			Bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, mAttachment, GL_TEXTURE_2D, enable ? mIndex : 0, 0);
			mEnabled = enable;
			Unbind();
		}
		return true;
	}
	return false;
}

void GeometryEngine::GeometryBuffer::GDepthStencilTexture::copy(const GDepthStencilTexture & ref)
{
	GRenderTarget::copy(ref);
}
