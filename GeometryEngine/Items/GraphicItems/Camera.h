#pragma once

#ifndef GEOMETRYCAMERA_H
#define GEOMETRYCAMERA_H

#include <list>

#include "../WorldItem.h"
#include "Render Utils\GBuffer.h"
#include "../PostProcess/PostProcess.h"


namespace GeometryEngine
{
	namespace GeometryRenderStep
	{
		class RenderStep;
	}

	namespace GeometryBuffer
	{
		class GBuffer;
	}

	namespace GeometryPostProcess
	{
		class PostProcess;
	}

	namespace GeometryWorldItem
	{
		namespace GeometryCamera
		{
			class Camera : public WorldItem
			{
			public:
				// FAQ: Scaling the camera scales the whole coordinate system, be careful with it
				Camera(const QVector4D& viewportSize, bool autoResize = true, GLdouble zNear = 0.1, GLdouble zFar = 30.0, const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);
				virtual ~Camera();

				virtual const QMatrix4x4& GetProjectionMatrix() { return mProjection; }
				virtual const QMatrix4x4& GetViewMatrix() { return mModelMatrix; }
				virtual const QMatrix4x4& GetViewProjectionMatrix() { return mViewProjection; }
				virtual void SetViewport(const QVector4D& size) { mViewportSize.setX(size.x()); mViewportSize.setY(size.y()); mViewportSize.setZ(size.z()); mViewportSize.setW(size.w()); }
				void SetBoundaries(GLdouble zNear, GLdouble zFar) { mZNear = zNear; mZFar = zFar; }
				virtual void CalculateProjectionMatrix() = 0;
				virtual void UpdateModelMatrix(bool updateChildren = false) override;
				virtual const QVector4D& GetViewportSize() { return mViewportSize; }
				bool IsAutoResize() { return mAutoResize; }
				virtual GeometryBuffer::GBuffer* GetGBuffer() { return mpGBuffer; }

				virtual bool AddCustomRenderStep(const GeometryRenderStep::RenderStep& step);
				virtual bool RemoveCustomRenderStep(int pos);
				virtual bool InsertCustomRenderStep(const GeometryRenderStep::RenderStep& step, unsigned int pos);
				virtual void ClearCustomRenderSteps();
				virtual const std::list< GeometryRenderStep::RenderStep* >& GetCustomRenderSteps() { return mCustomRenderSteps; }

				virtual bool AddPostProcess(const GeometryPostProcess::PostProcess& process);
				virtual bool RemovePostProcess(int pos);
				virtual bool InsertPostProcess(const GeometryPostProcess::PostProcess& process, unsigned int pos);
				virtual void ClearPostProcess();
				virtual const std::list< GeometryPostProcess::PostProcess* >& GetPostProcess() { return mPostProcess; }

				virtual bool AddRenderGroup(int group);
				virtual bool RemoveRenderGroup(int group);
				virtual const std::unordered_set<int>& GetRenderGroups() { return mRenderGroups; }
				virtual void ClearRenderGroups() { mRenderGroups.clear(); }

			protected:
				GLdouble mZNear;
				GLdouble mZFar;
				QVector4D mViewportSize;
				QMatrix4x4 mProjection;
				QMatrix4x4 mViewProjection;
				bool mAutoResize;
				GeometryBuffer::GBuffer* mpGBuffer;
				std::list< GeometryRenderStep::RenderStep* > mCustomRenderSteps;
				std::list< GeometryPostProcess::PostProcess*> mPostProcess;
				virtual void ResetCameraBeforeCalculation();
				virtual void ApplyCameraModelMatrix() { mViewProjection = mProjection * mModelMatrix; };
				std::unordered_set<int> mRenderGroups;
			};
		}
	}
}

#endif