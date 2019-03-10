#pragma once
#ifndef FINALPASS_H
#define FINALPASS_H

#include "../RenderStep.h"

namespace GeometryEngine
{
	class FinalPass : public RenderStep
	{
	public:
		FinalPass() : RenderStep() {}
		virtual ~FinalPass(){}
		virtual void Render(Camera* cam = nullptr, std::unordered_set<GeometryItem*> * items = nullptr, std::unordered_set<Light*> * lights = nullptr) override;
	protected:
		void renderToScreen(Camera* cam);
	};
}
#endif