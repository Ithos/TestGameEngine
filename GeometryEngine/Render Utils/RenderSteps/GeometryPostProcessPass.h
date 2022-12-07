#pragma once

#ifndef GEOMETRYPOSTPROCESSPASS_H
#define GEOMETRYPOSTPROCESSPASS_H

#include "../RenderStep.h"
#include "../GBuffer.h"

namespace GeometryEngine
{
	class GBufferTextureInfo;

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
			/// \param orderedItems Set of items to be rendered ordered by distance to the camera.
			/// \param lights Set of lights in the scene. Not used.
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems = nullptr,
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
			virtual void renderGeometry(GeometryWorldItem::GeometryCamera::Camera * cam, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * items);
			/// Draws an individual item.
			/// \param cam Pointer to camera to be rendered
			/// \param buf Pointer to the camera geometry buffer
			/// \param gbuff Reference to the gbuffer texture information structure
			/// \param item Item to be rendered
			/// \param texture location of the texture where the post process will be applied
			void drawItem(GeometryWorldItem::GeometryCamera::Camera* cam, GeometryBuffer::GBuffer* buf, const GBufferTextureInfo& gbuff, GeometryWorldItem::GeometryItem::GeometryItem* item,
				const GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE & texture);
			/// Prepares the OpenGl pipeline for the first pass post process 
			/// \param buf Pointer to the geometry buffer
			/// \param texture location of the texture where the post process will be applied
			void initPostProcessPass(GeometryBuffer::GBuffer* buf, const GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE & texture);
			/// Prepares the OpenGl pipeline for the second pass post process 
			/// \param buf Pointer to the geometry buffer
			/// \param texture location of the texture where the post process will be applied
			void secondPostProcessPass(GeometryBuffer::GBuffer* buf, const GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE & texture);
			/// Blits the tmp texture into the binded texture
			/// \param buf Pointer to the geometry buffer
			void copyBuffer(GeometryBuffer::GBuffer* buf);
			/// Copies the data from a DoublePostProcessPass into this object
			/// \param ref DoublePostProcessPass to be copied
			virtual void copy(const GeometryPostProcessPass& ref) { RenderStep::copy(ref); }
		};
	}
}

#endif //GEOMETRYPOSTPROCESSPASS_H
