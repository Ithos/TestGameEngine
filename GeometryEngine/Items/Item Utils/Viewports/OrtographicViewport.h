#pragma once

#ifndef GEOMETRYORTOGRPHICVIEWPORT_H
#define GEOMETRYORTOGRPHICVIEWPORT_H

#include "../Viewport.h"

namespace GeometryEngine
{
	namespace GeometryItemUtils
	{
		class OrtographicViewport : public Viewport
		{
		public:
			/// Constructor. 
			/// \param viewportSize Size of the viewport. The values indicate in order x position, yposition, width, height.
			/// \param orthoSize size of the ortographic prism
			/// \param zNear Nearest z coordinate to be rendered
			/// \param zFar Farthest z coordinate to be rendered
			OrtographicViewport(const QVector4D& viewportSize, const QRect& orthoSize, GLdouble zNear = 0.1, GLdouble zFar = 30.0);
			/// Copy constructor
			/// \param ref Object to be copied.
			OrtographicViewport(const OrtographicViewport& ref);
			/// Destructor
			virtual ~OrtographicViewport() {};
			/// Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual OrtographicViewport* Clone() const;
			/// Calculates the projection matrix for the camera as an orthographic projection matrix
			virtual void CalculateProjectionMatrix() override;
			/// Sets the size of the orthographic prism used to render the scene
			virtual void SetOrthoSize(const QRect& orthoSize) { mOrthoSize = QRect(orthoSize); }
		protected:
			QRect mOrthoSize;

			/// Copies the data of a Camera object to the current object
			/// \param ref OrtographicViewport to be copied
			virtual void copy(const OrtographicViewport& ref);
		};
	}
}

#endif