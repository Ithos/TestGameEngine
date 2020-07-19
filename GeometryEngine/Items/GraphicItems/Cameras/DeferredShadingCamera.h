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
				/// param gbuffer Geometry buffer to be copied into the camera
				/// param viewportSize Size of the viewport. The values indicate in order x position, yposition, width, height.
				/// param autoResize Indicates if the viewport should be resized when the window's size changes
				/// param zNear Nearest z coordinate to be rendered
				/// param zFar Farthest z coordinate to be rendered
				/// param pos Initial position of the item
				/// param rot Initial rotaion of the item
				/// param scale Initial scale to be applied to this item model
				/// param parent Pointer to this items parent item, nullptr if none.
				DeferredShadingCamera( const GeometryRenderData::RenderBuffersData& gbuffer, const GeometryItemUtils::Viewport& viewport, bool autoResize = true, const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);
				/// Copy constructor
				/// param ref Object to be copied.
				DeferredShadingCamera(const DeferredShadingCamera& ref);
				/// Destructor
				virtual ~DeferredShadingCamera() {}
				/// Resizes the Opengl viewport and the Geometry buffer viewport size
				/// param size Viewport size
				virtual void SetViewportSize(const QVector4D& size) override;
				/// Factory method. Creates a copy of this object
				/// return Pointer to a copy of this object
				virtual DeferredShadingCamera* Clone() const;
				/// Gets a pointer to the geometry buffer
				/// return geometry buffer
				virtual GeometryBuffer::GBuffer* GetGBuffer();

			protected:
				/// Clones the Geometry buffer into the camera and initializes the buffer textures with the size of the screen
				/// param gBuffer Geometry buffer to be cloned into the camera
				virtual void init(const GeometryRenderData::RenderBuffersData& gbuffer);

				/// Copies the data of a Camera object to the current object
				/// param ref Camera to be copied
				virtual void copy(const DeferredShadingCamera& ref);
			};
		}
	}
}
#endif