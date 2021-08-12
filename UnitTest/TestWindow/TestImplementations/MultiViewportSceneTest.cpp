#include "stdafx.h"

#include <GeometryEngine.h>
#include <GeometryScene.h>
#include <SceneManager.h>

#include <GeometryFactory.h>

#include <Items\GeometryItem.h>
#include <Items\GraphicItems\Lights\ShadowCastingLights\ShadowSpotlight.h>
#include <Items\GraphicItems\Camera.h>
#include <Items\GraphicItems\CameraUtils\ImgUtils.h>

#include <Scenes/MultiViewportScene.h>

#include "MultiViewportSceneTest.h"

const float UnitTest::CMultiViewportSceneTest::SECS = 50.0f;
const QVector3D UnitTest::CMultiViewportSceneTest::ROTATION = QVector3D(0.0f, 0.3f, 0.0f);
const QVector3D UnitTest::CMultiViewportSceneTest::MOVEMENT = QVector3D(0.1f, 0.0f, 0.1f);
const QVector3D UnitTest::CMultiViewportSceneTest::POINT = QVector3D(-5.0f, 0.0f, -15.0f);
const QVector3D UnitTest::CMultiViewportSceneTest::POS_LIGHT = QVector3D(-5.0f, 10.0f, -15.0f);

const std::string UnitTest::CMultiViewportSceneTest::MULTI_SCENE_TEST_IMAGE = "MULTI_SCENE_TEST_IMAGE";
const std::string UnitTest::CMultiViewportSceneTest::DEFAULT_LOG_IMAGE_NAME = "MultiViewportTest";


UnitTest::CMultiViewportSceneTest::~CMultiViewportSceneTest()
{
	destroyGeometry();
}

