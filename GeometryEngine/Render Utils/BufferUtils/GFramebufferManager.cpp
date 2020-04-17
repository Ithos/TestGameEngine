#include "GFramebufferObject.h"
#include "GFramebufferManager.h"

GeometryEngine::GeometryBuffer::GFramebufferManager* GeometryEngine::GeometryBuffer::GFramebufferManager::mpInstance = nullptr;

GeometryEngine::GeometryBuffer::GFramebufferManager * GeometryEngine::GeometryBuffer::GFramebufferManager::GetInstance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new GFramebufferManager();
	}
	return mpInstance;
}

void GeometryEngine::GeometryBuffer::GFramebufferManager::Release()
{
	if (mpInstance != nullptr)
	{
		mpInstance->close();
		delete mpInstance;
		mpInstance = nullptr;
	}
}

void GeometryEngine::GeometryBuffer::GFramebufferManager::AddFramebuffer(GFramebufferObject * fbo)
{
	auto it = mFramebufferList.find(fbo);
	if (it == mFramebufferList.end())
	{
		mFramebufferList.insert(fbo);
	}
}

void GeometryEngine::GeometryBuffer::GFramebufferManager::DeleteFramebuffer(GFramebufferObject * fbo)
{
	auto it = mFramebufferList.find(fbo);
	if (it != mFramebufferList.end())
	{
		mFramebufferList.erase(fbo);
		delete fbo;
	}
}

void GeometryEngine::GeometryBuffer::GFramebufferManager::SetFramebufferActive(GFramebufferObject * fbo)
{
	if (fbo != mpActiveFramebuffer)
	{
		mpActiveFramebuffer = fbo;
		for (auto iter = mFramebufferList.begin(); iter != mFramebufferList.end(); ++iter)
		{
			if ((*iter) != mpActiveFramebuffer)
			{
				(*iter)->Unbind();
			}
		}
	}
}

GeometryEngine::GeometryBuffer::GFramebufferObject * GeometryEngine::GeometryBuffer::GFramebufferManager::GetNewFramebuffer()
{
	return new GFramebufferObject(this);
}

GeometryEngine::GeometryBuffer::GFramebufferManager::GFramebufferManager()
{
	init();
}

GeometryEngine::GeometryBuffer::GFramebufferManager::~GFramebufferManager()
{
	for (auto iter = mFramebufferList.begin(); iter != mFramebufferList.end(); ++iter)
	{
		delete (*iter);
	}

	mFramebufferList.clear();
}

void GeometryEngine::GeometryBuffer::GFramebufferManager::close()
{
}

void GeometryEngine::GeometryBuffer::GFramebufferManager::init()
{
	mpActiveFramebuffer = nullptr;
}
