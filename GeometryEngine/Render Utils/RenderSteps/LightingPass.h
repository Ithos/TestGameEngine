#pragma once

#ifndef LIGHTINGPASS_H
#define LIGHTINGPASS_H

#include"../RenderStep.h"

namespace GeometryEngine
{
	namespace GeometryRenderStep
	{
		class LightingPass : public RenderStep
		{
		public:

			LightingPass() : RenderStep() {}
			LightingPass(const LightingPass& ref) { copy(ref); }
			virtual ~LightingPass() {}

			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*> * items = nullptr, 
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			virtual RenderStep* Clone() const override { return new LightingPass(*this); }

		protected:
			void initStep();
			void renderLights(GeometryWorldItem::GeometryCamera::Camera* cam, std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights);
			void applyLight(GeometryWorldItem::GeometryCamera::Camera* cam, std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights);
			void finishStep();

			void prepareStencilPass(GeometryWorldItem::GeometryCamera::Camera* cam);
			void stencilPass(GeometryWorldItem::GeometryLight::Light* light, GeometryWorldItem::GeometryCamera::Camera* cam);
			void setStencilLight();
			void prepareLightPass();
			void finishLightPass();
			void finishStencilPass();

			virtual void copy(const LightingPass& ref) { RenderStep::copy(ref); }
		};
	}
}

#endif