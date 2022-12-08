#include "stdafx.h"

#include <GeometryEngine.h>
#include <GeometryScene.h>
#include <SceneManager.h>

#include <GeometryFactory.h>

#include <Items\GeometryItem.h>
#include <Items\GraphicItems\Lights\ShadowCastingLights\ShadowSpotlight.h>
#include <Items\GraphicItems\Camera.h>
#include <Items\GraphicItems\CameraUtils\ImgUtils.h>

#include <Items/Materials/Material.h>

#include <Items\CommonInerfaces\MaterialPostProcessInterface.h>

#include <Scenes/GeometryPostProcessScene.h>

#include "GeometryPostProcessTest.h"

const float UnitTest::CGeometryPostProcessTest::SECS = 50.0f;
const QVector3D UnitTest::CGeometryPostProcessTest::ROTATION = QVector3D(0.0f, 0.3f, 0.0f);
const QVector3D UnitTest::CGeometryPostProcessTest::MOVEMENT = QVector3D(0.1f, 0.0f, 0.1f);
const QVector3D UnitTest::CGeometryPostProcessTest::POINT = QVector3D(-5.0f, 0.0f, -15.0f);
const QVector3D UnitTest::CGeometryPostProcessTest::POS_LIGHT = QVector3D(-5.0f, 10.0f, -15.0f);

const std::string UnitTest::CGeometryPostProcessTest::GEOM_POST_PROCESS_TEST_IMAGE = "GEOM_POST_PROCESS_TEST_IMAGE";
const std::string UnitTest::CGeometryPostProcessTest::DEFAULT_LOG_IMAGE_NAME = "GeometryPostProcessTest";

UnitTest::CGeometryPostProcessTest::~CGeometryPostProcessTest()
{
	destroyGeometry();
}

void UnitTest::CGeometryPostProcessTest::InitGeometry(GeometryEngine::GeometryEngine * engine, int sceneWidth, int sceneHeight)
{
	mSceneWidth = sceneWidth;
	mSceneHeigth = sceneHeight;

	//Create the scene
	GeometryEngine::GeometryScene::GeometryScene* scene = engine->GetSceneManager()->CreateScene<GeometryEngine::GeometryScene::GeometryPostProcessScene>();

	// Create scene objects
	mpSphere = GeometryEngine::GeometryFactory::CreateSphere(
		GeometryEngine::GeometryFactory::CreateAlphaColorMaterial(QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f),
			QVector3D(0.0f, 0.0f, 0.0f), 0.7f, true, 100.0f),
		1.0, QVector3D(5.0f, 0.0f, -15.0f)
	);

	GeometryEngine::GeometryMaterial::Material* floorMat =
		GeometryEngine::GeometryFactory::CreateColorMaterial(QVector3D(0.9f, 0.9f, 0.9f), QVector3D(0.9f, 0.9f, 0.9f), QVector3D(0.9f, 0.9f, 0.9f));
	GeometryEngine::GeometryFactory::AddMaterialBlurPostProcess(floorMat, GeometryEngine::CustomShading::MaterialPostProcessSteps::FIRST_POST_PROCESS);
	mpFloorCube = GeometryEngine::GeometryFactory::CreateCube(floorMat, GeometryEngine::TEXTURE_MODE::REPEAT, 1.0f, QVector3D(0.0f, -6.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f),
		QVector3D(100.0f, 1.0f, 100.0f) );

	mpCube = GeometryEngine::GeometryFactory::CreateCube(
		GeometryEngine::GeometryFactory::CreateAlphaTextureMaterial(
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
			GeometryEngine::GeometryMaterial::TextureConstant::NORMALMAP_TEST_BLUE_CHIP,
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLACK_TEXTURE, 0.8f, true, 1000.0f),
		GeometryEngine::TEXTURE_MODE::REPEAT, 4.0f, QVector3D(-5.0f, 0.0f, -15.0f), QVector3D(30.0f, -30.0f, 0.0f)
	);

	GeometryEngine::GeometryMaterial::Material* grassMat =
		GeometryEngine::GeometryFactory::CreateAlphaTextureMaterial(GeometryEngine::GeometryMaterial::TextureConstant::TEST_GRASS_TEXTURE, 1.0f);
	grassMat->SetDrawBacksideFaces(true);
	GeometryEngine::GeometryFactory::AddMaterialGreyScalePostProcess(grassMat, GeometryEngine::CustomShading::MaterialPostProcessSteps::FIRST_POST_PROCESS);
	GeometryEngine::GeometryFactory::AddMaterialBlurPostProcess(grassMat, GeometryEngine::CustomShading::MaterialPostProcessSteps::SECOND_POST_PROCESS);
	mpQuad = GeometryEngine::GeometryFactory::CreateQuad(grassMat, 1.0f, 1.0f, QVector3D(0.0f, 0.0f, -15.0f), QVector3D(0.0f, 45.0f, 0.0f));

	mpSkyboxCube = GeometryEngine::GeometryFactory::CreateSkyboxCube(GeometryEngine::GeometryMaterial::TextureConstant::TEST_SKYBOX_TEXTURE, 200.0f);

	// lights
	mpSecondLight = GeometryEngine::GeometryFactory::CreateTranslucentSpotlight(QVector3D(-5.0f, 10.0f, -15.0f), QVector3D(0.0f, -1.0f, 0.0f),
		GeometryEngine::GeometryFactory::CreatePerspectiveViewport(QVector4D(0, 0, mSceneWidth, mSceneHeigth), 120.0f, 1.0f, 0.1f, 100.0f),
		QVector3D(1.0f, 1.0f, 1.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f), 45.0f, QVector3D(0.1f, 0.1f, 0.01f), QVector3D(0.0f, 0.0f, 0.0f), 0.0003f);

	mpMainLight = GeometryEngine::GeometryFactory::CreateDirectionalTranslucentLight(QVector3D(5.0f, 30.0f, -15.0f), QVector3D(0.0, -1.0, 0.0),
		GeometryEngine::GeometryFactory::CreateOrtographicViewport(QVector4D(0, 0, mSceneWidth, mSceneHeigth), QRect(-mSceneWidth / 24, -mSceneHeigth / 24,
			mSceneWidth / 12, mSceneHeigth / 12), 0.1f, 1000.0f),
		QVector3D(0.1f, 0.1f, 0.1f),
		QVector3D(0.1f, 0.1f, 0.1f), QVector3D(0.1f, 0.1f, 0.1f));

	//camera
	mpCam = GeometryEngine::GeometryFactory::CreateDeferredShadingCamera(GeometryEngine::GeometryFactory::CreatePerspectiveViewport(
		QVector4D(0, 0, mSceneWidth, mSceneHeigth), 45.0f, 1.0f, 0.1f, 1000.0f),
		QVector3D(0.0f, 0.0f, -3.5f)
	);

	// Add objects to the scene
	scene->AddItem(mpSkyboxCube);
	scene->AddItem(mpSphere);
	scene->AddItem(mpFloorCube);
	scene->AddItem(mpCube);
	scene->AddItem(mpQuad);

	scene->AddLight(mpMainLight);
	scene->AddLight(mpSecondLight);

	scene->AddCamera(mpCam);

	scene->InitializeGL();
	engine->GetSceneManager()->SetActiveScene(scene);
}

