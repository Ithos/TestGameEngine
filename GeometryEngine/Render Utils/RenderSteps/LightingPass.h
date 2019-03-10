#pragma once

#ifndef LIGHTINGPASS_H
#define LIGHTINGPASS_H

#include"../RenderStep.h"

namespace GeometryEngine
{
	class LightingPass : RenderStep
	{
	public:

		LightingPass() : RenderStep() {}
		virtual ~LightingPass() {}

		virtual void Render(Camera* cam = nullptr, std::unordered_set<GeometryItem*> * items = nullptr, std::unordered_set<Light*> * lights = nullptr) override;

	protected:
		void initStep();
		void renderLights(Camera* cam, std::unordered_set<Light*> * lights);
		void applyLight(Camera* cam, std::unordered_set<Light*> * lights);
		void finishStep();

		void prepareStencilPass(Camera* cam);
		void stencilPass(Light* light, Camera* cam);
		void setStencilLight();
		void prepareLightPass();
		void finishLightPass();
		void finishStencilPass();
	};
}

#endif