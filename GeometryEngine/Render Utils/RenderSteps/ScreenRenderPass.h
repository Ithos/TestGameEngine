#pragma once
#ifndef FINALPASS_H
#define FINALPASS_H

#include "../RenderStep.h"
#include "../RenderStepUtils/ScreenViewportLocations.h"

namespace GeometryEngine
{
	namespace GeometryRenderStep
	{
		/// Renders the final texture to the screen
		class ScreenRenderPass : public RenderStep
		{
		public:
			/// Constructor
			ScreenRenderPass();
			/// Constructor
			/// \param viewportNumber Number of viewports that the screen will contain
			ScreenRenderPass(int viewportNumber);
			/// Constructor
			/// \param locationMap Map of camera targets and their positions and sizes
			ScreenRenderPass(const std::map<GeometryWorldItem::GeometryCamera::CameraTargets, QVector4D>& locationMap);
			/// Constructor
			/// \param viewportLocations Object that contains the locations for the window viewports and their camera target
			ScreenRenderPass(const ScreenViewportLocation&  viewportLocations);
			/// Copy Constructor
			/// \param ref Const reference to FinalPass to be copied
			ScreenRenderPass(const ScreenRenderPass& ref) { copy(ref); }
			/// Destructor
			virtual ~ScreenRenderPass();
			/// Executes the render step
			/// \param cam Pointer to camera to be rendered. We only need the information of the camera and the geometry buffer.
			/// \param orderedItems Set of items to be rendered ordered by distance to the camera. Not used.
			/// \param lights Set of lights in the scene. Not used.
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems = nullptr,
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			/// Factory method. Returns a copy of this object.
			/// \return A copy of this object.
			virtual RenderStep* Clone() const override { return new ScreenRenderPass(*this); }
		protected:
			ScreenViewportLocation* mViewportLocations;

			/// Blits final texture to the screen
			/// \param cam Pointer to camera to be rendered.
			void renderToScreen(GeometryWorldItem::GeometryCamera::Camera* cam);
			/// Copies the data from a FinalPass into this object
			/// \param ref FinalPass to be copied
			virtual void copy(const ScreenRenderPass& ref) { RenderStep::copy(ref); }
		};
	}
}
#endif