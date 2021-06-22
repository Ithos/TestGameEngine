#include "BufferUtils\GFramebufferManager.h"
#include "BufferUtils\GFramebufferObject.h"
#include "BufferUtils\FramebufferElements\GRenderTarget.h"
#include "../Items/CommonItemParameters.h"
#include <assert.h>
#include "GBuffer.h"

const int GeometryEngine::GeometryBuffer::GBuffer::mTextureUnits[GBUFFER_NUM_TEXTURES + 1] = { 0, 1, 2, 3, 4, 5, 6, 7 };

GeometryEngine::GeometryBuffer::GBuffer::GBuffer() : mFinalTextureLocation(0), mpFBO(nullptr), mMaxTextureSize(0.0f, 0.0f), mTextureSize(0.0f, 0.0f)
{
}

GeometryEngine::GeometryBuffer::GBuffer::GBuffer(const GBuffer & ref)
{
	copy(ref);
}

GeometryEngine::GeometryBuffer::GBuffer::~GBuffer()
{
	if (mpFBO != nullptr)
	{
		delete mpFBO;
		mpFBO = nullptr;
	}
}

bool GeometryEngine::GeometryBuffer::GBuffer::Init(unsigned int MaxWindowWidth, unsigned int MaxWindowHeight)
{
	initializeOpenGLFunctions();

	if (mpFBO != nullptr) return false;

	mMaxTextureSize.setX(MaxWindowWidth); mMaxTextureSize.setY(MaxWindowHeight);
	// Create the FBO
	mpFBO = GFramebufferManager::GetInstance()->GetNewFramebuffer();
	
	mpFBO->Bind(GeometryBuffer::DRAW);

	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		mActiveTextures.find((GBUFFER_TEXTURE_TYPE)i) != mActiveTextures.end() ?
			mpFBO->AddTexture(QVector2D(MaxWindowWidth, MaxWindowHeight), GFramebufferCommons::G_COLOR_ATTACHMENTS(GL_COLOR_ATTACHMENT0 + i)) :
			mpFBO->AddTexture(GFramebufferCommons::G_COLOR_ATTACHMENTS(GL_COLOR_ATTACHMENT0 + i));
	}
	
	mpFBO->AddTexture(QVector2D(MaxWindowWidth, MaxWindowHeight), GFramebufferCommons::G_COLOR_ATTACHMENTS(GL_COLOR_ATTACHMENT0 + GBUFFER_NUM_TEXTURES));

	mpFBO->SetStencilDepthBuffer(QVector2D(MaxWindowWidth, MaxWindowHeight), GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS(GL_DEPTH_STENCIL_ATTACHMENT));
	mpFBO->GetStencilDepthBuffer()->Enable(false);
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

bool GeometryEngine::GeometryBuffer::GBuffer::Resize(unsigned int WindowWidth, unsigned int WindowHeight)
{
	mpFBO->Bind(GeometryBuffer::DRAW);

	QVector2D size(WindowWidth, WindowHeight);

	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		if (mActiveTextures.find((GBUFFER_TEXTURE_TYPE)i) != mActiveTextures.end())	mpFBO->GetColorTarget(i)->Resize(size);
	}

	mpFBO->GetColorTarget(GBUFFER_NUM_TEXTURES)->Resize(size);
	mpFBO->GetStencilDepthTexture()->Resize(size);
	mpFBO->GetStencilDepthBuffer()->Resize(size);

	mTextureSize.setX(WindowWidth); mTextureSize.setY(WindowHeight);

	mpFBO->Unbind();

	return true;
}

