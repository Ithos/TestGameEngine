#pragma once

#ifndef DOUBLEPOSTPROCESSPASS_H
#define DOUBLEPOSTPROCESPASS_H

#include "../RenderStep.h"

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		class GBuffer;
	}

	namespace GeometryPostProcess
	{
		class PostProcess;
	}

	namespace GeometryRenderStep
	{
		/// Applies a double step post process to the whole scene
		class PostProcessPass : public RenderStep
		{
		public:
			/// Constructor
			PostProcessPass() : RenderStep() {}
			/// Copy Constructor
			/// param ref Const reference to DoublePostProcessPass to be copied
			PostProcessPass(const PostProcessPass& ref) { copy(ref); }
			/// Destructor
			virtual ~PostProcessPass() {}
			/// Executes the render step
			/// param cam Pointer to camera to be rendered. We only need the information of the camera and the geometry buffer.
			/// param items Set of items to be rendered. Not used.
			/// param lights Set of lights in the scene. Not used.
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*> * items = nullptr,
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			/// Factory method. Returns a copy of this object.
			/// return A copy of this object.
			virtual RenderStep* Clone() const override { return new PostProcessPass(*this); }
		protected:
			/// Initializes the OpenGl pipeline and applies every single pass post process in the list
			/// param buf Pointer to the geometry buffer
			/// param list of post processes to apply
			void applyPostProcess(GeometryBuffer::GBuffer* buf, const std::list< GeometryPostProcess::PostProcess*>& postProcess);
			/// Prepares the OpenGl pipeline for the first pass post process 
			/// param buf Pointer to the geometry buffer
			void initPostProcessPass(GeometryBuffer::GBuffer* buf);
			/// Prepares the OpenGl pipeline for the second pass post process 
			/// param buf Pointer to the geometry buffer
			void secondPostProcessPass(GeometryBuffer::GBuffer* buf);
			/// Blits the tmp texture into the final texture
			/// param buf Pointer to the geometry buffer
			void finishPostProcesPass(GeometryBuffer::GBuffer* buf);
			/// Copies the data from a DoublePostProcessPass into this object
			/// param ref DoublePostProcessPass to be copied
			virtual void copy(const PostProcessPass& ref) { RenderStep::copy(ref); }
		};
	}
}

#endif
