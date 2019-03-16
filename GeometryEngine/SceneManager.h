#pragma once

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <list>
#include <type_traits>
#include "GeometryScene.h"


namespace GeometryEngine
{
	namespace GeometryScene
	{
		class GeometryScene;
		class DeferredShadingScene;
	}
	

	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		GeometryScene::GeometryScene* CreateScene();
		template <typename T > GeometryScene::GeometryScene* CreateScene() { return new T(this); }
		bool AddScene(GeometryScene::GeometryScene* scene);
		bool RemoveScene(GeometryScene::GeometryScene* scene);
		bool RemoveScene(unsigned int sceneId);
		int GetSceneCount() { return mSceneList.size(); }
		bool SetActiveScene(GeometryScene::GeometryScene* scene);
		bool SetActiveScene(unsigned int sceneID);
		GeometryScene::GeometryScene* GetActiveScene() { return mpActiveScene; }
		void ClearScenes();

	protected:
		std::list<GeometryScene::GeometryScene*> mSceneList;
		GeometryScene::GeometryScene* mpActiveScene;
	};
}

#endif