void GeometryEngine::GeometryBuffer::GBuffer::StartFrame()
{
	mpFBO->Bind(GeometryBuffer::DRAW);
	mpFBO->Draw(GBUFFER_NUM_TEXTURES); 
	mpFBO->GetStencilDepthTexture()->Bind();

	// Clear final texture and depth texture
	glClear(GL_COLOR_BUFFER_BIT);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindForGeomPass()
{
	mpFBO->Bind(GeometryBuffer::DRAW);
	unsigned int DrawBuffers[] = { 0, 1, 2, 3, 4, 5, 6 };
	mpFBO->Draw(DrawBuffers, GBUFFER_NUM_TEXTURES);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindForStencilPass()
{
	// must disable the draw buffers 
	mpFBO->DisableDraw();
}

void GeometryEngine::GeometryBuffer::GBuffer::BindForLightPass()// Buffer to final
{
	mpFBO->Bind(GeometryBuffer::DRAW_READ);
	mpFBO->Draw(GBUFFER_NUM_TEXTURES);
	BindBuffer();
}

void GeometryEngine::GeometryBuffer::GBuffer::BindForFinalPass()
{
	mpFBO->Unbind();
	mpFBO->Bind(GeometryBuffer::READ);
	mpFBO->Read(GBUFFER_NUM_TEXTURES);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindTexturesForPostProcess()
{
	mpFBO->Bind(GeometryBuffer::DRAW_READ);
	mpFBO->Read(GBUFFER_NUM_TEXTURES);
	mpFBO->Draw(GBUFFER_TEXTURE_TYPE_TEXCOORD);
}

void GeometryEngine::GeometryBuffer::GBuffer::FinishPostProcessBinding()
{
	mpFBO->Bind(GeometryBuffer::DRAW_READ);
	mpFBO->Read(GBUFFER_TEXTURE_TYPE_TEXCOORD);
	mpFBO->Draw(GBUFFER_NUM_TEXTURES);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindTmpTextureWrite()
{
	mpFBO->Bind(GeometryBuffer::DRAW_READ);
	mpFBO->Draw(GBUFFER_TEXTURE_TYPE_TEXCOORD);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindBuffer()
{
	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		glActiveTexture(GL_TEXTURE0 + mTextureUnits[i]); // The framebuffer object wrapper doesn't manage the texture units
		mpFBO->GetColorTarget(i)->Bind();
	}
}

void GeometryEngine::GeometryBuffer::GBuffer::BindFinalTexture(GBUFFER_TEXTURE_TYPE location)
{
	glActiveTexture(GL_TEXTURE0 + mTextureUnits[location]);
	mpFBO->GetColorTarget(GBUFFER_NUM_TEXTURES)->Bind();
	mFinalTextureLocation = location;
}

void GeometryEngine::GeometryBuffer::GBuffer::BindFinalTexture(GBUFFER_TEXTURE_TYPE location, GBufferTextureInfo & bufferInfo)
{
	BindFinalTexture(location);
	bufferInfo.FinalTexture = mFinalTextureLocation;
}

void GeometryEngine::GeometryBuffer::GBuffer::ResetBindings()
{
	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		glActiveTexture(GL_TEXTURE0 + mTextureUnits[i]); // The framebuffer object wrapper doesn't manage the texture units
		mpFBO->GetColorTarget(i)->Unbind();
	}
}

void GeometryEngine::GeometryBuffer::GBuffer::UnbindBuffer()
{
	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		UnbindTexture((GBUFFER_TEXTURE_TYPE)i);
	}
}

void GeometryEngine::GeometryBuffer::GBuffer::UnbindFinalTexture()
{
	
	glActiveTexture(GL_TEXTURE0 + mTextureUnits[mFinalTextureLocation]);
	mpFBO->GetColorTarget((unsigned int)GBUFFER_NUM_TEXTURES)->Unbind();
	mFinalTextureLocation = 0;
}

void GeometryEngine::GeometryBuffer::GBuffer::BindTextureRead(unsigned int index)
{
	bindTextureMode(GeometryBuffer::READ, index);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindTextureDraw(unsigned int index)
{
	bindTextureMode(GeometryBuffer::DRAW, index);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindTextureDrawRead(unsigned int index)
{
	bindTextureMode(GeometryBuffer::DRAW_READ, index);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindTexture(unsigned int index)
{
	assert((index >= 0 || index <= GBUFFER_NUM_TEXTURES) && "index outside bounds");
	BindTexture((GBUFFER_TEXTURE_TYPE)index);

}

void GeometryEngine::GeometryBuffer::GBuffer::BindTexture(unsigned int index, unsigned int textureUnit)
{
	assert((index >= 0 || index <= GBUFFER_NUM_TEXTURES) && "index outside bounds");
	BindTexture((GBUFFER_TEXTURE_TYPE)index, textureUnit);
}

void GeometryEngine::GeometryBuffer::GBuffer::UnbindTexture(unsigned int index)
{
	assert((index >= 0 || index <= GBUFFER_NUM_TEXTURES) && "index outside bounds");
	UnbindTexture((GBUFFER_TEXTURE_TYPE)index);

}

void GeometryEngine::GeometryBuffer::GBuffer::UnbindTexture(unsigned int index, unsigned int textureUnit)
{
	assert((index >= 0 || index <= GBUFFER_NUM_TEXTURES) && "index outside bounds");
	UnbindTexture((GBUFFER_TEXTURE_TYPE)index, textureUnit);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindTexture(GBUFFER_TEXTURE_TYPE tex)
{
	BindTexture(tex, (unsigned int)tex);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindTexture(GBUFFER_TEXTURE_TYPE tex, unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + mTextureUnits[textureUnit]);
	mpFBO->GetColorTarget((unsigned int)tex)->Bind();
}

void GeometryEngine::GeometryBuffer::GBuffer::UnbindTexture(GBUFFER_TEXTURE_TYPE tex)
{
	UnbindTexture(tex, (unsigned int)tex);
}

void GeometryEngine::GeometryBuffer::GBuffer::UnbindTexture(GBUFFER_TEXTURE_TYPE tex, unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + mTextureUnits[textureUnit]);
	mpFBO->GetColorTarget((unsigned int)tex)->Unbind();
}

void GeometryEngine::GeometryBuffer::GBuffer::bindTextureMode(GeometryBuffer::G_FRAMEBUFFER_BINDS mode, unsigned int tex)
{
	assert((tex >= 0 || tex <= GBUFFER_NUM_TEXTURES) && "index outside bounds");
	mpFBO->Unbind();
	mpFBO->Bind(mode);
	mpFBO->Read(tex);
}

void GeometryEngine::GeometryBuffer::GBuffer::copy(const GBuffer & ref)
{
	this->mTextureSize.setX(ref.mTextureSize.x());
	this->mTextureSize.setY(ref.mTextureSize.y());
	this->mMaxTextureSize.setX(ref.mMaxTextureSize.x());
	this->mMaxTextureSize.setY(ref.mMaxTextureSize.y());

	this->mpFBO = nullptr;

	for (auto it = ref.mActiveTextures.begin(); it != ref.mActiveTextures.end(); ++it)
	{
		this->mActiveTextures.insert((*it));
	}
}

void GeometryEngine::GeometryBuffer::GBuffer::ClearColorTexture(GBUFFER_TEXTURE_TYPE texture)
{
	mpFBO->GetColorTarget((unsigned int)texture)->Clear();
}

void GeometryEngine::GeometryBuffer::GBuffer::DetachDepthTexture()
{
	mpFBO->GetStencilDepthTexture()->Enable(false);
	mpFBO->GetStencilDepthBuffer()->Enable(true);
}

void GeometryEngine::GeometryBuffer::GBuffer::AttachDepthTexture()
{
	mpFBO->GetStencilDepthBuffer()->Enable(false);
	mpFBO->GetStencilDepthTexture()->Enable(true);
}

void GeometryEngine::GeometryBuffer::GBuffer::FillGBufferInfo(GBufferTextureInfo & bufferInfo)
{
	// Indices of the texture units in which we store each texture by default
	bufferInfo.AmbientTexture = mTextureUnits[(unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_AMBIENT];
	bufferInfo.DiffuseTexture = mTextureUnits[(unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_DIFFUSE];
	bufferInfo.ReflectiveTexture = mTextureUnits[(unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_REFLECTIVE];
	bufferInfo.EmissiveTexture = mTextureUnits[(unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_EMMISSIVE];
	bufferInfo.PositionTexture = mTextureUnits[(unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_POSITION];
	bufferInfo.NormalTexture = mTextureUnits[(unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_NORMAL];
	bufferInfo.TmpTexture = mTextureUnits[(unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_TEXCOORD];
	bufferInfo.FinalTexture = mTextureUnits[mFinalTextureLocation];
	

	// Boolean values that indicate if each texture exists
	bufferInfo.UseAmbientTexture = this->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_AMBIENT);
	bufferInfo.UseDiffuseTexture = this->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	bufferInfo.UseReflectiveTexture = this->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_REFLECTIVE);
	bufferInfo.UseEmissiveTexture = this->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_EMMISSIVE);
	bufferInfo.UsePositionTexture = this->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_POSITION);
	bufferInfo.UseNormalTexture = this->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_NORMAL);
	bufferInfo.UseTmpTexture = this->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_TEXCOORD);
}
