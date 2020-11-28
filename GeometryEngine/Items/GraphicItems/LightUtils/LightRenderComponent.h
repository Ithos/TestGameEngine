#pragma once

#ifndef GEOMETRYRENDERTECHNIQUE_H
#define GEOMETRYRENDERTECHNIQUE_H

#include <qmatrix4x4.h>
#include "LightComponentManager.h"

namespace GeometryEngine
{
	class BuffersInfo;

	namespace LightUtils
	{
		/// Class that applies a render technique for the parent light
		class LightRenderComponent
		{
		public:
			/// Constructor
			/// \param parent Pointer to the LightFunctionalities that contains it 
			/// \param step indicates to which LightRender key this object corresponds
			LightRenderComponent(LightComponentManager* parent, LightRender step);

			/// Copy constructor
			/// \param ref Object to be copied.
			LightRenderComponent(const LightRenderComponent& ref) { copy(ref); }

			/// Destructor
			virtual ~LightRenderComponent() {};

			/// Apply light technique. Abstract method to be implemented by child classes.
			/// \param projectionMatrix Camera projection matrix
			/// \param viewMatrix Camera view matrix
			/// \param gBuffInfo Texture info from the camera buffers
			/// \param viewPos Position of the camera
			virtual void Render(const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const BuffersInfo& buffersInfo, const QVector3D& viewPos) = 0;

			/// Sets the target for this technique
			/// \param target Light to be rendered
			virtual void SetTargetLight(GeometryWorldItem::GeometryLight::Light* target) { mpTargetLight = target; }

			/// Abstract method. Factory method. Creates a copy of this object
			/// \param parent Pointer to the LightFunctionalities that contains it 
			/// \param step indicates to which LightRender key this object corresponds
			/// \return Pointer to a copy of this object
			virtual LightRenderComponent* Clone(LightComponentManager* parent, LightRender step) const = 0;

		protected:

			LightComponentManager* mpParentInterface;
			GeometryWorldItem::GeometryLight::Light* mpTargetLight;

			/// Sets a new parent interface and adds the step to that interface
			/// \param parent Pointer to the LightComponentManager that contains it 
			/// \param step indicates to which LightRender key this object corresponds
			virtual void AddToInterface(LightComponentManager* parent, LightRender step);

			/// Copies the data of a CustomShadingStep object to the current object
			/// \param ref CustomShadingStep to be copied
			virtual void copy(const LightRenderComponent& ref);
		};
	}
}

#endif