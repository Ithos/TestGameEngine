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
	class WorldItem;
	class Light;
	class Camera;
	class RenderStep;
	class GlSetup;

	class GeometryScene
	{
	public:
		GeometryScene(SceneManager* manager, GLdouble fovy = 45.0, GLdouble zNear = 0.1, GLdouble zFar = 30.0, QVector4D clearColor = QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
		virtual ~GeometryScene();

		virtual void ResizeScene(int w, int h, int formerW, int formerH);
		virtual void Draw();
		virtual bool AddItem(GeometryItem* item);
		virtual bool RemoveItem(GeometryItem* item);
		virtual bool AddCamera(Camera* item);
		virtual bool RemoveCamera(Camera* item);
		virtual bool AddLight(Light* item);
		virtual bool RemoveLight(Light* item);
		virtual void InitializeGL();
		virtual void SetInitialSetup(const GlSetup& setup) { mpInitialSetup = setup.Clone(); }
		virtual bool AddRenderStep(const RenderStep& step);
		virtual bool RemoveRenderStep(int pos);
		virtual bool InsertRenderStep(const RenderStep& step, unsigned int pos);
		virtual void ClearRenderSteps();

	protected:
		std::unordered_set< GeometryItem* > mItemList;// DO NOT DELETE
		SceneManager* mpParentManager; // DO NOT DELETE
		QVector4D mClearColor;
		std::unordered_set< Camera* > mCameras;// DO NOT DELETE
		std::unordered_set< Light* > mLights;// DO NOT DELETE
		std::list<RenderStep * > mRenderSteps;
		GlSetup* mpInitialSetup;

		virtual void renderCamera(Camera * cam, const std::list<RenderStep * >& renderSteps);
	};
}

#endif