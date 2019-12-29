#pragma once

#ifndef GEOMETRYCAMERA_H
#define GEOMETRYCAMERA_H

#include <list>

#include "../WorldItem.h"
#include "Render Utils\GBuffer.h"
#include "../PostProcess/PostProcess.h"
#include "../Item Utils/Viewport.h"


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
		///namespace for all cameras
		namespace GeometryCamera
		{
			/// Class that calculates projection matrices, and holds information for render steps, post processes and the geometry buffer
			class Camera : public WorldItem
			{
			public:
				/// Constructor.  FAQ: Scaling the camera scales the whole coordinate system, be careful with it
				/// param viewport Object that defines the view and projection matrices
				/// param pos Initial position of the item
				/// param rot Initial rotaion of the item
				/// param scale Initial scale to be applied to this item model
				/// param parent Pointer to this items parent item, nullptr if none.
				Camera(const GeometryItemUtils::Viewport& viewport, bool autoResize = true, const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);

				/// Copy constructor
				/// param ref Object to be copied.
				Camera(const Camera& ref);
				/// Destructor
				virtual ~Camera();
				/// Gets a pointer to the camera vewport
				/// return pointer to the camera viewport
				GeometryItemUtils::Viewport* GetViewport() { return mpViewport; }
				/// Gets the View matrix
				/// return View Matrix
				virtual const QMatrix4x4& GetViewMatrix() { return mpViewport->GetViewMatrix(); }
				/// Gets the Projection matrix
				/// return projection matrix
				virtual const QMatrix4x4& GetProjectionMatrix() { return mpViewport->GetProjectionMatrix(); }
				/// Gets the View projection matrix
				/// return View projection matrix
				virtual const QMatrix4x4& GetViewProjectionMatrix() { return mpViewport->GetViewProjectionMatrix(); }
				/// Get the current viewport size. Gets the stored viewport size but not the actual viewport size, if there are unnaplied changes the actual size may differ.
				/// return Viewport size as a QVector4D. The values indicate in order x position, yposition, width, height.
				virtual const QVector4D& GetViewportSize() { return mpViewport->GetViewportSize(); }
				/// Updates the model matrix for the camera
				/// param calculateChildren true if children matrix should be updated
				virtual void UpdateModelMatrix(bool updateChildren = false) override;
				/// Resizes the Opengl viewport
				/// param size Viewport size
				virtual void SetViewportSize(const QVector4D& size) { mpViewport->SetViewportSize(size); }
				/// Sets new values for the z coordinate limits, but does not update the projection matrix inmediately
				/// param zNear Nearest z coordinate to be rendered
				/// param zFar Farthest z coordinate to be rendered
				void SetBoundaries(GLdouble zNear, GLdouble zFar) { mpViewport->SetBoundaries(zNear, zFar); }

				/// Gets the auto resize value
				/// return Auto resize value
				bool IsAutoResize() { return mAutoResize; }
				/// Gets a pointer to the geometric buffer
				/// return geometric buffer
				virtual GeometryBuffer::GBuffer* GetGBuffer() { return mpGBuffer; }

				/// Adds a render step to this camera. If a camera has custom render steps they are used instead of the scene when rendering the camera.
				/// param step render step to be added to the camera
				/// return true if ok false otherwise
				virtual bool AddCustomRenderStep(const GeometryRenderStep::RenderStep& step);
				/// Removes a render step from the render step list.
				/// param pos Position in the list of the step to be removed
				/// return true if pos < list.size false otherwise
				virtual bool RemoveCustomRenderStep(int pos);
				/// Inserts a render step into a certain position within the render step list .If a camera has custom render steps they are used instead of the scene when rendering the camera.
				/// param step render step to be added to the camera
				/// param pos Position in the render step list where the step will be added
				/// return true if pos < list.size false otherwise 
				virtual bool InsertCustomRenderStep(const GeometryRenderStep::RenderStep& step, unsigned int pos);
				/// Removes all render steps from the render step list and deletes them.
				virtual void ClearCustomRenderSteps();
				/// Gets the Render step list.
				/// return Render step list.
				virtual const std::list< GeometryRenderStep::RenderStep* >& GetCustomRenderSteps() { return mCustomRenderSteps; }
				/// Adds a post process to this camera post process list. Post processes are contained exclusively in the camera instead of in the scene.
				/// param process Post process to be added
				/// return true if ok false otherwise
				virtual bool AddPostProcess(const GeometryPostProcess::PostProcess& process);
				/// Removes a post process from the post process list.
				/// param pos Position in the list of the step to be removed
				/// return true if pos < list.size false otherwise
				virtual bool RemovePostProcess(int pos);
				/// Inserts a post process into a certain position within the post process list. Post processes are contained exclusively in the camera instead of in the scene.
				/// param step post process to be added to the camera
				/// param pos Position in the render step list where the step will be added
				/// return true if pos < list.size false otherwise 
				virtual bool InsertPostProcess(const GeometryPostProcess::PostProcess& process, unsigned int pos);
				/// Removes all post process steps from the render step list and deletes them.
				virtual void ClearPostProcess();
				/// Gets the post process step list.
				/// return post process step list.
				virtual const std::list< GeometryPostProcess::PostProcess* >& GetPostProcess() { return mPostProcess; }

				/// Adds this camera to a render group. Cameras only render objects contained in the same render groups that they are. All objects start in the null render group.
				/// param group Identifier of the render group
				/// return bool true if the object was not already contained in the render group
				virtual bool AddRenderGroup(int group);
				/// Removes this camera from a render group. Cameras only render objects contained in the same render groups that they are. 
				/// All objects start in the null render group. An object cannot be removed from the null render group, if an object is not contained in any render group it's considered to be contained
				/// in the null render group.
				/// param group Identifier of the render group
				/// return bool true if the object was contained in the render group
				virtual bool RemoveRenderGroup(int group);
				/// Gets the set of render groups this object is contained in
				/// return set of render groups this object is contained in
				virtual const std::unordered_set<int>& GetRenderGroups() { return mRenderGroups; }
				/// Removes the camera from all render groups. An object is not contained in any render group it's considered to be contained in the null render group.
				virtual void ClearRenderGroups() { mRenderGroups.clear(); }

				/// Factory method. Creates a copy of this object
				/// return Pointer to a copy of this object
				virtual Camera* Clone() const;

			protected:
				GeometryItemUtils::Viewport* mpViewport;
				
				bool mAutoResize;
				GeometryBuffer::GBuffer* mpGBuffer;
				std::list< GeometryRenderStep::RenderStep* > mCustomRenderSteps;
				std::list< GeometryPostProcess::PostProcess*> mPostProcess;
				std::unordered_set<int> mRenderGroups;

				/// Default constructor to be used by child classes when being copied
				Camera() {}

				/// Copies the data of a Camera object to the current object
				/// param ref Camera to be copied
				virtual void copy(const Camera& ref);
			};
		}
	}
}

#endif