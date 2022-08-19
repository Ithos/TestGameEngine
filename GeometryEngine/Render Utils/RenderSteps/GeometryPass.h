#pragma once

#ifndef GEOMETRYPASS_H
#define GEOMETRYPASS_H

#include "../RenderStep.h"

namespace GeometryEngine
{
	namespace GeometryRenderStep
	{
		/// Renders geometry without illumination
		class GeometryPass : public RenderStep
		{
		public:
			/// Construct
			GeometryPass() : RenderStep() {}
			/// Copy constructor
			/// \param ref Geometry pass to be copied
			GeometryPass(const GeometryPass& ref) { copy(ref); }
			/// Destructor
			virtual ~GeometryPass() {}

			/// Executes the render step
			/// \param cam Pointer to camera to be rendered
			/// \param orderedItems Set of items to be rendered ordered by distance to the camera
			/// \param lights Set of lights in the scene. This lights will not be used in this step.
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems = nullptr,
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			/// Factory method. Returns a copy of this object.
			/// \return Acopy of this object.
			virtual RenderStep* Clone() const override { return new GeometryPass(*this); }

		protected:
			/// Initializes the OpenGl pipeline
			void initStep();
			/// Draws each item in the set using the camera transformation matrix
			/// \param cam Pointer to camera to be rendered
			/// \param orderedItems Set of items to be rendered ordered by distance to the camera
			virtual void renderGeometry(GeometryWorldItem::GeometryCamera::Camera * cam, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems);
			/// Draws an individual item.
			/// \param cam Pointer to camera to be rendered
			/// \param item Item to be rendered
			void drawItem(GeometryWorldItem::GeometryCamera::Camera* cam, GeometryWorldItem::GeometryItem::GeometryItem* item);
			/// Copies the data from a GeometryPass into this object
			/// \param ref GeometryPass to be copied
			virtual void copy(const GeometryPass& ref) { RenderStep::copy(ref); }
		};
	}
}

#endif