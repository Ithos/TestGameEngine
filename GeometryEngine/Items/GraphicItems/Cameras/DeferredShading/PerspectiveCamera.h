#pragma once

#ifndef GEOMETRYPERSPECTIVECAMERA_H
#define GEOMETRYPERSPECTIVECAMERA_H

#include "..\DeferredShadingCamera.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryCamera
		{
			/// Camera that applies a perspective projection to the scene ("normal" camera)
			class PerspectiveCamera : public DeferredShadingCamera
			{
			public:
				/// Constructor. FAQ: Scaling the camera scales the whole coordinate system, be careful with it
				/// param gbuffer Geometric buffer to be copied into the camera
				/// param viewportSize Size of the viewport. The values indicate in order x position, yposition, width, height.
				/// param autoResize Indicates if the viewport should be resized when the window's size changes
				/// param zNear Nearest z coordinate to be rendered
				/// param zFar Farthest z coordinate to be rendered
				/// param pos Initial position of the item
				/// param rot Initial rotaion of the item
				/// param scale Initial scale to be applied to this item model
				/// param parent Pointer to this items parent item, nullptr if none.
				PerspectiveCamera(const GeometryBuffer::GBuffer& gbuffer, const QVector4D& viewportSize, GLdouble fovy = 45.0, GLdouble aspectRatio = 1.0, bool autoResize = true, GLdouble zNear = 0.1, GLdouble zFar = 30.0,
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f),
					WorldItem* parent = nullptr);
				/// Destructor
				virtual ~PerspectiveCamera() {};
				/// Sets the aspect ratio to be used by the projection
				/// param w Viewport width
				/// param h Vewport height
				void SetAspectRatio(int w, int h) { mAspectRatio = qreal(w) / qreal(h ? h : 1); }
				/// Sets the field of view angle and the z render limits
				/// param fovy Angle that the perspective frustrum opens 
				/// param zNear Nearest z plane to be rendered
				/// param zFar Farthest z plane to be rendered
				void SetProjection(GLdouble fovy, GLdouble zNear, GLdouble zFar) { mFoView = fovy; SetBoundaries(zNear, zFar); }
				/// Stes the Viewport size
				/// param size Viewport size
				virtual void SetViewport(const QVector4D& size) override { DeferredShadingCamera::SetViewport(size); SetAspectRatio(size.z(), size.w()); }

				/// Calculates the projection matrix of the camera as a perspective projection matrix
				virtual void CalculateProjectionMatrix() override;

			protected:

				GLdouble mFoView;
				GLdouble mAspectRatio;

			};
		}
	}
}

#endif