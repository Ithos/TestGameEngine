#pragma once

#ifndef GEOMETRYTRANSPARENTGEOMETRYPASS_H
#define GEOMETRYTRANSPARENTGEOMETRYPASS_H

#include "ShadowedLightingPass.h"

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		class ShadingBuffer;
	}

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
			/// Calculates the translucent color map for a single light
			/// param light Light whose color map will be calculated
			/// param items Set of items whose color map will be applied
			virtual void calculateSingleLightColorMap(GeometryWorldItem::GeometryLight::Light* light, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* translucentItems);
			/// Calculates the translucent items shadow map for a single light
			/// param light Light whose shadow map will be calculated
			/// param translucent items Set of items whose shadows will be applied
			virtual void calculateSingleLightTranslucentShadowMap(GeometryWorldItem::GeometryLight::Light* light, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* translucentItems);
			/// Copies the data from a LightingPass into this object
			/// param ref LightingPass to be copied
			virtual void copy(const ShadowedLightingPass& ref) { ShadowedLightingPass::copy(ref); }
			/// Calculates the shadowmap for every light and applies the light contribution
			/// param cam Pointer to camera to be rendered
			/// param items Set of items to be rendered. Items will no be used in this step, we will use the information stored in the geometry buffer instead.
			/// param shadowedLights Set of shadow casting lights in the scene.
			virtual void CalculateShadowMap(GeometryWorldItem::GeometryCamera::Camera* cam, std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* shadowedLights,
				std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items) override;
			/// Calculates the colormap contribution of the item
			/// param item Item to be rendered into the colormap
			/// param light Light whose color map will be calculated
			virtual void calculateItemTranslucentShadowing(GeometryWorldItem::GeometryItem::GeometryItem * item, GeometryWorldItem::GeometryLight::Light* light);
		private:
			/// Performs the actions required to calculate the transparent shadowmap
			/// param buf ShadingBuffer that stores the shadow map
			void initTransparentShadowMap(GeometryBuffer::ShadingBuffer* buf);
			/// Resets the pipeline state after the transparent shadowmap calculation
			void finishTransparentShadowMap(GeometryBuffer::ShadingBuffer* buf);
			/// Initializes the OpenGl pipeline for the color map calculation
			/// param buf ShadingBuffer that stores the shadow map
			void initColorMap(GeometryBuffer::ShadingBuffer* buf);
			/// Resets the openGl pipeline
			/// param buf ShadingBuffer that stores the shadow map
			void finishColorMap(GeometryBuffer::ShadingBuffer* buf);
		};
	}
}
#endif