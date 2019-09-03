#pragma once

#ifndef SINGLEPOSTPROCESSPASS_H
#define SINGLEPOSTPROCESSPASS_H

#include "../RenderStep.h"
#include "../Items/PostProcess/SinglePassPostProcess.h"

namespace GeometryEngine
{
	namespace GeometryRenderStep
	{
		/// Applies a single step post process to the whole scene
		class SinglePostProcessPass : public RenderStep
		{
		public:
			/// Constructor
			SinglePostProcessPass() : RenderStep() {}
			/// Copy Constructor
			/// param ref Const reference to SinglePostProcessPass to be copied
			SinglePostProcessPass(const SinglePostProcessPass& ref) { copy(ref); }
			/// Destructor
			virtual ~SinglePostProcessPass() {}
			/// Executes the render step
			/// param cam Pointer to camera to be rendered. We only need the information of the camera and the geometric buffer.
			/// param items Set of items to be rendered. Not used.
			/// param lights Set of lights in the scene. Not used.
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*> * items = nullptr,
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			/// Factory method. Returns a copy of this object.
			/// return A copy of this object.
			virtual RenderStep* Clone() const override { return new SinglePostProcessPass(*this); }
		protected:
			/// Initializes the OpenGl pipeline and applies every single pass post process in the list
			/// param buf Pointer to the geometric buffer
			/// param list of post processes to apply
			void applyPostProcess(GeometryBuffer::GBuffer* buf, std::list< GeometryPostProcess::PostProcess*> postProcess);
			/// Prepares the OpenGl pipeline for the post process 
			/// param buf Pointer to the geometric buffer
			void initPostProcessPass(GeometryBuffer::GBuffer* buf);
			/// Blits the tmp texture into the final texture
			/// param buf Pointer to the geometric buffer
			void finishPostProcesPass(GeometryBuffer::GBuffer* buf);
			/// Copies the data from a SinglePostProcessPass into this object
			/// param ref SinglePostProcessPass to be copied
			virtual void copy(const SinglePostProcessPass& ref) { RenderStep::copy(ref); }
		};
	}
}

#endif
