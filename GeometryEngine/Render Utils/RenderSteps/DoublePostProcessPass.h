#pragma once

#ifndef DOUBLEPOSTPROCESSPASS_H
#define DOUBLEPOSTPROCESPASS_H

#include "../RenderStep.h"
#include "../Items/PostProcess/DoublePassPostProcess.h"

namespace GeometryEngine
{
	namespace GeometryRenderStep
	{
		/// Applies a double step post process to the whole scene
		class DoublePostProcessPass : public RenderStep
		{
		public:
			/// Constructor
			DoublePostProcessPass() : RenderStep() {}
			/// Copy Constructor
			/// param ref Const reference to DoublePostProcessPass to be copied
			DoublePostProcessPass(const DoublePostProcessPass& ref) { copy(ref); }
			/// Destructor
			virtual ~DoublePostProcessPass() {}
			/// Executes the render step
			/// param cam Pointer to camera to be rendered. We only need the information of the camera and the geometric buffer.
			/// param items Set of items to be rendered. Not used.
			/// param lights Set of lights in the scene. Not used.
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*> * items = nullptr,
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			/// Factory method. Returns a copy of this object.
			/// return A copy of this object.
			virtual RenderStep* Clone() const override { return new DoublePostProcessPass(*this); }
		protected:
			/// Initializes the OpenGl pipeline and applies every single pass post process in the list
			/// param buf Pointer to the geometric buffer
			/// param list of post processes to apply
			void applyPostProcess(GeometryBuffer::GBuffer* buf, const std::list< GeometryPostProcess::PostProcess*>& postProcess);
			/// Prepares the OpenGl pipeline for the first pass post process 
			/// param buf Pointer to the geometric buffer
			void initPostProcessPass(GeometryBuffer::GBuffer* buf);
			/// Prepares the OpenGl pipeline for the second pass post process 
			/// param buf Pointer to the geometric buffer
			void secondPostProcessPass(GeometryBuffer::GBuffer* buf);
			/// Copies the data from a DoublePostProcessPass into this object
			/// param ref DoublePostProcessPass to be copied
			virtual void copy(const DoublePostProcessPass& ref) { RenderStep::copy(ref); }
		};
	}
}

#endif
