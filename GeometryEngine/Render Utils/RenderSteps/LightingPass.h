#pragma once

#ifndef LIGHTINGPASS_H
#define LIGHTINGPASS_H

#include "../RenderStep.h"

namespace GeometryEngine
{
    class BuffersInfo;

	namespace GeometryRenderStep
	{
		/// Appplies lighting to already rendered geometry
		class LightingPass : public RenderStep
		{
		public:
			/// Constructor
			LightingPass() : RenderStep() {}
			/// Copy constructor
			/// \param ref Const reference to LightingPass to be copied
			LightingPass(const LightingPass& ref) { copy(ref); }
			/// Destructor
			virtual ~LightingPass() {}

			/// Executes the render step
			/// \param cam Pointer to camera to be rendered
			/// \param orderedItems Set of items to be rendered ordered by distance to the camera. Items will no be used in this step, we will use the information stored in the geometry buffer instead.
			/// \param lights Set of lights in the scene.
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems = nullptr,
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			/// Factory method. Returns a copy of this object.
			/// \return A copy of this object.
			virtual RenderStep* Clone() const override { return new LightingPass(*this); }

		protected:
			/// Initializes the OpenGl pipeline
			void initStep();
			/// Updates the camera and calls applyLight
			/// \param cam Pointer to Camera to be rendered
			/// \param lights Set of lights in the scene
			void renderLights(GeometryWorldItem::GeometryCamera::Camera* cam, std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights);
			/// Applies stencil test and renders every light in the set
			/// \param cam Pointer to Camera to be rendered
			/// \param lights Set of lights in the scene
			void applyLight(GeometryWorldItem::GeometryCamera::Camera* cam, std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights);
			/// Applies the effects of a single light to the scene
			/// \param cam Pointer to Camera to be rendered
			/// \param gBuf Geometry buffer information to be passed to the light shader
			/// \param light Light to be calculated
			void applySingleLight(GeometryWorldItem::GeometryCamera::Camera* cam, const BuffersInfo& buff, GeometryWorldItem::GeometryLight::Light* light);
			/// Checks if the light uses bounding geometry and calculates the lighting
			/// \param light Light to be calculated
			/// \param projectionMatrix Camera projection matrix
			/// \param viewMatrix Camera view matrix
			/// \param buffTexInfo Buffer information, geometry buffer information is required
			/// \param viewPos Position of the camera
			void calculateLighting(GeometryWorldItem::GeometryLight::Light* light, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const BuffersInfo& buffTexInfo, const QVector3D& viewPos);

			/// Returns the OpenGl pipeline to its original state
			void finishStep();

			/// Prepares the OpenGl pipeline to apply the light stencil test
			/// \param cam Camera being rendered. It contains a pointer to the geometry buffer
			void prepareStencilPass(GeometryWorldItem::GeometryCamera::Camera* cam);
			/// Calls to the light calculateStencil
			/// \param light being rendered
			/// \param cam Pointer to Camera to be rendered
			void stencilPass(GeometryWorldItem::GeometryLight::Light* light, GeometryWorldItem::GeometryCamera::Camera* cam);

			/// Sets the OpenGl pipeline after performing the stencil test
			void setStencilLight();
			/// Prepares the OpenGl pipeline for the actual lighting pass after the stencil test
			void prepareLightPass();
			/// Restores the state of OpenGl pipeline afther the lighting pass
			void finishLightPass();
			/// Restores the state of the OpenGl pipeline after the stencil pass is performed 
			void finishStencilPass();

			/// Copies the data from a LightingPass into this object
			/// \param ref LightingPass to be copied
			virtual void copy(const LightingPass& ref) { RenderStep::copy(ref); }
		};
	}
}

#endif