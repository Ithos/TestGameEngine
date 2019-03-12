#pragma once
#ifndef DEFERREDSHADINGSCENE_H
#define DEFERREDSHADINGSCENE_H

#include "../GeometryScene.h"

#include "../Render Utils/RenderSteps/GeometryPass.h"
#include "../Render Utils/RenderSteps/LightingPass.h"
#include "../Render Utils/RenderSteps/FinalPass.h"
#include "../Render Utils/GlSetups/InitialSetup.h"

namespace GeometryEngine
{
	class GeometryScene;
	class SceneManager;

	class DeferredShadingScene : public GeometryScene
	{
	public:
		DeferredShadingScene(SceneManager* manager, GLdouble fovy = 45.0, GLdouble zNear = 0.1, GLdouble zFar = 30.0, QVector4D clearColor = QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
		virtual ~DeferredShadingScene() {};
	protected:
	};
}

#endif