void UnitTest::CGeometryPostProcessTest::UpdateGeometry(qint64 tick)
{
	if (this->mpCube != nullptr) this->mpCube->SetRotation(SECS * ROTATION);

	QVector3D mov = MOVEMENT;
	if (mpSecondLight->GetPosition().z() < -65 || mpSecondLight->GetPosition().x() < -40) mov = MOVEMENT;
	if (mpSecondLight->GetPosition().z() > 35 || mpSecondLight->GetPosition().x() > 30) mov = -MOVEMENT;

	mpSecondLight->SetPosition(POS_LIGHT + mpSecondLight->ToModelCoordSystem(mov * SECS));

	QVector3D nDir(POINT - mpSecondLight->GetPosition());
	nDir.normalize();
	((GeometryEngine::GeometryWorldItem::GeometryLight::ShadowSpotlight*)(mpSecondLight))->SetDirection(nDir);

	evaluate();
}

void UnitTest::CGeometryPostProcessTest::copy(const CGeometryPostProcessTest & ref)
{
	CBaseGeometryTest::copy(ref);

	destroyGeometry();

	this->mpSphere = nullptr;
	this->mpCube = nullptr;
	this->mpQuad = nullptr;
	this->mpSkyboxCube = nullptr;
	this->mpFloorCube = nullptr;
	this->mpMainLight = nullptr;
	this->mpSecondLight = nullptr;
	this->mpCam = nullptr;
}

void UnitTest::CGeometryPostProcessTest::evaluate()
{
	if (!mScenePainted) return;
	mTestFinished = true;
	mTestSuccessful = true;
	mTestSuccessful &= compareScreenImage(mpConfManager->GetTextureDir(GEOM_POST_PROCESS_TEST_IMAGE), DEFAULT_LOG_IMAGE_NAME);
}

void UnitTest::CGeometryPostProcessTest::destroyGeometry()
{
	if (this->mpSphere != nullptr) delete this->mpSphere;
	if (this->mpCube != nullptr) delete this->mpCube;
	if (this->mpQuad != nullptr) delete this->mpQuad;
	if (this->mpSkyboxCube != nullptr) delete this->mpSkyboxCube;
	if (this->mpFloorCube != nullptr) delete this->mpFloorCube;
	if (this->mpMainLight != nullptr) delete this->mpMainLight;
	if (this->mpSecondLight != nullptr) delete this->mpSecondLight;
	if (this->mpCam != nullptr) delete this->mpCam;
}
