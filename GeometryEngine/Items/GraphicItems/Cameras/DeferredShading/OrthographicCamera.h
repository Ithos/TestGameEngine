#pragma once

#ifndef GEOMETRYORTHOGRAPHICCAMERA_H
#define GEOMETRYORTHOGRAPHICCAMERA_H

#include "..\DeferredShadingCamera.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryCamera
		{
			/// Camera that applies an ortographic projection to the scene
			class OrthographicCamera : public DeferredShadingCamera
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
				OrthographicCamera(const GeometryBuffer::GBuffer& gbuffer, const QVector4D& viewportSize, const QRect& orthoSize, bool autoResize = true, GLdouble zNear = 0.1, GLdouble zFar = 30.0,
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f),
					WorldItem* parent = nullptr);

				/// Destructor
				~OrthographicCamera() {};

				/// Calculates the projection matrix for the camera as an orthographic projection matrix
				virtual void CalculateProjectionMatrix() override;

				/// Sets the size of the orthographic prism used to render the scene
				virtual void SetOrthoSize(const QRect& orthoSize) { mOrthoSize = QRect(orthoSize); }

			protected:
				QRect mOrthoSize;
			};
		}
	}
}

#endif
