#pragma once

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <list>
#include <type_traits>

namespace GeometryEngine
{
	namespace GeometryScene
	{
		class GeometryScene;
		class DeferredShadingScene;
	}
	
	/// Class that contains all scenes in the engine and manages swaps between active scenes
	class SceneManager
	{
	public:
		/// Constructor
		SceneManager();
		/// Destructor. Destroys all scenes contained in the manager.
		~SceneManager();

		/// Factory method. Builds a new GeometryScene and adds it to this manager.
		/// \return Pointer to the new scene
		GeometryScene::GeometryScene* CreateScene();
		/// Factory method. Builds a new object that inherits from GeometryScene and adds it to this manager.
		/// \return Pointer to the new scene
		template <typename T > GeometryScene::GeometryScene* CreateScene() { return new T(this); }
		/// Adds a scene to the manager's list
		/// \param Pointer to the scene to be added
		/// \return false if the manager already contains the scene true otherwise
		bool AddScene(GeometryScene::GeometryScene* scene);
		/// Remove a scene from the manager's list
		/// \param scene Pointer to the scene to be removed
		/// \return false if the manager doesn't contain the scene true otherwise
		bool RemoveScene(GeometryScene::GeometryScene* scene);
		/// Remove a scene from the manager's list by index
		/// \param sceneId Index of the scene in the scene list
		/// \return false if the manager doesn't contain the scene true otherwise
		bool RemoveScene(unsigned int sceneId);
		/// Get the number of scenes in the manager
		/// \return Amount of scenes in the manager scene list
		int GetSceneCount() { return mSceneList.size(); }
		/// Set the active scene
		/// \param scene Pointer to the scene
		/// \return false if the manager doesn't contain the scene true otherwise
		bool SetActiveScene(GeometryScene::GeometryScene* scene);
		/// Set the active scene by index
		/// \param sceneID Index of the scene in the scene list
		/// \return false if the manager doesn't contain the scene true otherwise
		bool SetActiveScene(unsigned int sceneID);
		/// Returns a pointer to the active scene
		/// \return Pointer to the currently active scene 
		GeometryScene::GeometryScene* GetActiveScene() { return mpActiveScene; }
		/// Deletes all scenes in the manager
		void ClearScenes();

	protected:
		std::list<GeometryScene::GeometryScene*> mSceneList;
		GeometryScene::GeometryScene* mpActiveScene;
	};
}

#endif