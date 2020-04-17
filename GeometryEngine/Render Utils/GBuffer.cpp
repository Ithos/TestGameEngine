#include "BufferUtils\GFramebufferManager.h"
#include "BufferUtils\GFramebufferObject.h"
#include "GBuffer.h"

GeometryEngine::GeometryBuffer::GBuffer::GBuffer() : mFinalTextureLocation(0), mpFBO(nullptr)
{
	initializeOpenGLFunctions();
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

	// This remains
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
		glActiveTexture(GL_TEXTURE0 + i); // The framebuffer object wrapper doesn't manage the texture units
		mpFBO->GetColorTarget(i)->Bind();
	}
}

void GeometryEngine::GeometryBuffer::GBuffer::BindFinalTexture(GBUFFER_TEXTURE_TYPE location)
{
	glActiveTexture(GL_TEXTURE0 + location);
	mpFBO->GetColorTarget(mFinalTexture)->Bind();
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
		glActiveTexture(GL_TEXTURE0 + i); // The framebuffer object wrapper doesn't manage the texture units
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
	
	glActiveTexture(GL_TEXTURE0 + mFinalTextureLocation);
	mpFBO->GetColorTarget(mFinalTexture)->Unbind();
	mFinalTextureLocation = 0;
}

void GeometryEngine::GeometryBuffer::GBuffer::BindTexture(GBUFFER_TEXTURE_TYPE tex)
{
	glActiveTexture(GL_TEXTURE0 + (unsigned int)tex);
	mpFBO->GetColorTarget((unsigned int)tex)->Bind();
}

void GeometryEngine::GeometryBuffer::GBuffer::UnbindTexture(GBUFFER_TEXTURE_TYPE tex)
{
	glActiveTexture(GL_TEXTURE0 + (unsigned int)tex);
	mpFBO->GetColorTarget((unsigned int)tex)->Unbind();
}

void GeometryEngine::GeometryBuffer::GBuffer::copy(const GBuffer & ref)
{
	this->mFbo = 0;
	this->mDepthTexture = 0;
	this->mFinalTexture = 0;
	this->mTextureSize.setX(ref.mTextureSize.x());
	this->mTextureSize.setY(ref.mTextureSize.y());
	this->mMaxTextureSize.setX(ref.mMaxTextureSize.x());
	this->mMaxTextureSize.setY(ref.mMaxTextureSize.y());
	this->mFinalTextureLocation = 0;

	this->mpFBO = nullptr;

	for (int i = 0; i < GBUFFER_NUM_TEXTURES; ++i)
	{
		this->mTextures[i] = 0;
	}

	for (auto it = ref.mActiveTextures.begin(); it != ref.mActiveTextures.end(); ++it)
	{
		this->mActiveTextures.insert((*it));
	}
}

void GeometryEngine::GeometryBuffer::GBuffer::ClearColorTexture(GBUFFER_TEXTURE_TYPE texture)
{
	mpFBO->GetColorTarget((unsigned int)texture)->Clear();
}

void GeometryEngine::GeometryBuffer::GBuffer::DetachDepthBuffer()
{
	mpFBO->GetStencilDepthTexture()->Enable(false);
	mpFBO->GetStencilDepthBuffer()->Enable(true);
}

void GeometryEngine::GeometryBuffer::GBuffer::AttachDepthBuffer()
{
	mpFBO->GetStencilDepthBuffer()->Enable(false);
	mpFBO->GetStencilDepthTexture()->Enable(true);
}

void GeometryEngine::GeometryBuffer::GBuffer::FillGBufferInfo(GBufferTextureInfo & bufferInfo)
{
	// Indices of the texture units in which we store each texture
	bufferInfo.AmbientTexture = (unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_AMBIENT;
	bufferInfo.DiffuseTexture = (unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_DIFFUSE;
	bufferInfo.ReflectiveTexture = (unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_REFLECTIVE;
	bufferInfo.EmissiveTexture = (unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_EMMISSIVE;
	bufferInfo.PositionTexture = (unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_POSITION;
	bufferInfo.NormalTexture = (unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_NORMAL;
	bufferInfo.TmpTexture = (unsigned int)GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_TEXCOORD;
	bufferInfo.FinalTexture = mFinalTextureLocation;
	

	// Boolean values that indicate if each texture exists
	bufferInfo.UseAmbientTexture = this->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_AMBIENT);
	bufferInfo.UseDiffuseTexture = this->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	bufferInfo.UseReflectiveTexture = this->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_REFLECTIVE);
	bufferInfo.UseEmissiveTexture = this->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_EMMISSIVE);
	bufferInfo.UsePositionTexture = this->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_POSITION);
	bufferInfo.UseNormalTexture = this->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_NORMAL);
	bufferInfo.UseTmpTexture = this->IsTextureActive(GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_TEXCOORD);
}
