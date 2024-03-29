#pragma once

#ifndef DOUBLEPOSTPROCESSPASS_H
#define DOUBLEPOSTPROCESPASS_H

#include "../RenderStep.h"

namespace GeometryEngine
{
	class GBufferTextureInfo;

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
			/// \param ref Const reference to DoublePostProcessPass to be copied
			PostProcessPass(const PostProcessPass& ref) { copy(ref); }
			/// Destructor
			virtual ~PostProcessPass() {}
			/// Executes the render step
			/// \param cam Pointer to camera to be rendered. We only need the information of the camera and the geometry buffer.
			/// \param orderedItems Set of items to be rendered ordered by distance to the camera. Not used.
			/// \param lights Set of lights in the scene. Not used.
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems = nullptr,
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			/// Factory method. Returns a copy of this object.
			/// \return A copy of this object.
			virtual RenderStep* Clone() const override { return new PostProcessPass(*this); }
		protected:
			/// Initializes the OpenGl pipeline and applies every single pass post process in the list
			/// \param buf Pointer to the geometry buffer
			/// \param list of post processes to apply
			void applyPostProcess(GeometryBuffer::GBuffer* buf, const std::list< GeometryPostProcess::PostProcess*>& postProcess);
			/// Prepares the OpenGl pipeline for the first pass post process 
			/// \param buf Pointer to the geometry buffer
			void initPostProcessPass(GeometryBuffer::GBuffer* buf);
			/// Prepares the OpenGl pipeline for the second pass post process 
			/// \param buf Pointer to the geometry buffer
			void secondPostProcessPass(GeometryBuffer::GBuffer* buf);
			/// Blits the tmp texture into the final texture
			/// \param buf Pointer to the geometry buffer
			void finishPostProcesPass(GeometryBuffer::GBuffer* buf);
			/// Applies additional steps for the post process
			/// \param postProcess post processes to apply
			/// \param buf Pointer to the geometry buffer
			/// \param gBuff Object that contains buffer data adapted to be passed to the shaders
			/// \return True if the second step was applied
			bool applyExtraSteps(GeometryPostProcess::PostProcess* postProcess, GeometryBuffer::GBuffer * buf, const GBufferTextureInfo& gBuff);
			/// Copies the data from a DoublePostProcessPass into this object
			/// \param ref DoublePostProcessPass to be copied
			virtual void copy(const PostProcessPass& ref) { RenderStep::copy(ref); }
		};
	}
}

#endif
