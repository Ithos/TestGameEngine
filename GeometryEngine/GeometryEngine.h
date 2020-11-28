#pragma once

#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

/* QTDIR se define en vcxproj.user, hay que definir la carpeta correcta para configuración*/
///Main namespace it contains all GeometryEngine elements.
namespace GeometryEngine
{
	class SceneManager;
	/// Singleton that gives acces to the GeometryEngine
	class GeometryEngine
	{
	public:
		/// Returnss the pointer to the singleton. It builds the object if the singleton hasn't been built before.
		static GeometryEngine* GetInstance();
		/// Destroys the pointer to the singleton.
		static void Release();
		/// Returns a pointer to the scene manager.
		SceneManager* GetSceneManager() { return mpSceneManager; }

	protected:
		/// Constructor. Calls init()
		GeometryEngine();
		/// Destructor. Destroys the object.
		virtual ~GeometryEngine();
		/// Deinitializes the object.
		void close();
		/// Builds the scene manager
		void init();

		static GeometryEngine* mpInstance;
		SceneManager* mpSceneManager;
	};
}

#endif // GEOMETRYENGINE_H
