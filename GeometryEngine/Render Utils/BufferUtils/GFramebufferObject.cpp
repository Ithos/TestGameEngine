#include "GFramebufferManager.h"
#include "GFramebufferObject.h"

GeometryEngine::GeometryBuffer::GFramebufferObject::GFramebufferObject(GFramebufferManager* owner) : mFramebufferState(G_FRAMEBUFFER_BINDS::DETACH), mpDepthBuffer(nullptr), mpDepthTexture(nullptr),
	mpManager(owner), mFramebufferIndex(0)
{
	initializeOpenGLFunctions();
	glGenFramebuffers(1, &mFramebufferIndex);
	mpManager->AddFramebuffer(this);
}

GeometryEngine::GeometryBuffer::GFramebufferObject::~GFramebufferObject()
{
	if (mpDepthBuffer != nullptr) {
		delete mpDepthBuffer;
		mpDepthBuffer = nullptr;
	}

	if (mpDepthTexture != nullptr) {
		delete mpDepthTexture;
		mpDepthTexture = nullptr;
	}

	for (auto it = mColorAttachements.begin(); it != mColorAttachements.end(); ++it)
	{
		delete (*it);
	}

	mColorAttachements.clear();

	if (mFramebufferIndex != 0) {
		glDeleteFramebuffers(1, &mFramebufferIndex);
	}

	mpManager->DeleteFramebuffer(this);
}

bool GeometryEngine::GeometryBuffer::GFramebufferObject::AddTexture(GFramebufferCommons::G_COLOR_ATTACHMENTS attachment)
{
	if (!IsBound()) return false;
	mColorAttachements.push_back(new GTexture(attachment, this));
	return true;
}

bool GeometryEngine::GeometryBuffer::GFramebufferObject::AddTexture(const QVector2D & textureSize, GFramebufferCommons::G_COLOR_ATTACHMENTS attachment)
{
	if (!IsBound()) return false;
	AddTexture(textureSize, QVector3D(1.0f, 1.0f, 1.0f), attachment);
	return true;
}

bool GeometryEngine::GeometryBuffer::GFramebufferObject::AddTexture(const QVector2D & textureSize, const QVector3D & borderColor, GFramebufferCommons::G_COLOR_ATTACHMENTS attachment)
{
	if (!IsBound()) return false;
	mColorAttachements.push_back(new GTexture(textureSize, borderColor, attachment, this));
	return true;
}

bool GeometryEngine::GeometryBuffer::GFramebufferObject::AddBuffer(const QVector2D & bufferSize, GFramebufferCommons::G_COLOR_ATTACHMENTS attachment)
{
	if (!IsBound()) return false;
	mColorAttachements.push_back(new GRenderBuffer(bufferSize, attachment, this));
	return true;
}

bool GeometryEngine::GeometryBuffer::GFramebufferObject::SetStencilDepthTexture(const QVector2D & textureSize, GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS attachment)
{
	if (!IsBound()) return false;
	if (mpDepthTexture != nullptr) delete mpDepthTexture;
	mpDepthTexture = new GDepthStencilTexture(textureSize, attachment, this);
	return true;
}

bool GeometryEngine::GeometryBuffer::GFramebufferObject::SetStencilDepthBuffer(const QVector2D & bufferSize, GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS attachment)
{
	if (!IsBound()) return false;
	if (mpDepthBuffer != nullptr) delete mpDepthBuffer;
	mpDepthBuffer = new GDepthStencilRenderBuffer(bufferSize, attachment, this);
	return true;
}

GeometryEngine::GeometryBuffer::GRenderTarget * GeometryEngine::GeometryBuffer::GFramebufferObject::GetColorTarget(unsigned int index)
{
	if (!IsBound()) return nullptr;
	if(mColorAttachements.size() <= index) return nullptr;
	auto it = mColorAttachements.begin();
	std::advance(it, index);
	return (*it);
}

void GeometryEngine::GeometryBuffer::GFramebufferObject::Bind(G_FRAMEBUFFER_BINDS bind)
{
	if (bind != G_FRAMEBUFFER_BINDS::DETACH)
	{
		mpManager->SetFramebufferActive(this);
		glBindFramebuffer(static_cast<GLenum>(bind), mFramebufferIndex);
	}
	else
	{
		glBindFramebuffer(static_cast<GLenum>(mFramebufferState), 0);
	}
	
	mFramebufferState = bind;
}

void GeometryEngine::GeometryBuffer::GFramebufferObject::Unbind()
{
	Bind(G_FRAMEBUFFER_BINDS::DETACH);
}

bool GeometryEngine::GeometryBuffer::GFramebufferObject::Draw(unsigned int index)
{
	if (!IsDrawState()) return false;

	GRenderTarget* target = GetColorTarget(index);
	if(target == nullptr) return false;
	glDrawBuffer(target->GetAttachment());
	return true;
}

bool GeometryEngine::GeometryBuffer::GFramebufferObject::Draw(unsigned int* indices, unsigned int lenght)
{
	if (!IsDrawState()) return false;

	GLenum* DrawBuffers = new GLenum[lenght];

	for (unsigned int i = 0; i < lenght; ++i)
	{
		GRenderTarget* target = GetColorTarget(indices[i]);
		if (target == nullptr)
		{
			delete[] DrawBuffers;
			return false;
		}

		DrawBuffers[i] = target->GetAttachment();
	}

	glDrawBuffers(lenght, DrawBuffers);
	delete[] DrawBuffers;
	return true;
}

bool GeometryEngine::GeometryBuffer::GFramebufferObject::DisableDraw()
{
	if (!IsDrawState()) return false;
	glDrawBuffer(GL_NONE);
	return true;
}

bool GeometryEngine::GeometryBuffer::GFramebufferObject::Read(unsigned int index)
{
	if (!IsReadState()) return false;

	GRenderTarget* target = GetColorTarget(index);
	if (target == nullptr) return false;
	glReadBuffer(target->GetAttachment());
	return true;
}

bool GeometryEngine::GeometryBuffer::GFramebufferObject::DisableRead()
{
	if (!IsReadState()) return false;
	glReadBuffer(GL_NONE);
	return true;
}
