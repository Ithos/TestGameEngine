#include "GBuffer.h"

GeometryEngine::GBuffer::GBuffer() : mFbo(0), mDepthTexture(0), mFinalTexture(0)
{
	for (int i = 0; i < GBUFFER_NUM_TEXTURES; ++i)
	{
		mTextures[i] = 0;
	}

	initializeOpenGLFunctions();
}

GeometryEngine::GBuffer::~GBuffer()
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

bool GeometryEngine::GBuffer::Init(unsigned int MaxWindowWidth, unsigned int MaxWindowHeight)
{
	mMaxTextureSize.setX(MaxWindowWidth); mMaxTextureSize.setY(MaxWindowHeight);
	// Create the FBO
	glGenFramebuffers(1, &mFbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

	// Create the gbuffer textures
	glGenTextures(GBUFFER_NUM_TEXTURES, mTextures);
	glGenTextures(1, &mDepthTexture);
	glGenTextures(1, &mFinalTexture);

	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		glBindTexture(GL_TEXTURE_2D, mTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, MaxWindowWidth, MaxWindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mTextures[i], 0);
	}
	
	// depth
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, MaxWindowWidth, MaxWindowHeight, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);

	// final
	glBindTexture(GL_TEXTURE_2D, mFinalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MaxWindowWidth, MaxWindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, mFinalTexture, 0);


	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(GBUFFER_NUM_TEXTURES, DrawBuffers);

	mTextureSize.setX(MaxWindowWidth); mTextureSize.setY(MaxWindowHeight);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

bool GeometryEngine::GBuffer::Resize(unsigned int WindowWidth, unsigned int WindowHeight)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);


	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		glBindTexture(GL_TEXTURE_2D, mTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	}

	// depth
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, WindowWidth, WindowHeight, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);

	// final
	glBindTexture(GL_TEXTURE_2D, mFinalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);

	mTextureSize.setX(WindowWidth); mTextureSize.setY(WindowHeight);

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

void GeometryEngine::GBuffer::BindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

	/*for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mTextures[GBUFFER_TEXTURE_TYPE_DIFFUSE + i]);
	}*/
}

void GeometryEngine::GBuffer::BindForReading()
{
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, mFbo);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mTextures[GBUFFER_TEXTURE_TYPE_DIFFUSE + i]);
	}
}

void GeometryEngine::GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}

void GeometryEngine::GBuffer::StartFrame()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GeometryEngine::GBuffer::BindForGeomPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);

	GLenum DrawBuffers[] = { 
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2, 
		GL_COLOR_ATTACHMENT3};

	glDrawBuffers(GBUFFER_NUM_TEXTURES, DrawBuffers);
}

void GeometryEngine::GBuffer::BindForStencilPass()
{
	// must disable the draw buffers 
	glDrawBuffer(GL_NONE);
}

void GeometryEngine::GBuffer::BindForLightPass()
{
	glDrawBuffer(GL_COLOR_ATTACHMENT4);

	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mTextures[GBUFFER_TEXTURE_TYPE_DIFFUSE + i]);
	}
}

void GeometryEngine::GBuffer::BindForFinalPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mFbo);
	glReadBuffer(GL_COLOR_ATTACHMENT4);
}
