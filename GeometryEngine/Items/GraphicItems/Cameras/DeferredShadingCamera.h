#pragma once

#ifndef DEFERREDSHADINGCAMERA_H
#define DEFERREDSHADINGCAMERA_H

#include <qapplication.h>
#include <qdesktopwidget.h>
#include "../Camera.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryCamera
		{
			/// Camera that stores a GBuffer to allow multiple step shading
			class DeferredShadingCamera : public Camera
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
				DeferredShadingCamera( const GeometryBuffer::GBuffer& gbuffer, const QVector4D& viewportSize, bool autoResize = true, GLdouble zNear = 0.1, 
					GLdouble zFar = 30.0, const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);
				/// Destructor
				virtual ~DeferredShadingCamera() {}
				/// Resizes the Opengl viewport and the Geometric buffer viewport size
				/// param size Viewport size
				virtual void SetViewport(const QVector4D& size) override;

			protected:
				/// Clones the Geometric buffer into the camera and initializes the buffer textures with the size of the screen
				/// param gBuffer Geometric buffer to be cloned into the camera
				virtual void init( const GeometryBuffer::GBuffer& gbuffer);
			};
		}
	}
}
#endif