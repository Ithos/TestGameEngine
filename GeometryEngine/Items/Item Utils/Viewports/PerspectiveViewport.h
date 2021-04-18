#pragma once

#ifndef GEOMETRYPERSPECTIVEVIEWPORT_H
#define GEOMETRYPERSPECTIVEVIEWPORT_H

#include "../Viewport.h"

namespace GeometryEngine
{
	namespace GeometryItemUtils
	{
		class PerspectiveViewport : public Viewport
		{
		public:
			/// Constructor. 
			/// \param viewportSize Size of the viewport. The values indicate in order x position, yposition, width, height.
			/// \param fovy Angle that the perspective frustrum opens 
			/// \param aspectRatio Relation between width and height of the image
			/// \param zNear Nearest z coordinate to be rendered
			/// \param zFar Farthest z coordinate to be rendered
			PerspectiveViewport(const QVector4D& viewportSize, GLdouble fovy = 45.0, GLdouble aspectRatio = 1.0, GLdouble zNear = 0.1, GLdouble zFar = 30.0);

			/// Copy constructor
			/// \param ref Object to be copied.
			PerspectiveViewport(const PerspectiveViewport& ref);
			/// Destructor
			virtual ~PerspectiveViewport() {};
			/// Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual PerspectiveViewport* Clone() const override;
			/// Sets the aspect ratio to be used by the projection
			/// \param w Viewport width
			/// \param h Vewport height
			void SetAspectRatio(int w, int h) { mAspectRatio = qreal(w) / qreal(h ? h : 1); }
			/// Sets the field of view angle and the z render limits
			/// \param fovy Angle that the perspective frustrum opens 
			/// \param zNear Nearest z plane to be rendered
			/// \param zFar Farthest z plane to be rendered
			void SetProjection(GLdouble fovy, GLdouble zNear, GLdouble zFar) { mFoView = fovy; SetBoundaries(zNear, zFar); }
			/// Sets the Viewport size
			/// \param size Viewport size
			virtual void SetViewportSize(const QVector4D& size) override { Viewport::SetViewportSize(size); SetAspectRatio(size.z(), size.w()); }

			/// Calculates the projection matrix of the camera as a perspective projection matrix
			virtual void CalculateProjectionMatrix() override;

		protected:

			GLdouble mFoView;
			GLdouble mAspectRatio;
			/// Copies the data of a Camera object to the current object
			/// \param ref PerspectiveViewport to be copied
			virtual void copy(const PerspectiveViewport& ref);
		};
	}
}


#endif