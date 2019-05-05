#pragma once

#ifndef DOUBLEPOSTPROCESSPASS_H
#define DOUBLEPOSTPROCESPASS_H

#include "../RenderStep.h"
#include "../Items/PostProcess/DoublePassPostProcess.h"

namespace GeometryEngine
{
	namespace GeometryRenderStep
	{
		class DoublePostProcessPass : public RenderStep
		{
		public:
			DoublePostProcessPass() : RenderStep() {}
			DoublePostProcessPass(const DoublePostProcessPass& ref) { copy(ref); }
			virtual ~DoublePostProcessPass() {}
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*> * items = nullptr,
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			virtual RenderStep* Clone() const override { return new DoublePostProcessPass(*this); }
		protected:
			void applyPostProcess(GeometryBuffer::GBuffer* buf, const std::list< GeometryPostProcess::PostProcess*>& postProcess);
			void initPostProcessPass(GeometryBuffer::GBuffer* buf);
			void secondPostProcessPass(GeometryBuffer::GBuffer* buf);
			virtual void copy(const DoublePostProcessPass& ref) { RenderStep::copy(ref); }
		};
	}
}

#endif
