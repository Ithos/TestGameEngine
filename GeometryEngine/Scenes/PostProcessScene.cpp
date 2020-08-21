#include "../SceneManager.h"
#include "../Render Utils/RenderSteps/GeometryPass.h"
#include "../Render Utils/RenderSteps/LightingPass.h"
#include "../Render Utils//RenderSteps/EmissiveLightingPass.h"
#include "../Render Utils/RenderSteps/FinalPass.h"
#include "../Render Utils/RenderSteps/PostProcessPass.h"
#include "../Render Utils/GlSetups/InitialSetup.h"

#include "../Items/Materials/ColorMaterial.h"
#include "../Items/Geometries/Quad.h"
#include "../Items/GraphicItems/Lights/EmissiveLighting.h"

#include "../Items/GraphicItems/LightUtils/LightFunctionalities.h"
#include "../Items/GraphicItems//LightUtils/LightRenderTechniques/BoundingGeometryLighting.h"

#include "PostProcessScene.h"

GeometryEngine::GeometryScene::PostProcessScene::PostProcessScene(SceneManager * manager, GLdouble fovy, GLdouble zNear, GLdouble zFar, QVector4D clearColor) :
	GeometryScene(manager, fovy, zNear, zFar, clearColor)
{
	mpInitialSetup = new GeometryGlSetup::InitialSetup();

	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::GeometryPass());
	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::LightingPass());

	GeometryEngine::LightUtils::LightFunctionalities lightFuncs;
	lightFuncs.AddNewLightTechnique< GeometryEngine::LightUtils::BoundingGeometryLighting<GeometryEngine::GeometryWorldItem::GeometryLight::EmissiveLighting> >
		(GeometryEngine::LightUtils::BOUNDING_GEOMETRY);

	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::EmissiveLightingPass( GeometryEngine::GeometryWorldItem::GeometryLight::EmissiveLighting( 
		&GeometryEngine::GeometryWorldItem::GeometryItem::Quad(GeometryEngine::GeometryMaterial::ColorMaterial(), 3.0f, 3.0f ), &lightFuncs) ) );

	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::PostProcessPass());
	mRenderSteps.push_back(new GeometryEngine::GeometryRenderStep::FinalPass());
}
