#include "BufferUtils\GFramebufferManager.h"
#include "BufferUtils\GFramebufferObject.h"
#include "BufferUtils\FramebufferElements\GRenderTarget.h"
#include "../Items/CommonItemParameters.h"
#include "ShadingBuffer.h"

const int GeometryEngine::GeometryBuffer::ShadingBuffer::mTextureUnits[SHADINGBUFFER_NUM_TEXTURES] = { 8, 9, 10, 11 };

GeometryEngine::GeometryBuffer::ShadingBuffer::ShadingBuffer(): mpFBO(nullptr), mMaxTextureSize(0.0f, 0.0f), mTextureSize(0.0f, 0.0f)
{
}

GeometryEngine::GeometryBuffer::ShadingBuffer::ShadingBuffer(const ShadingBuffer & ref)
{
	copy(ref);
}

GeometryEngine::GeometryBuffer::ShadingBuffer::~ShadingBuffer()
{
	if (mpFBO != nullptr)
	{
		delete mpFBO;
		mpFBO = nullptr;
	}
}

bool GeometryEngine::GeometryBuffer::ShadingBuffer::Init(unsigned int MaxWindowWidth, unsigned int MaxWindowHeight)
{
	initializeOpenGLFunctions();

	if (mpFBO != nullptr) return false;

	mMaxTextureSize.setX(MaxWindowWidth); mMaxTextureSize.setY(MaxWindowHeight);
	// Create the FBO
	mpFBO = GFramebufferManager::GetInstance()->GetNewFramebuffer();

	mpFBO->Bind(GeometryBuffer::DRAW);

	for (unsigned int i = 0; i < SHADINGBUFFER_NUM_TEXTURES; i++) {
		mpFBO->AddTexture(QVector2D(MaxWindowWidth, MaxWindowHeight), GFramebufferCommons::G_COLOR_ATTACHMENTS(GL_COLOR_ATTACHMENT0 + i));
	}

	mpFBO->SetStencilDepthTexture(QVector2D(MaxWindowWidth, MaxWindowHeight), GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS(GL_DEPTH_STENCIL_ATTACHMENT));

	mTextureSize.setX(MaxWindowWidth); mTextureSize.setY(MaxWindowHeight);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

	mpFBO->Unbind();

	return true;
}

bool GeometryEngine::GeometryBuffer::ShadingBuffer::Resize(unsigned int WindowWidth, unsigned int WindowHeight)
{
	mpFBO->Bind(GeometryBuffer::DRAW);

	QVector2D size(WindowWidth, WindowHeight);

	for (unsigned int i = 0; i < SHADINGBUFFER_NUM_TEXTURES; i++) {
		mpFBO->GetColorTarget(i)->Resize(size);
	}

	mpFBO->GetStencilDepthTexture()->Resize(size);
	mTextureSize.setX(WindowWidth); mTextureSize.setY(WindowHeight);
	mpFBO->Unbind();

	return true;
}

void GeometryEngine::GeometryBuffer::ShadingBuffer::StartFrame()
{
	GLfloat clearColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);

	mpFBO->Bind(GeometryBuffer::DRAW);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	{
		unsigned int DrawBuffers[] = { 0, 1 };
		mpFBO->Draw(DrawBuffers, 2);
	}
	glClear(GL_COLOR_BUFFER_BIT);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	{
		unsigned int DrawBuffers[] = { 2, 3 };
		mpFBO->Draw(DrawBuffers, 2);
		mpFBO->GetStencilDepthTexture()->Bind();
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
}

void GeometryEngine::GeometryBuffer::ShadingBuffer::BindColorMaps()
{
	mpFBO->Bind(GeometryBuffer::DRAW);
	unsigned int ColorMapBuffers[] = { 0, 1 };
	mpFBO->Draw(ColorMapBuffers, 2);
}

void GeometryEngine::GeometryBuffer::ShadingBuffer::ResetBindings()
{
	for (unsigned int i = 0; i < SHADINGBUFFER_NUM_TEXTURES; i++) {
		glActiveTexture(GL_TEXTURE0 + mTextureUnits[i]); // The framebuffer object wrapper doesn't manage the texture units
		mpFBO->GetColorTarget(i)->Unbind();
	}
}

void GeometryEngine::GeometryBuffer::ShadingBuffer::UnbindBuffer()
{
	for (unsigned int i = 0; i < SHADINGBUFFER_NUM_TEXTURES; i++) {
		UnbindTexture((SHADINGBUFFER_TEXTURE_TYPE)i);
	}
}

void GeometryEngine::GeometryBuffer::ShadingBuffer::BindTexture(SHADINGBUFFER_TEXTURE_TYPE tex)
{
	BindTexture(tex, (unsigned int)tex);
}

void GeometryEngine::GeometryBuffer::ShadingBuffer::BindTexture(SHADINGBUFFER_TEXTURE_TYPE tex, unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + mTextureUnits[textureUnit]);

	mpFBO->GetColorTarget((unsigned int)tex)->Bind();
}

void GeometryEngine::GeometryBuffer::ShadingBuffer::UnbindTexture(SHADINGBUFFER_TEXTURE_TYPE tex)
{
	UnbindTexture(tex, (unsigned int)tex);
}

void GeometryEngine::GeometryBuffer::ShadingBuffer::UnbindTexture(SHADINGBUFFER_TEXTURE_TYPE tex, unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + mTextureUnits[textureUnit]);
	mpFBO->GetColorTarget((unsigned int)tex)->Unbind();
}

void GeometryEngine::GeometryBuffer::ShadingBuffer::BindShadowMapTextureWrite()
{
	mpFBO->Bind(GeometryBuffer::DRAW_READ);
	mpFBO->Draw(SHADINGBUFFER_TEXTURE_TYPE_SHADOW_MAP);
}

void GeometryEngine::GeometryBuffer::ShadingBuffer::BindShadowMapTextureRead()
{
	mpFBO->Bind(GeometryBuffer::DRAW_READ);
	mpFBO->Read(SHADINGBUFFER_TEXTURE_TYPE_SHADOW_MAP);
}

void GeometryEngine::GeometryBuffer::ShadingBuffer::ClearColorTexture(SHADINGBUFFER_TEXTURE_TYPE texture)
{
	mpFBO->GetColorTarget((unsigned int)texture)->Clear();
}

void GeometryEngine::GeometryBuffer::ShadingBuffer::FillShadingBufferInfo(ShadingBufferTextureInfo & bufferInfo)
{
	// Indices of the texture units in which we store each texture by default
	bufferInfo.DiffuseColorMapTexture = mTextureUnits[SHADINGBUFFER_TEXTURE_TYPE_DIFFUSE_MAP];
	bufferInfo.SpecularColorMapTexture = mTextureUnits[SHADINGBUFFER_TEXTURE_TYPE_SPECULAR_MAP];
	bufferInfo.ShadowMapTexture = mTextureUnits[SHADINGBUFFER_TEXTURE_TYPE_SHADOW_MAP];
	bufferInfo.TranslucentDepthMapTexture = mTextureUnits[SHADINGBUFFER_TEXTURE_TYPE_TRANSLUCENT_DEPTH_MAP];
}

void GeometryEngine::GeometryBuffer::ShadingBuffer::copy(const ShadingBuffer & ref)
{
	this->mTextureSize.setX(ref.mTextureSize.x());
	this->mTextureSize.setY(ref.mTextureSize.y());
	this->mMaxTextureSize.setX(ref.mMaxTextureSize.x());
	this->mMaxTextureSize.setY(ref.mMaxTextureSize.y());
	this->mpFBO = nullptr;
}


