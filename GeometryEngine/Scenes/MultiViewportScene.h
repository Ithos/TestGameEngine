#pragma once
#ifndef MULTIVIEWPORTSCENE_H
#define MULTIVIEWPORTSCENE_H

#include <map>
#include <qvector4d.h>
#include "../Items/GraphicItems/CameraUtils/CameraCommons.h"
#include "../GeometryScene.h"

namespace GeometryEngine
{
	class SceneManager;

	namespace GeometryScene
	{
		/// Scene that has multiple viewports for different main cameras
		class MultiViewportScene : public GeometryScene
		{
		public:
			/// Constructor. Here we define the initial setup and add the render steps for the scene
			/// \param manager Pointer to the scene manager
			/// \param viewportNumber Number of screen viewports
			/// \param fovy Field of view angle
			/// \param zNear Near plane z coordinate
			/// \param zFar Far plane z coordinate
			/// \param clearColor Clear color for the scene
			MultiViewportScene(SceneManager* manager, int viewportNumber = 2, GLdouble fovy = 45.0, GLdouble zNear = 0.1, GLdouble zFar = 30.0, 
				QVector4D clearColor = QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
			/// Constructor. Here we define the initial setup and add the render steps for the scene
			/// \param manager Pointer to the scene manager
			/// \param locationMap Map of camera targets and their positions and sizes
			/// \param fovy Field of view angle
			/// \param zNear Near plane z coordinate
			/// \param zFar Far plane z coordinate
			/// \param clearColor Clear color for the scene
			MultiViewportScene(SceneManager* manager, const std::map<GeometryWorldItem::GeometryCamera::CameraTargets, QVector4D>& locationMap, GLdouble fovy = 45.0, GLdouble zNear = 0.1, 
				GLdouble zFar = 30.0, QVector4D clearColor = QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
			/// Destructor
			virtual ~MultiViewportScene() {};
		};
	}
}

#endif // !MULTIVIEWPORTSCENE_H