void UnitTest::CMultiViewportSceneTest::InitGeometry(GeometryEngine::GeometryEngine * engine, int sceneWidth, int sceneHeight)
{
	mSceneWidth = sceneWidth;
	mSceneHeigth = sceneHeight;

	//Create the scene
	GeometryEngine::GeometryScene::GeometryScene* scene = new GeometryEngine::GeometryScene::MultiViewportScene(engine->GetSceneManager(), 4);

	// Create scene objects
	mpSphere = GeometryEngine::GeometryFactory::CreateSphere(
		GeometryEngine::GeometryFactory::CreateAlphaColorMaterial(QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f),
			QVector3D(0.0f, 0.0f, 0.0f), 0.7f, true, 100.0f),
		1.0, QVector3D(5.0f, 0.0f, -15.0f)
	);

	mpFloorCube = GeometryEngine::GeometryFactory::CreateCube(
		GeometryEngine::GeometryFactory::CreateColorMaterial(QVector3D(0.9f, 0.9f, 0.9f), QVector3D(0.9f, 0.9f, 0.9f), QVector3D(0.9f, 0.9f, 0.9f)),
		GeometryEngine::TEXTURE_MODE::REPEAT, 1.0f, QVector3D(0.0f, -6.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(100.0f, 1.0f, 100.0f)
	);

	mpCube = GeometryEngine::GeometryFactory::CreateCube(
		GeometryEngine::GeometryFactory::CreateAlphaTextureMaterial(
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
			GeometryEngine::GeometryMaterial::TextureConstant::NORMALMAP_TEST_BLUE_CHIP,
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLACK_TEXTURE, 0.8f, true, 1000.0f),
		GeometryEngine::TEXTURE_MODE::REPEAT, 4.0f, QVector3D(-5.0f, 0.0f, -15.0f), QVector3D(30.0f, -30.0f, 0.0f)
	);

	mpQuad = GeometryEngine::GeometryFactory::CreateQuad(
		GeometryEngine::GeometryFactory::CreateAlphaTextureMaterial(GeometryEngine::GeometryMaterial::TextureConstant::TEST_GRASS_TEXTURE, 1.0f, true),
		1.0f, 1.0f, QVector3D(0.0f, 0.0f, -15.0f), QVector3D(0.0f, 45.0f, 0.0f));

	mpSkyboxCube = GeometryEngine::GeometryFactory::CreateSkyboxCube(GeometryEngine::GeometryMaterial::TextureConstant::TEST_SKYBOX_TEXTURE, 200.0f);

	// lights
	mpSecondLight = GeometryEngine::GeometryFactory::CreateShadowSpotlight(QVector3D(-5.0f, 10.0f, -15.0f), QVector3D(0.0f, -1.0f, 0.0f),
		GeometryEngine::GeometryFactory::CreatePerspectiveViewport(QVector4D(0, 0, mSceneWidth, mSceneHeigth), 120.0f, 1.0f, 0.1f, 100.0f),
		QVector3D(1.0f, 1.0f, 1.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f), 45.0f, QVector3D(0.1f, 0.1f, 0.01f), QVector3D(0.0f, 0.0f, 0.0f), 0.0003f);

	mpMainLight = GeometryEngine::GeometryFactory::CreateDirectionalShadowLight(QVector3D(5.0f, 30.0f, -15.0f), QVector3D(0.0, -1.0, 0.0),
		GeometryEngine::GeometryFactory::CreateOrtographicViewport(QVector4D(0, 0, mSceneWidth, mSceneHeigth), QRect(-mSceneWidth / 24, -mSceneHeigth / 24,
			mSceneWidth / 12, mSceneHeigth / 12), 0.1f, 1000.0f),
		QVector3D(0.1f, 0.1f, 0.1f),
		QVector3D(0.1f, 0.1f, 0.1f), QVector3D(0.1f, 0.1f, 0.1f));

	//camera
	mpCam1 = GeometryEngine::GeometryFactory::CreateDeferredShadingCamera(GeometryEngine::GeometryFactory::CreatePerspectiveViewport(
		QVector4D(0, 0, mSceneWidth, mSceneHeigth), 45.0f, 1.0f, 0.1f, 1000.0f),
		QVector3D(0.0f, 0.0f, 0.0f)
	);

	mpCam1->SetRenderTarget(GeometryEngine::GeometryWorldItem::GeometryCamera::CAM1);

	mpCam2 = GeometryEngine::GeometryFactory::CreateDeferredShadingCamera(GeometryEngine::GeometryFactory::CreatePerspectiveViewport(
		QVector4D(0, 0, mSceneWidth, mSceneHeigth), 45.0f, 1.0f, 0.1f, 1000.0f),
		QVector3D(0.0f, 0.0f, 0.0f)
	);

	mpCam2->SetRenderTarget(GeometryEngine::GeometryWorldItem::GeometryCamera::CAM2);

	mpCam3 = GeometryEngine::GeometryFactory::CreateDeferredShadingCamera(GeometryEngine::GeometryFactory::CreatePerspectiveViewport(
		QVector4D(0, 0, mSceneWidth, mSceneHeigth), 45.0f, 1.0f, 0.1f, 1000.0f),
		QVector3D(0.0f, 0.0f, 0.0f)
	);

	mpCam3->SetRenderTarget(GeometryEngine::GeometryWorldItem::GeometryCamera::CAM3);

	mpCam4 = GeometryEngine::GeometryFactory::CreateDeferredShadingCamera(GeometryEngine::GeometryFactory::CreatePerspectiveViewport(
		QVector4D(0, 0, mSceneWidth, mSceneHeigth), 45.0f, 1.0f, 0.1f, 1000.0f),
		QVector3D(0.0f, 0.0f, 0.0f)
	);

	mpCam4->SetRenderTarget(GeometryEngine::GeometryWorldItem::GeometryCamera::CAM4);

	// Add objects to the scene
	scene->AddItem(mpSkyboxCube);
	scene->AddItem(mpSphere);
	scene->AddItem(mpFloorCube);
	scene->AddItem(mpCube);
	scene->AddItem(mpQuad);

	scene->AddLight(mpMainLight);
	scene->AddLight(mpSecondLight);

	scene->AddCamera(mpCam1);
	scene->AddCamera(mpCam2);
	scene->AddCamera(mpCam3);
	scene->AddCamera(mpCam4);

	scene->InitializeGL();
	engine->GetSceneManager()->SetActiveScene(scene);
}

void UnitTest::CMultiViewportSceneTest::UpdateGeometry(qint64 tick)
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

void UnitTest::CMultiViewportSceneTest::copy(const CMultiViewportSceneTest & ref)
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
	this->mpCam1 = nullptr;
	this->mpCam2 = nullptr;
	this->mpCam3 = nullptr;
	this->mpCam4 = nullptr;
}

void UnitTest::CMultiViewportSceneTest::evaluate()
{
	if (!mScenePainted) return;
	mTestFinished = true;
	mTestSuccessful = true;
	mTestSuccessful &= compareScreenImage(mpConfManager->GetTextureDir(MULTI_SCENE_TEST_IMAGE), DEFAULT_LOG_IMAGE_NAME);
}

void UnitTest::CMultiViewportSceneTest::destroyGeometry()
{
	if (this->mpSphere != nullptr) delete this->mpSphere;
	if (this->mpCube != nullptr) delete this->mpCube;
	if (this->mpQuad != nullptr) delete this->mpQuad;
	if (this->mpSkyboxCube != nullptr) delete this->mpSkyboxCube;
	if (this->mpFloorCube != nullptr) delete this->mpFloorCube;
	if (this->mpMainLight != nullptr) delete this->mpMainLight;
	if (this->mpSecondLight != nullptr) delete this->mpSecondLight;
	if (this->mpCam1 != nullptr) delete this->mpCam1;
	if (this->mpCam2 != nullptr) delete this->mpCam2;
	if (this->mpCam3 != nullptr) delete this->mpCam3;
	if (this->mpCam4 != nullptr) delete this->mpCam4;
}
