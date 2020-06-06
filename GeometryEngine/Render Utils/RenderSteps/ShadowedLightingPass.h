#pragma once

#ifndef SHADOWEDLIGHTINGPASS_H
#define SHADOWEDLIGHTINGPASS_H

#include "LightingPass.h"

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		class GBuffer;
	}

	namespace GeometryRenderStep
	{
		/// Lighting pass that calculates solid shadows
		class ShadowedLightingPass : public LightingPass
		{
		public:
			/// Constructor
			/// param useFrontFaceCulling Front face culling helps avoid Shadow acne.
			ShadowedLightingPass(bool useFrontFaceCulling = true) : mFrontFaceCulling(useFrontFaceCulling), LightingPass() {}
			/// Copy constructor
			/// param ref Const reference to LightingPass to be copied
			ShadowedLightingPass(const ShadowedLightingPass& ref) : LightingPass() { copy(ref); }
			/// Destructor
			virtual ~ShadowedLightingPass() {}

			/// Executes the render step
			/// param cam Pointer to camera to be rendered
			/// param items Set of items to be rendered. Items will no be used in this step, we will use the information stored in the geometry buffer instead.
			/// param lights Set of lights in the scene.
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*> * items = nullptr,
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			/// Factory method. Returns a copy of this object.
			/// return A copy of this object.
			virtual RenderStep* Clone() const override { return new ShadowedLightingPass(*this); }

		protected:

			bool mFrontFaceCulling;

			/// Copies the data from a LightingPass into this object
			/// param ref LightingPass to be copied
			virtual void copy(const ShadowedLightingPass& ref) { LightingPass::copy(ref); }
			///Calculates the shadowmap for every light and applies the light contribution
			/// param cam Pointer to camera to be rendered
			/// param items Set of items to be rendered. Items will no be used in this step, we will use the information stored in the geometry buffer instead.
			/// param shadowedLights Set of shadow casting lights in the scene.
			void CalculateShadowMap(GeometryWorldItem::GeometryCamera::Camera* cam, std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* shadowedLights,
				std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items);
			/// Calculates the shadow map for a single light
			/// param light Light whose shadow mat will be calculated
			/// param items Set of items whose shadows will be applied
			virtual void calculateSingleLightShadowMap(GeometryWorldItem::GeometryLight::Light* light, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items);
			/// Calls the shadowMap calculation for a GeometryItem
			void calculateItemShadowMap(GeometryWorldItem::GeometryItem::GeometryItem* item, GeometryWorldItem::GeometryLight::Light* light);
		private:
			/// Initializes the OpenGl pipeline for the shadow calculation
			/// sets cull face to front and sets the tmpTexture as the active texture, also enables depth test just in case
			void initShadowStep(GeometryBuffer::GBuffer* buf);
			/// Resets the openGl pipeline
			void finishShadowStep(GeometryBuffer::GBuffer* buf);
		};
	}
}
#endif