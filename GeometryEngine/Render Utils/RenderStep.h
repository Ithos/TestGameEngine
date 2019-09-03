#pragma once

#ifndef RENDERSTEP_H
#define RENDERSTEP_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <unordered_set>
#include "Items\GeometryItem.h"
#include "Items\GraphicItems\Camera.h"
#include "Items\GraphicItems\Light.h"

namespace GeometryEngine
{
	///namespace for render steps
	namespace GeometryRenderStep
	{
		/// Abstract class that acts as an interface for all RenderSteps. 
		/// Each render step performs all operations corresponding to a certain render phase.
		class RenderStep : protected QOpenGLExtraFunctions
		{
		public:
			/// Constructor. Just calls to initiallizeOpenGLFunctions
			RenderStep();
			/// Copy Constructor
			/// parameter ref Constant reference to the RenderStep to be copied
			RenderStep(const RenderStep& ref) { copy(ref); };
			/// Destructor
			virtual ~RenderStep() {};
			/// Executes the render step operations
			/// param cam Pointer to the camera is being rendered
			/// param items Set of items to be rendered
			/// param lights Set of lights to be rendered
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*> * items = nullptr, 
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) = 0;
			/// Factory method that creates a copy of the object
			/// return Pointer to a copy of the object
			virtual RenderStep* Clone() const = 0;
		protected:
			/// Copies the values of a RenderStep to this object
			/// param ref Constant reference to a RenderStep whose values should be copied
			virtual void copy(const RenderStep& ref) {}
		};
	}
}
#endif