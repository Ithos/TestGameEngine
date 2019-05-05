#include "GBuffer.h"

GeometryEngine::GeometryBuffer::GBuffer::GBuffer() : mFbo(0), mDepthTexture(0), mFinalTexture(0), mFinalTextureLocation(0)
{
	for (int i = 0; i < GBUFFER_NUM_TEXTURES; ++i)
	{
		mTextures[i] = 0;
	}

	initializeOpenGLFunctions();
}

GeometryEngine::GeometryBuffer::GBuffer::GBuffer(const GBuffer & ref)
{
	copy(ref);
}

GeometryEngine::GeometryBuffer::GBuffer::~GBuffer()
{
	if (mFbo != 0) {
		glDeleteFramebuffers(1, &mFbo);
	}

	if (mTextures[0] != 0) {
		glDeleteTextures(GBUFFER_NUM_TEXTURES, mTextures);
	}

	if (mDepthTexture != 0) {
		glDeleteTextures(1, &mDepthTexture);
	}

	if (mFinalTexture != 0)
	{
		glDeleteTextures(1, &mFinalTexture);
	}
}

bool GeometryEngine::GeometryBuffer::GBuffer::Init(unsigned int MaxWindowWidth, unsigned int MaxWindowHeight)
{
	mMaxTextureSize.setX(MaxWindowWidth); mMaxTextureSize.setY(MaxWindowHeight);
	// Create the FBO
	glGenFramebuffers(1, &mFbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

	// Create the gbuffer textures
	glGenTextures(GBUFFER_NUM_TEXTURES, mTextures);
	glGenTextures(1, &mFinalTexture);
	glGenTextures(1, &mDepthTexture);
	

	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		mActiveTextures.find((GBUFFER_TEXTURE_TYPE)i) != mActiveTextures.end() ? 
			generateTexture(i, MaxWindowWidth, MaxWindowHeight) : 
			generateNullTexture(i);
	}
	
	// final
	glBindTexture(GL_TEXTURE_2D, mFinalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, MaxWindowWidth, MaxWindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + GBUFFER_NUM_TEXTURES, GL_TEXTURE_2D, mFinalTexture, 0);

	

	// depth
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, MaxWindowWidth, MaxWindowHeight, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);

	mTextureSize.setX(MaxWindowWidth); mTextureSize.setY(MaxWindowHeight);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool GeometryEngine::GeometryBuffer::GBuffer::Resize(unsigned int WindowWidth, unsigned int WindowHeight)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);


	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		if (mActiveTextures.find((GBUFFER_TEXTURE_TYPE)i) != mActiveTextures.end())
		{
			glBindTexture(GL_TEXTURE_2D, mTextures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		}
	}

	// final
	glBindTexture(GL_TEXTURE_2D, mFinalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);

	// depth
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, WindowWidth, WindowHeight, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);

	mTextureSize.setX(WindowWidth); mTextureSize.setY(WindowHeight);

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void GeometryEngine::GeometryBuffer::GBuffer::StartFrame()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT0 + GBUFFER_NUM_TEXTURES);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindForGeomPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

	GLenum DrawBuffers[] = { 
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2, 
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,
		GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6
	};

	glDrawBuffers(GBUFFER_NUM_TEXTURES, DrawBuffers);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindForStencilPass()
{
	// must disable the draw buffers 
	glDrawBuffer(GL_NONE);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindForLightPass()// Buffer to final
{
	glDrawBuffer(GL_COLOR_ATTACHMENT0 + GBUFFER_NUM_TEXTURES);
	BindBuffer();
}

void GeometryEngine::GeometryBuffer::GBuffer::BindForFinalPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mFbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + GBUFFER_NUM_TEXTURES);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindForPostProcess()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + GBUFFER_NUM_TEXTURES);
	glDrawBuffer(GL_COLOR_ATTACHMENT0 + GBUFFER_TEXTURE_TYPE_TEXCOORD);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindForFinishPostProcess()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + GBUFFER_TEXTURE_TYPE_TEXCOORD);
	glDrawBuffer(GL_COLOR_ATTACHMENT0 + GBUFFER_NUM_TEXTURES);
}

void GeometryEngine::GeometryBuffer::GBuffer::BindBuffer()
{
	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mTextures[i]);
	}
}

void GeometryEngine::GeometryBuffer::GBuffer::BindFinalTexture(GBUFFER_TEXTURE_TYPE location)
{
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_2D, mFinalTexture);

	mFinalTextureLocation = location;
}

void GeometryEngine::GeometryBuffer::GBuffer::BindFinalTexture(GBUFFER_TEXTURE_TYPE location, GBufferTextureInfo & bufferInfo)
{
	BindFinalTexture(location);
	bufferInfo.FinalTexture = mFinalTextureLocation;
}

void GeometryEngine::GeometryBuffer::GBuffer::ResetBindings()
{
	for (unsigned int i = 0; i <= GBUFFER_NUM_TEXTURES; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
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
	glBindTexture(GL_TEXTURE_2D, 0);
	mFinalTextureLocation = 0;
}

void GeometryEngine::GeometryBuffer::GBuffer::BindTexture(GBUFFER_TEXTURE_TYPE tex)
{
	glActiveTexture(GL_TEXTURE0 + (unsigned int)tex);
	glBindTexture(GL_TEXTURE_2D, mTextures[tex]);
}

void GeometryEngine::GeometryBuffer::GBuffer::UnbindTexture(GBUFFER_TEXTURE_TYPE tex)
{
	glActiveTexture(GL_TEXTURE0 + (unsigned int)tex);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GeometryEngine::GeometryBuffer::GBuffer::generateTexture(unsigned int arrayIndex, unsigned int maxWindowWidth, unsigned int maxWindowHeight)
{
	glBindTexture(GL_TEXTURE_2D, mTextures[arrayIndex]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, maxWindowWidth, maxWindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + arrayIndex, GL_TEXTURE_2D, mTextures[arrayIndex], 0);
}

void GeometryEngine::GeometryBuffer::GBuffer::generateNullTexture(unsigned int arrayIndex)
{
	glBindTexture(GL_TEXTURE_2D, mTextures[arrayIndex]);
	GLubyte data[] = { 255, 255, 255, 255 };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + arrayIndex, GL_TEXTURE_2D, mTextures[arrayIndex], 0);
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

	for (int i = 0; i < GBUFFER_NUM_TEXTURES; ++i)
	{
		this->mTextures[i] = 0;
	}

	for (auto it = ref.mActiveTextures.begin(); it != ref.mActiveTextures.end(); ++it)
	{
		this->mActiveTextures.insert((*it));
	}
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
