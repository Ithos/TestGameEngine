#pragma once

#ifndef DYNAMICSHADOWSSCENE_H
#define DYNAMICSHADOWSSCENE_H

#include "../GeometryScene.h"

namespace GeometryEngine
{
	class SceneManager;

	namespace GeometryScene
	{
		/// Scene that includes geometry rendering, deferred shading lighting and scene wide image postprocessing
		class DynamicShadowsScene : public GeometryScene
		{
		public:
			/// Constructor. Here we define the initial setup and add the render steps for the scene
			/// param manager Pointer to the scene manager
			/// param fovy Field of view angle
			/// param zNear Near plane z coordinate
			/// param zFar Far plane z coordinate
			/// param clearColor Clear color for the scene
			DynamicShadowsScene(SceneManager* manager, GLdouble fovy = 45.0, GLdouble zNear = 0.1, GLdouble zFar = 30.0, QVector4D clearColor = QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
			/// Destructor
			virtual ~DynamicShadowsScene() {};
		protected:
		};
	}
}

#endif // ! DYNAMICSHADOWSSCENE_H
