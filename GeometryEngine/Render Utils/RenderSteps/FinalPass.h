#pragma once
#ifndef FINALPASS_H
#define FINALPASS_H

#include "../RenderStep.h"

namespace GeometryEngine
{
	namespace GeometryRenderStep
	{
		/// Renders the final texture to the screen
		class FinalPass : public RenderStep
		{
		public:
			/// Constructor
			FinalPass() : RenderStep() {}
			/// Copy Constructor
			/// param ref Const reference to FinalPass to be copied
			FinalPass(const FinalPass& ref) { copy(ref); }
			/// Destructor
			virtual ~FinalPass() {}
			/// Executes the render step
			/// param cam Pointer to camera to be rendered. We only need the information of the camera and the geometric buffer.
			/// param items Set of items to be rendered. Not used.
			/// param lights Set of lights in the scene. Not used.
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*> * items = nullptr, 
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			/// Factory method. Returns a copy of this object.
			/// return A copy of this object.
			virtual RenderStep* Clone() const override { return new FinalPass(*this); }
		protected:
			/// Blits final texture to the screen
			/// param cam Pointer to camera to be rendered.
			void renderToScreen(GeometryWorldItem::GeometryCamera::Camera* cam);
			/// Copies the data from a FinalPass into this object
			/// param ref FinalPass to be copied
			virtual void copy(const FinalPass& ref) { RenderStep::copy(ref); }
		};
	}
}
#endif