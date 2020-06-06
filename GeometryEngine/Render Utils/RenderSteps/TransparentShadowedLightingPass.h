#pragma once

#ifndef GEOMETRYTRANSPARENTGEOMETRYPASS_H
#define GEOMETRYTRANSPARENTGEOMETRYPASS_H

#include "ShadowedLightingPass.h"

namespace GeometryEngine
{
	namespace GeometryRenderStep
	{
		/// Lighting pass that calculates transparent and translucent shadows
		class TransparentShadowedLightingPass : public ShadowedLightingPass
		{
		public:
			/// Constructor
			/// param useFrontFaceCulling Front face culling helps avoid Shadow acne.
			TransparentShadowedLightingPass(bool useFrontFaceCulling = true) : ShadowedLightingPass(useFrontFaceCulling) {}
			/// Copy constructor
			/// param ref Const reference to LightingPass to be copied
			TransparentShadowedLightingPass(const ShadowedLightingPass& ref) : ShadowedLightingPass() { copy(ref); }
			/// Destructor
			virtual ~TransparentShadowedLightingPass() {}
			/// Factory method. Returns a copy of this object.
			/// return A copy of this object.
			virtual RenderStep* Clone() const override { return new TransparentShadowedLightingPass(*this); }
		protected:
			/// Calculates the shadow map for a single light
			/// param light Light whose shadow mat will be calculated
			/// param items Set of items whose shadows will be applied
			virtual void calculateSingleLightShadowMap(GeometryWorldItem::GeometryLight::Light* light, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items) override;
			/// Copies the data from a LightingPass into this object
			/// param ref LightingPass to be copied
			virtual void copy(const ShadowedLightingPass& ref) { ShadowedLightingPass::copy(ref); }
		};
	}
}
#endif