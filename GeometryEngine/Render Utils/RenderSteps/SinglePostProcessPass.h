#pragma once

#ifndef SINGLEPOSTPROCESSPASS_H
#define SINGLEPOSTPROCESSPASS_H

#include "../RenderStep.h"
#include "../Items/PostProcess/SinglePassPostProcess.h"

namespace GeometryEngine
{
	namespace GeometryRenderStep
	{
		class SinglePostProcessPass : public RenderStep
		{
		public:
			SinglePostProcessPass() : RenderStep() {}
			SinglePostProcessPass(const SinglePostProcessPass& ref) { copy(ref); }
			virtual ~SinglePostProcessPass() {}
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*> * items = nullptr,
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			virtual RenderStep* Clone() const override { return new SinglePostProcessPass(*this); }
		protected:
			void applyPostProcess(GeometryBuffer::GBuffer* buf, std::list< GeometryPostProcess::PostProcess*> postProcess);
			void initPostProcessPass(GeometryBuffer::GBuffer* buf);
			void finishPostProcesPass(GeometryBuffer::GBuffer* buf);
			virtual void copy(const SinglePostProcessPass& ref) { RenderStep::copy(ref); }
		};
	}
}

#endif
