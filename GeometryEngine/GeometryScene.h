#pragma once

#ifndef GEOMETRYSCENE_H
#define GEOMETRYSCENE_H

#include <unordered_set>
#include <list>
#include "Items/WorldItem.h"
#include "Items\GeometryItem.h"
#include "Items/GraphicItems/Camera.h"
#include "Items\GraphicItems\Light.h"
#include "Render Utils\RenderStep.h"
#include "Render Utils\GlSetup.h"
#include "SceneManager.h"


namespace GeometryEngine
{
	class SceneManager;

	namespace GeometryWorldItem
	{
		class WorldItem;

		namespace GeometryLight
		{
			class Light;
		}

		namespace GeometryCamera
		{
			class Camera;
		}
	}

	namespace GeometryGlSetup
	{
		class GlSetup;
	}

	namespace GeometryRenderStep
	{
		class RenderStep;
	}
	///Namespace for scenes
	namespace GeometryScene
	{
		/// Base class for more specific scenes. It can be used as a custom scene if you need to build your own
		/// Scenes should define the render steps and initial setup that will be used and contain GeometricItems, cameras and lights.
		class GeometryScene
		{
		public:
			/// Constructor. Here the scene adds itself to the scene manager.
			/// param manager Pointer to the scene manager
			/// param fovy Field of view angle
			/// param zNear Near plane z coordinate
			/// param zFar Far plane z coordinate
			/// param clearColor Clear color for the scene
			GeometryScene(SceneManager* manager, GLdouble fovy = 45.0, GLdouble zNear = 0.1, GLdouble zFar = 30.0, QVector4D clearColor = QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
			/// Destructor. Destroys all render stpeps and the initial setup.
			virtual ~GeometryScene();

			/// Scale the scene and the viewports for all cameras
			/// param w New scene width
			/// param h New scene height
			/// param formerW Previous scene width
			/// param formerH Previous scene height
			virtual void ResizeScene(int w, int h, int formerW, int formerH);
			/// Render all cameras
			virtual void Draw();
			/// Add item to the scene
			/// param item Pointer to GeometryItem
			/// return false if the scene already contained the item true otherwise
			virtual bool AddItem(GeometryWorldItem::GeometryItem::GeometryItem* item);
			/// Remove item from the scene
			/// param item Pointer to GeometryItem
			/// return false if the scene doesn't contain the item true otherwise
			virtual bool RemoveItem(GeometryWorldItem::GeometryItem::GeometryItem* item);
			/// Add camera to the scene
			/// param item Pointer to Camera
			/// return false if the scene already contained the camera true otherwise
			virtual bool AddCamera(GeometryWorldItem::GeometryCamera::Camera* item);
			/// Remove camera from the scene
			/// param item Pointer to Camera
			/// return false if the scene doesn't contain the camera true otherwise
			virtual bool RemoveCamera(GeometryWorldItem::GeometryCamera::Camera* item);
			/// Add light to the scene
			/// param item Pointer to Light
			/// return false if the scene already contained the light true otherwise
			virtual bool AddLight(GeometryWorldItem::GeometryLight::Light* item);
			/// Remove light from the scene
			/// param item Pointer to Light
			/// return false if the scene doesn't contain the light true otherwise
			virtual bool RemoveLight(GeometryWorldItem::GeometryLight::Light* item);
			/// Apply the initial OpenGl setup
			virtual void InitializeGL();
			/// Set the initla setup object for the scene
			/// param setup Constant reference to GlSetup
			virtual void SetInitialSetup(const GeometryGlSetup::GlSetup& setup) { mpInitialSetup = setup.Clone(); }
			/// Add render step to the scene
			/// param step Constant reference to RenderStep
			/// return false if the scene already contained the render step true otherwise
			virtual bool AddRenderStep(const GeometryRenderStep::RenderStep& step);
			/// Remove render step at a certain position on the list from the scene
			/// param pos index of the render step on the list
			/// return false if the srender step list doesn't contain the index true otherwise
			virtual bool RemoveRenderStep(int pos);
			/// Insert a render step at a certain position on the render step list
			/// param step Constant reference to RenderStep
			/// param pos Position on the list to insert the render step 
			/// return false if the srender step list doesn't contain the index true otherwise
			virtual bool InsertRenderStep(const GeometryRenderStep::RenderStep& step, unsigned int pos);
			/// Delete all render steps and clear list
			virtual void ClearRenderSteps();

		protected:
			std::unordered_set< GeometryWorldItem::GeometryItem::GeometryItem* > mItemList;// DO NOT DELETE
			SceneManager* mpParentManager; // DO NOT DELETE
			QVector4D mClearColor;
			std::unordered_set< GeometryWorldItem::GeometryCamera::Camera* > mCameras;// DO NOT DELETE
			std::unordered_set< GeometryWorldItem::GeometryLight::Light* > mLights;// DO NOT DELETE
			std::list<GeometryRenderStep::RenderStep * > mRenderSteps;
			GeometryGlSetup::GlSetup* mpInitialSetup;

			/// Render individual camera
			/// param cam Pointer to camera to be rendered
			/// param renderSteps List of render steps to be performed
			virtual void renderCamera(GeometryWorldItem::GeometryCamera::Camera * cam, const std::list<GeometryRenderStep::RenderStep * >& renderSteps);
		};
	}
}

#endif