#include "BufferUtils\GFramebufferManager.h"
#include "BufferUtils\GFramebufferObject.h"
#include "BufferUtils\FramebufferElements\GRenderTarget.h"
#include "../Items/CommonItemParameters.h"
#include "TranslucentBuffer.h"

GeometryEngine::GeometryBuffer::TranslucentBuffer::TranslucentBuffer(): mpFBO(nullptr), mMaxTextureSize(0.0f, 0.0f), mTextureSize(0.0f, 0.0f)
{
	initializeOpenGLFunctions();
}

GeometryEngine::GeometryBuffer::TranslucentBuffer::TranslucentBuffer(const TranslucentBuffer & ref)
{
	copy(ref);
}

GeometryEngine::GeometryBuffer::TranslucentBuffer::~TranslucentBuffer()
{
	if (mpFBO != nullptr)
	{
		delete mpFBO;
		mpFBO = nullptr;
	}
}

bool GeometryEngine::GeometryBuffer::TranslucentBuffer::Init(unsigned int MaxWindowWidth, unsigned int MaxWindowHeight)
{
	if (mpFBO != nullptr) return false;

	mMaxTextureSize.setX(MaxWindowWidth); mMaxTextureSize.setY(MaxWindowHeight);
	// Create the FBO
	mpFBO = GFramebufferManager::GetInstance()->GetNewFramebuffer();

	mpFBO->Bind(GeometryBuffer::DRAW);

	for (unsigned int i = 0; i < TBUFFER_NUM_TEXTURES; i++) {
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

bool GeometryEngine::GeometryBuffer::TranslucentBuffer::Resize(unsigned int WindowWidth, unsigned int WindowHeight)
{
	mpFBO->Bind(GeometryBuffer::DRAW);

	QVector2D size(WindowWidth, WindowHeight);

	for (unsigned int i = 0; i < TBUFFER_NUM_TEXTURES; i++) {
		mpFBO->GetColorTarget(i)->Resize(size);
	}

	mpFBO->GetStencilDepthTexture()->Resize(size);
	mTextureSize.setX(WindowWidth); mTextureSize.setY(WindowHeight);
	mpFBO->Unbind();

	return true;
}

void GeometryEngine::GeometryBuffer::TranslucentBuffer::StartFrame()
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

void GeometryEngine::GeometryBuffer::TranslucentBuffer::BindColorMaps()
{
	mpFBO->Bind(GeometryBuffer::DRAW);
	unsigned int ColorMapBuffers[] = { 0, 1 };
	mpFBO->Draw(ColorMapBuffers, 2);
}

void GeometryEngine::GeometryBuffer::TranslucentBuffer::ResetBindings()
{
	for (unsigned int i = 0; i < TBUFFER_NUM_TEXTURES; i++) {
		glActiveTexture(GL_TEXTURE0 + i); // The framebuffer object wrapper doesn't manage the texture units
		mpFBO->GetColorTarget(i)->Unbind();
	}
}

void GeometryEngine::GeometryBuffer::TranslucentBuffer::UnbindBuffer()
{
	for (unsigned int i = 0; i < TBUFFER_NUM_TEXTURES; i++) {
		UnbindTexture((TBUFFER_TEXTURE_TYPE)i);
	}
}

void GeometryEngine::GeometryBuffer::TranslucentBuffer::BindTexture(TBUFFER_TEXTURE_TYPE tex)
{
	BindTexture(tex, (unsigned int)tex);
}

void GeometryEngine::GeometryBuffer::TranslucentBuffer::BindTexture(TBUFFER_TEXTURE_TYPE tex, unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	mpFBO->GetColorTarget((unsigned int)tex)->Bind();
}

void GeometryEngine::GeometryBuffer::TranslucentBuffer::UnbindTexture(TBUFFER_TEXTURE_TYPE tex)
{
	UnbindTexture(tex, (unsigned int)tex);
}

void GeometryEngine::GeometryBuffer::TranslucentBuffer::UnbindTexture(TBUFFER_TEXTURE_TYPE tex, unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	mpFBO->GetColorTarget((unsigned int)tex)->Unbind();
}

void GeometryEngine::GeometryBuffer::TranslucentBuffer::BindShadowMapTextureWrite()
{
	mpFBO->Bind(GeometryBuffer::DRAW_READ);
	mpFBO->Draw(TBUFFER_TEXTURE_TYPE_SHADOW_MAP);
}

void GeometryEngine::GeometryBuffer::TranslucentBuffer::ClearColorTexture(TBUFFER_TEXTURE_TYPE texture)
{
	mpFBO->GetColorTarget((unsigned int)texture)->Clear();
}

void GeometryEngine::GeometryBuffer::TranslucentBuffer::FillGBufferInfo(TBufferTextureInfo & bufferInfo)
{
	// Indices of the texture units in which we store each texture by default
	bufferInfo.DiffuseColorMapTexture = TBUFFER_TEXTURE_TYPE_DIFFUSE_MAP;
	bufferInfo.SpecularColorMapTexture = TBUFFER_TEXTURE_TYPE_SPECULAR_MAP;
	bufferInfo.ShadowMapTexture = TBUFFER_TEXTURE_TYPE_SHADOW_MAP;
	bufferInfo.TranslucentDepthMapTexture = TBUFFER_TEXTURE_TYPE_TRANSLUCENT_DEPTH_MAP;
}

void GeometryEngine::GeometryBuffer::TranslucentBuffer::copy(const TranslucentBuffer & ref)
{
	this->mTextureSize.setX(ref.mTextureSize.x());
	this->mTextureSize.setY(ref.mTextureSize.y());
	this->mMaxTextureSize.setX(ref.mMaxTextureSize.x());
	this->mMaxTextureSize.setY(ref.mMaxTextureSize.y());
	this->mpFBO = nullptr;
}


