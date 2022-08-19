#pragma once
/*
#ifndef GEOMETRYPOSTPROCESSPASS_H
#define GEOMETRYPOSTPROCESSPASS_H

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
		/// Applies a double step post process to the individual scene objects
		class GeometryPostProcessPass : public RenderStep
		{
		public:
			/// Constructor
			GeometryPostProcessPass() : RenderStep() {}
			/// Copy Constructor
			/// \param ref Const reference to DoublePostProcessPass to be copied
			GeometryPostProcessPass(const GeometryPostProcessPass& ref) { copy(ref); }
			/// Destructor
			virtual ~GeometryPostProcessPass() {}
			/// Executes the render step
			/// \param cam Pointer to camera to be rendered. We only need the information of the camera and the geometry buffer.
			/// \param items Set of items to be rendered. Not used.
			/// \param lights Set of lights in the scene. Not used.
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*> * items = nullptr,
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			/// Factory method. Returns a copy of this object.
			/// \return A copy of this object.
			virtual RenderStep* Clone() const override { return new GeometryPostProcessPass(*this); }
		protected:
			/// Initializes the OpenGl pipeline
			void initStep();
			/// Resets the state of the OpenGl pipeline
			void endStep();
			/// Draws each item in the set using the camera transformation matrix
			/// \param cam Pointer to camera to be rendered
			/// \param items Set of items to be rendered
			virtual void renderGeometry(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items);
			/// Draws an individual item.
			/// \param cam Pointer to camera to be rendered
			/// \param item Item to be rendered
			void drawItem(GeometryWorldItem::GeometryCamera::Camera* cam, GeometryWorldItem::GeometryItem::GeometryItem* item);
			/// Method that checks if the item and the camera are in the same render group. Used to decide if an item should be rendered
			/// \param cam Pointer to camera to be rendered
			/// \param item Item to be rendered
			/// \return true if both are in the same render group false otherwise
			bool checkRenderGroups(GeometryWorldItem::GeometryCamera::Camera* cam, GeometryWorldItem::GeometryItem::GeometryItem* item);
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
			/// \param gBuff Object that contains buffer data adapted to be passed to the shaders
			void applyExtraSteps(GeometryPostProcess::PostProcess* postProcess, const GBufferTextureInfo& gBuff);
			/// Copies the data from a DoublePostProcessPass into this object
			/// \param ref DoublePostProcessPass to be copied
			virtual void copy(const GeometryPostProcessPass& ref) { RenderStep::copy(ref); }

		};
	}
}

#endif //GEOMETRYPOSTPROCESSPASS_H
*/