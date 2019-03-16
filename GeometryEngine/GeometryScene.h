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

	namespace GeometryScene
	{
		class GeometryScene
		{
		public:
			GeometryScene(SceneManager* manager, GLdouble fovy = 45.0, GLdouble zNear = 0.1, GLdouble zFar = 30.0, QVector4D clearColor = QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
			virtual ~GeometryScene();

			virtual void ResizeScene(int w, int h, int formerW, int formerH);
			virtual void Draw();
			virtual bool AddItem(GeometryWorldItem::GeometryItem::GeometryItem* item);
			virtual bool RemoveItem(GeometryWorldItem::GeometryItem::GeometryItem* item);
			virtual bool AddCamera(GeometryWorldItem::GeometryCamera::Camera* item);
			virtual bool RemoveCamera(GeometryWorldItem::GeometryCamera::Camera* item);
			virtual bool AddLight(GeometryWorldItem::GeometryLight::Light* item);
			virtual bool RemoveLight(GeometryWorldItem::GeometryLight::Light* item);
			virtual void InitializeGL();
			virtual void SetInitialSetup(const GeometryGlSetup::GlSetup& setup) { mpInitialSetup = setup.Clone(); }
			virtual bool AddRenderStep(const GeometryRenderStep::RenderStep& step);
			virtual bool RemoveRenderStep(int pos);
			virtual bool InsertRenderStep(const GeometryRenderStep::RenderStep& step, unsigned int pos);
			virtual void ClearRenderSteps();

		protected:
			std::unordered_set< GeometryWorldItem::GeometryItem::GeometryItem* > mItemList;// DO NOT DELETE
			SceneManager* mpParentManager; // DO NOT DELETE
			QVector4D mClearColor;
			std::unordered_set< GeometryWorldItem::GeometryCamera::Camera* > mCameras;// DO NOT DELETE
			std::unordered_set< GeometryWorldItem::GeometryLight::Light* > mLights;// DO NOT DELETE
			std::list<GeometryRenderStep::RenderStep * > mRenderSteps;
			GeometryGlSetup::GlSetup* mpInitialSetup;

			virtual void renderCamera(GeometryWorldItem::GeometryCamera::Camera * cam, const std::list<GeometryRenderStep::RenderStep * >& renderSteps);
		};
	}
}

#endif