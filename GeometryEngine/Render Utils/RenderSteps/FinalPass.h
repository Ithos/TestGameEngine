#pragma once
#ifndef FINALPASS_H
#define FINALPASS_H

#include "../RenderStep.h"

namespace GeometryEngine
{
	namespace GeometryRenderStep
	{
		class FinalPass : public RenderStep
		{
		public:
			FinalPass() : RenderStep() {}
			FinalPass(const FinalPass& ref) { copy(ref); }
			virtual ~FinalPass() {}
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*> * items = nullptr, 
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			virtual RenderStep* Clone() const override { return new FinalPass(*this); }
		protected:
			void renderToScreen(GeometryWorldItem::GeometryCamera::Camera* cam);
			virtual void copy(const FinalPass& ref) { RenderStep::copy(ref); }
		};
	}
}
#endif