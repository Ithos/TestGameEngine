#include "GTexture.h"

GeometryEngine::GeometryBuffer::GTexture::GTexture(const QVector2D & textureSize, const QVector3D & borderColor, 
	GFramebufferCommons::G_COLOR_ATTACHMENTS attachment, const GFramebufferObject* owner ) : GTexture(textureSize, borderColor)
{
	mOwner = owner;
	SetAttachment(attachment);
	Enable(true);
}

GeometryEngine::GeometryBuffer::GTexture::GTexture(const QVector2D & textureSize, const QVector3D & borderColor) : GTexture(textureSize)
{
	SetBorderColor(borderColor);
}

GeometryEngine::GeometryBuffer::GTexture::GTexture(const QVector2D & textureSize) : GRenderTarget(textureSize), mBorderColor(QVector3D(0.0f, 0.0f, 0.0f))
{
	glGenTextures(1, &mIndex);
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, textureSize.x(), textureSize.y(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	Unbind();
}

GeometryEngine::GeometryBuffer::GTexture::GTexture() : GRenderTarget(), mBorderColor(QVector3D(0.0f, 0.0f, 0.0f))
{
	glGenTextures(1, &mIndex);
	Bind();
	GLubyte data[] = { 255, 255, 255, 255 };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	Unbind();
}

GeometryEngine::GeometryBuffer::GTexture::GTexture(GFramebufferCommons::G_COLOR_ATTACHMENTS attachment, const GFramebufferObject* owner): GTexture()
{
	mOwner = owner;
	SetAttachment(attachment);
}

GeometryEngine::GeometryBuffer::GTexture::GTexture(const GTexture & ref): GRenderTarget()
{
	copy(ref);
}

GeometryEngine::GeometryBuffer::GTexture::~GTexture()
{
	if (mIndex != 0)
	{
		glDeleteTextures(1, &mIndex);
		mIndex = 0;
	}
}

void GeometryEngine::GeometryBuffer::GTexture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, mIndex);
}

void GeometryEngine::GeometryBuffer::GTexture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GeometryEngine::GeometryBuffer::GTexture::Clear()
{
	GLuint arr[3] = { 0,0,0 }; 
	Clear(arr);
}

void GeometryEngine::GeometryBuffer::GTexture::Clear(GLuint * values)
{
	if (GRenderTarget::GetOwnerBound())
	{
		Bind();
		glClearBufferuiv(GL_COLOR, mAttachment, values);
		Unbind();
	}
}

bool GeometryEngine::GeometryBuffer::GTexture::Enable(bool enable)
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

void GeometryEngine::GeometryBuffer::GTexture::Resize(const QVector2D & textureSize)
{
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, textureSize.x(), textureSize.y(), 0, GL_RGB, GL_FLOAT, NULL);
	mSize = textureSize;
	Unbind();
}

void GeometryEngine::GeometryBuffer::GTexture::SetAttachment(unsigned int attachmentIndex)
{
	GFramebufferCommons::G_COLOR_ATTACHMENTS attachment = GFramebufferCommons::G_COLOR_ATTACHMENTS::NONE;
	GFramebufferCommons::EnumCasts::CastColorAttachment(attachmentIndex, &attachment);
	SetAttachment(attachment);
}

void GeometryEngine::GeometryBuffer::GTexture::SetAttachment(GFramebufferCommons::G_COLOR_ATTACHMENTS attachment)
{
	mAttachment = static_cast<GLenum>(attachment);
}

void GeometryEngine::GeometryBuffer::GTexture::SetBorderColor(const QVector3D & borderColor)
{
	Bind();
	mBorderColor = borderColor;
	float color[] = { borderColor.x(), borderColor.y(), borderColor.z(), 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	Unbind();
}

void GeometryEngine::GeometryBuffer::GTexture::copy(const GTexture & ref)
{
	GRenderTarget::copy(ref);
	mBorderColor = ref.mBorderColor;
	mAttachment = ref.mAttachment;
}
