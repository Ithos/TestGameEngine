#pragma once

#ifndef  POSTPROCESSSCENE_H
#define POSTPROCESSSCENE_H

#include "../GeometryScene.h"

#include "../Render Utils/RenderSteps/GeometryPass.h"
#include "../Render Utils/RenderSteps/LightingPass.h"
#include "../Render Utils/RenderSteps/FinalPass.h"
#include "../Render Utils/RenderSteps/SinglePostProcessPass.h"
#include "../Render Utils/RenderSteps/DoublePostProcessPass.h"
#include "../Render Utils/GlSetups/InitialSetup.h"


namespace GeometryEngine
{
	class SceneManager;

	namespace GeometryScene
	{
		class GeometryScene;

		class PostProcessScene : public GeometryScene
		{
		public:
			PostProcessScene(SceneManager* manager, GLdouble fovy = 45.0, GLdouble zNear = 0.1, GLdouble zFar = 30.0, QVector4D clearColor = QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
			virtual ~PostProcessScene() {};
		protected:
		};
	}
}

#endif // ! POSTPROCESSSCENE_H

