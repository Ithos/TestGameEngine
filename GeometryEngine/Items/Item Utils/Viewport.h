#pragma once

#ifndef GEOMETRYVIEWPORT_H
#define GEOMETRYVIEWPORT_H

#include <QVector3D>
#include <QVector2D>
#include <QQuaternion>
#include <QMatrix4x4>
#include <qopengl.h>

namespace GeometryEngine
{
	/// Namespace for classes to be used by world items as helper classes to perform certain functionality and for architectural reasons
	namespace GeometryItemUtils
	{
		class Viewport
		{
		public:
			/// Constructor. 
			/// param viewportSize Size of the viewport. The values indicate in order x position, yposition, width, height.
			/// param zNear Nearest z coordinate to be rendered
			/// param zFar Farthest z coordinate to be rendered
			Viewport(const QVector4D& viewportSize, GLdouble zNear = 0.1, GLdouble zFar = 30.0);

			/// Copy constructor
			/// param ref Object to be copied.
			Viewport(const Viewport& ref);
			/// Destructor
			virtual ~Viewport();
			/// Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual Viewport* Clone() const = 0;
			/// Gets the View matrix
			/// return View Matrix
			virtual const QMatrix4x4& GetViewMatrix() { return mViewMatrix; }
			/// Gets the Projection matrix
			/// return projection matrix
			virtual const QMatrix4x4& GetProjectionMatrix() { return mProjection; }
			/// Gets the View projection matrix
			/// return View projection matrix
			virtual const QMatrix4x4& GetViewProjectionMatrix() { return mViewProjection; }
			/// Sets a new size for the viewport, but does not update the viewport inmediately
			/// param size New Viewport size. The values indicate in order x position, yposition, width, height.
			virtual void SetViewportSize(const QVector4D& size) { mViewportSize.setX(size.x()); mViewportSize.setY(size.y()); mViewportSize.setZ(size.z()); mViewportSize.setW(size.w()); }
			/// Sets new values for the z coordinate limits, but does not update the projection matrix inmediately
			/// param zNear Nearest z coordinate to be rendered
			/// param zFar Farthest z coordinate to be rendered
			void SetBoundaries(GLdouble zNear, GLdouble zFar) { mZNear = zNear; mZFar = zFar; }
			/// Abstract method. Calculates the projection matrix for the camera.
			virtual void CalculateProjectionMatrix() = 0;
			/// Updates the model matrix for the camera
			/// param pos position where the viewport should be calculated
			/// param rot rotation ofthe viewport
			/// parm scale scale to be applied to the objects being represented by the viewport
			virtual const QMatrix4x4& UpdateViewMatrix(const QVector3D& pos, const QQuaternion & rot, const QVector3D & scale);
			/// Get the current viewport size. Gets the stored viewport size but not the actual viewport size, if there are unnaplied changes the actual size may differ.
			/// return Viewport size as a QVector4D. The values indicate in order x position, yposition, width, height.
			virtual const QVector4D& GetViewportSize() { return mViewportSize; }

		protected:
			GLdouble mZNear;
			GLdouble mZFar;
			QVector4D mViewportSize;
			QMatrix4x4 mProjection;
			QMatrix4x4 mViewProjection;
			QMatrix4x4 mViewMatrix;

			/// Default constructor to be used by child classes when being copied
			Viewport() {}

			/// Sets projection matrix to identity and updates the size of the actual OpenGl viewport
			virtual void ResetCameraBeforeCalculation();
			/// Calculates the view projection matrix for the camera by multiplying projection by model matrices
			virtual void ApplyCameraModelMatrix() { mViewProjection = mProjection * mViewMatrix; };

			/// Copies the data of a Camera object to the current object
			/// param ref Viewport to be copied
			virtual void copy(const Viewport& ref);
		};
	}
}
#endif