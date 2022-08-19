#pragma once

#ifndef TRANSPARENTGEOMETRYPASS_H
#define TRANSPARENTGEOMETRYPASS_H

#include "GeometryPass.h"

namespace GeometryEngine
{
	namespace GeometryRenderStep
	{
		/// Render step that renders geometry without illumination and separates the rendering of transparent and non transparent geometry.
		class TransparentGeometryPass : public GeometryPass
		{
		public:
			/// Constructor
			TransparentGeometryPass() : GeometryPass() {}
			/// Copy constructor
			/// \param ref Geometry pass to be copied
			TransparentGeometryPass(const TransparentGeometryPass& ref) { copy(ref); }
			/// Destructor
			virtual ~TransparentGeometryPass() {}
			/// Factory method. Returns a copy of this object.
			/// \return Acopy of this object.
			virtual TransparentGeometryPass* Clone() const override { return new TransparentGeometryPass(*this); }
		protected:
			/// Draws each item in the set using the camera transformation matrix
			/// \param cam Pointer to camera to be rendered
			/// \param orderedItems Set of items to be rendered ordered by distance to the camera
			void renderGeometry(GeometryWorldItem::GeometryCamera::Camera * cam, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems) override;
			/// Copies the data from a GeometryPass into this object
			/// \param ref GeometryPass to be copied
			virtual void copy(const TransparentGeometryPass& ref) { GeometryPass::copy(ref); }
		};
	}
}
#endif