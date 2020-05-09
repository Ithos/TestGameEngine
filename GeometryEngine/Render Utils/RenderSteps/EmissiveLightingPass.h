#pragma once

#ifndef EMISSIVELIGHTINGPASS_H
#define EMISSIVELIGHTINGPASS_H

#include "LightingPass.h"

namespace GeometryEngine
{
	namespace GeometryRenderStep
	{
		/// Renders emissive colors and textures
		class EmissiveLightingPass : public LightingPass
		{
		public:
			/// Constructor
			EmissiveLightingPass(const GeometryWorldItem::GeometryLight::Light& emissiveLighting);
			/// Copy constructor
			/// param ref Geometry pass to be copied
			EmissiveLightingPass(const EmissiveLightingPass& ref) { copy(ref); }
			virtual ~EmissiveLightingPass();
			/// Executes the render step
			/// param cam Pointer to camera to be rendered
			/// param items Set of items to be rendered
			/// param lights Set of lights in the scene. This lights will not be used in this step.
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*> * items = nullptr,
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			/// Factory method. Returns a copy of this object.
			/// return A copy of this object.
			virtual EmissiveLightingPass* Clone() const override { return new EmissiveLightingPass(*this); }

		protected:
			/// Copies the data from a LightingPass into this object
			/// param ref LightingPass to be copied
			virtual void copy(const EmissiveLightingPass& ref) { RenderStep::copy(ref); }

			GeometryWorldItem::GeometryLight::Light* mpEmissiveLighting;
			std::unordered_set<GeometryWorldItem::GeometryLight::Light*> mEmissiveLights;
		};
	}
}

#endif