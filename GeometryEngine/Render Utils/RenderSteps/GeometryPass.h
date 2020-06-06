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
			/// param ref Geometry pass to be copied
			GeometryPass(const GeometryPass& ref) { copy(ref); }
			/// Destructor
			virtual ~GeometryPass() {}

			/// Executes the render step
			/// param cam Pointer to camera to be rendered
			/// param items Set of items to be rendered
			/// param lights Set of lights in the scene. This lights will not be used in this step.
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*> * items = nullptr, 
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			/// Factory method. Returns a copy of this object.
			/// return Acopy of this object.
			virtual RenderStep* Clone() const override { return new GeometryPass(*this); }

		protected:
			/// Initializes the OpenGl pipeline
			void initStep();
			/// Draws each item in the set using the camera transformation matrix
			/// param cam Pointer to camera to be rendered
			/// param items Set of items to be rendered
			virtual void renderGeometry(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items);
			/// Draws an individual item.
			/// param cam Pointer to camera to be rendered
			/// param item Item to be rendered
			void drawItem(GeometryWorldItem::GeometryCamera::Camera* cam, GeometryWorldItem::GeometryItem::GeometryItem* item);
			/// Method that checks if the item and the camera are in the same render group. Used to decide if an item should be rendered
			/// param cam Pointer to camera to be rendered
			/// param item Item to be rendered
			/// return true if both are in the same render group false otherwise
			bool checkRenderGroups(GeometryWorldItem::GeometryCamera::Camera* cam, GeometryWorldItem::GeometryItem::GeometryItem* item);
			/// Copies the data from a GeometryPass into this object
			/// param ref GeometryPass to be copied
			virtual void copy(const GeometryPass& ref) { RenderStep::copy(ref); }
		};
	}
}

#endif