#pragma once

#ifndef GEOMETRYRENDERTECHNIQUE_H
#define GEOMETRYRENDERTECHNIQUE_H

#include <qmatrix4x4.h>
#include "LightFunctionalities.h"

namespace GeometryEngine
{
	class BuffersInfo;

	namespace LightUtils
	{

		/// Class that applies a render technique for the parent light
		class LightRenderTechnique
		{
		public:
			/// Constructor
			/// param parent Pointer to the LightFunctionalities that contains it 
			/// param step indicates to which technique this class corresponds
			LightRenderTechnique(LightFunctionalities* parent, LightTechniques step);

			/// Copy constructor
			/// param ref Object to be copied.
			LightRenderTechnique(const LightRenderTechnique& ref) { copy(ref); }

			/// Destructor
			virtual ~LightRenderTechnique() {};

			/// Apply light technique. 
			/// param projectionMatrix Camera projection matrix
			/// param viewMatrix Camera view matrix
			/// param gBuffInfo Texture info from the camera buffers
			/// param viewPos Position of the camera
			virtual void ApplyTechnique(const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const BuffersInfo& buffersInfo, const QVector3D& viewPos) = 0;

			/// Sets the target for this technique
			/// param target Light to be rendered
			virtual void SetTargetLight(GeometryWorldItem::GeometryLight::Light* target) { mpTargetLight = target; }

			/// Abstract method. Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual LightRenderTechnique* Clone(LightFunctionalities* parent, LightTechniques step) const = 0;

		protected:

			LightFunctionalities* mpParentInterface;
			GeometryWorldItem::GeometryLight::Light* mpTargetLight;

			/// Sets a new parent interface and adds the step to that interface
			/// param parent Pointer to the LightFunctionalities that contains it 
			/// param step indicates to which function this class corresponds
			virtual void AddToInterface(LightFunctionalities* parent, LightTechniques step);

			/// Copies the data of a CustomShadingStep object to the current object
			/// param ref CustomShadingStep to be copied
			virtual void copy(const LightRenderTechnique& ref);
		};
	}
}

#endif