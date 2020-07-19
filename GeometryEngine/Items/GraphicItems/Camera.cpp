#include "../PostProcess/PostProcess.h"
#include "../Item Utils/Viewport.h"
#include "Render Utils\RenderStep.h"
#include "Render Utils\RenderBuffersData.h"
#include "Camera.h"

GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::Camera(const GeometryItemUtils::Viewport & viewport, bool autoResize, const QVector3D & pos, 
	const QVector3D & rot, const QVector3D & scale, WorldItem * parent) : WorldItem(pos, rot, scale, parent), mAutoResize(autoResize), mpGBufferData(nullptr), mpViewport(nullptr)
{
	mpViewport = viewport.Clone();
	CalculateModelMatrix();
}

GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::Camera(const Camera & ref) : WorldItem()
{
	copy(ref);
}

GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::~Camera()
{
	if (mpGBufferData != nullptr)
	{
		delete mpGBufferData;
		mpGBufferData = nullptr;
	}

	if (mpViewport != nullptr)
	{
		delete mpViewport;
		mpViewport = nullptr;
	}

	ClearCustomRenderSteps();
	ClearPostProcess();
}

const QMatrix4x4 & GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::GetViewMatrix()
{
	return mpViewport->GetViewMatrix();
}

inline const QMatrix4x4 & GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::GetProjectionMatrix()
{
	return mpViewport->GetProjectionMatrix(); 
}

inline const QMatrix4x4 & GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::GetViewProjectionMatrix()
{
	return mpViewport->GetViewProjectionMatrix();
}

const QVector4D & GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::GetViewportSize()
{
	return mpViewport->GetViewportSize(); 
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::UpdateModelMatrix(bool updateChildren)
{
	mModelMatrix = mpViewport->UpdateViewMatrix(GetPosition(), mRotation, mScale);

	if (updateChildren)
	{
		for (auto iter = mpChildren.begin(); iter != mpChildren.end(); ++iter)
		{
			(*iter)->UpdateModelMatrix(updateChildren);
		}
	}
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::SetViewportSize(const QVector4D & size)
{
	mpViewport->SetViewportSize(size);
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::SetBoundaries(GLdouble zNear, GLdouble zFar)
{
	mpViewport->SetBoundaries(zNear, zFar);
}

GeometryEngine::GeometryBuffer::GBuffer * GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::GetGBuffer()
{
	if (mpGBufferData != nullptr) return mpGBufferData->GetGeometryBuffer();
	return nullptr;
}

bool GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::AddCustomRenderStep(const GeometryRenderStep::RenderStep & step)
{
	mCustomRenderSteps.push_back(step.Clone());
	return true;
}

bool GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::RemoveCustomRenderStep(int pos)
{
	if (mCustomRenderSteps.size() >= pos)
		return false;

	std::list<GeometryRenderStep::RenderStep* >::iterator it = mCustomRenderSteps.begin();
	std::advance(it, pos);
	mCustomRenderSteps.remove((*it));
	return true;
}

bool GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::InsertCustomRenderStep(const GeometryRenderStep::RenderStep & step, unsigned int pos)
{
	if (pos >= mCustomRenderSteps.size())
		return false;

	std::list<GeometryRenderStep::RenderStep* >::iterator it = mCustomRenderSteps.begin();
	std::advance(it, pos);
	mCustomRenderSteps.emplace(it, step.Clone());
	return true;
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::ClearCustomRenderSteps()
{
	for (auto iter = mCustomRenderSteps.begin(); iter != mCustomRenderSteps.end(); ++iter)
	{
		GeometryRenderStep::RenderStep* step = (*iter);
		delete(step);
	}
	mCustomRenderSteps.clear();
}

bool GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::AddPostProcess(const GeometryPostProcess::PostProcess & process)
{
	mPostProcess.push_back(process.Clone());
	return true;
}

bool GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::RemovePostProcess(int pos)
{
	if (mPostProcess.size() >= pos)
		return false;

	std::list<GeometryPostProcess::PostProcess* >::iterator it = mPostProcess.begin();
	std::advance(it, pos);
	mPostProcess.remove((*it));
	return true;
}

bool GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::InsertPostProcess(const GeometryPostProcess::PostProcess & process, unsigned int pos)
{
	if (pos >= mPostProcess.size())
		return false;

	std::list<GeometryPostProcess::PostProcess* >::iterator it = mPostProcess.begin();
	std::advance(it, pos);
	mPostProcess.emplace(it, process.Clone());
	return true;
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::ClearPostProcess()
{
	for (auto iter = mPostProcess.begin(); iter != mPostProcess.end(); ++iter)
	{
		GeometryPostProcess::PostProcess* process = (*iter);
		delete(process);
	}
	mPostProcess.clear();
}

bool GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::AddRenderGroup(int group)
{
	if (mRenderGroups.find(group) != mRenderGroups.end())
		return false;
	mRenderGroups.insert(group);
	return true;
}

bool GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::RemoveRenderGroup(int group)
{
	if (mRenderGroups.find(group) == mRenderGroups.end())
		return false;
	mRenderGroups.erase(group);
	return true;
}

GeometryEngine::GeometryWorldItem::GeometryCamera::Camera * GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::Clone() const
{
	return new Camera( (*this) );
}

void GeometryEngine::GeometryWorldItem::GeometryCamera::Camera::copy(const Camera & ref)
{
	WorldItem::Copy(ref);
	
	mpViewport = ref.mpViewport->Clone();

	this->mAutoResize = ref.mAutoResize;

	for (auto it = ref.mCustomRenderSteps.begin(); it != ref.mCustomRenderSteps.end(); ++it)
	{
		AddCustomRenderStep( *(*it) );
	}

	for (auto it = ref.mPostProcess.begin(); it != ref.mPostProcess.end(); ++it)
	{
		AddPostProcess( *(*it) );
	}

	for (auto it = ref.mRenderGroups.begin(); it != ref.mRenderGroups.end(); ++it)
	{
		AddRenderGroup( (*it) );
	}
}
