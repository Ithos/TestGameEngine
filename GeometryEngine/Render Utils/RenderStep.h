#pragma once

#ifndef RENDERSTEP_H
#define RENDERSTEP_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <unordered_set>


namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryCamera
		{
			class Camera;
		}

		namespace GeometryLight
		{
			class Light;
		}

		namespace GeometryItem
		{
			class GeometryItem;
		}
	}

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
			/// \parameter ref Constant reference to the RenderStep to be copied
			RenderStep(const RenderStep& ref) { copy(ref); };
			/// Destructor
			virtual ~RenderStep() {};
			/// Executes the render step operations
			/// \param cam Pointer to the camera is being rendered
			/// \param orderedItems Set of items to be rendered ordered by distance to the camera
			/// \param lights Set of lights to be rendered
			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems = nullptr,
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) = 0;
			/// Factory method that creates a copy of the object
			/// \return Pointer to a copy of the object
			virtual RenderStep* Clone() const = 0;
		protected:
			/// Method that checks if the item and the camera are in the same render group. Used to decide if an item should be rendered
			/// \param cam Pointer to camera to be rendered
			/// \param item Item to be rendered
			/// \return true if both are in the same render group false otherwise
			bool checkRenderGroups(GeometryWorldItem::GeometryCamera::Camera* cam, GeometryWorldItem::GeometryItem::GeometryItem* item);
			/// Copies the values of a RenderStep to this object
			/// \param ref Constant reference to a RenderStep whose values should be copied
			virtual void copy(const RenderStep& ref) {}
		};
	}
}
#endif