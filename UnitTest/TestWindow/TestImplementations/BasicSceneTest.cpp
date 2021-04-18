#include "stdafx.h"

#include <GeometryEngine.h>
#include <GeometryScene.h>
#include <SceneManager.h>

#include <GeometryFactory.h>

#include <Items/Materials/TextureMaterialCommons.h>
#include <Items\GeometryItem.h>
#include <Items\GraphicItems\Lights\Spotlight.h>

#include <Scenes/DeferredShadingScene.h>

#include "BasicSceneTest.h"

const float UnitTest::CBasicSceneTest::SECS = 50.0f;
const QVector3D UnitTest::CBasicSceneTest::ROTATION = QVector3D(0.0f, 0.3f, 0.0f);
const QVector3D UnitTest::CBasicSceneTest::MOVEMENT = QVector3D(0.1f, 0.0f, 0.1f);
const QVector3D UnitTest::CBasicSceneTest::POINT = QVector3D(-5.0f, 0.0f, -15.0f);
const QVector3D UnitTest::CBasicSceneTest::POS_LIGHT = QVector3D(-5.0f, 10.0f, -15.0f);

UnitTest::CBasicSceneTest::~CBasicSceneTest()
{
	destroyGeometry();
}

void UnitTest::CBasicSceneTest::InitGeometry(GeometryEngine::GeometryEngine * engine, int sceneWidth, int sceneHeight)
{
	mSceneWidth = sceneWidth;
	mSceneHeigth = sceneHeight;

	//Create the scene
	GeometryEngine::GeometryScene::GeometryScene* scene = engine->GetSceneManager()->CreateScene<GeometryEngine::GeometryScene::DeferredShadingScene>();

	// Create scene objects
	mpSphere = GeometryEngine::GeometryFactory::CreateSphere(
		GeometryEngine::GeometryFactory::CreateColorMaterial(QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 0.0f, 0.0f), 100.0f),
		1.0, QVector3D(5.0f, 0.0f, -15.0f)
	);

	mpFloorCube = GeometryEngine::GeometryFactory::CreateCube(
		GeometryEngine::GeometryFactory::CreateColorMaterial(QVector3D(0.9f, 0.9f, 0.9f), QVector3D(0.9f, 0.9f, 0.9f), QVector3D(0.9f, 0.9f, 0.9f)),
		GeometryEngine::TEXTURE_MODE::REPEAT, 1.0f, QVector3D(0.0f, -6.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(100.0f, 1.0f, 100.0f)
	);

	mpCube = GeometryEngine::GeometryFactory::CreateCube(
		GeometryEngine::GeometryFactory::CreateTextureMaterial(
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE, 
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE, 
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE, 
			GeometryEngine::GeometryMaterial::TextureConstant::NORMALMAP_TEST_BLUE_CHIP,
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLACK_TEXTURE, 1000.0f),
		GeometryEngine::TEXTURE_MODE::REPEAT, 4.0f, QVector3D(-5.0f, 0.0f, -15.0f), QVector3D(30.0f, -30.0f, 0.0f)
	);

	mpSkyboxCube = GeometryEngine::GeometryFactory::CreateSkyboxCube(GeometryEngine::GeometryMaterial::TextureConstant::TEST_SKYBOX_TEXTURE, 200.0f);

	// lights

	mpSecondLight = GeometryEngine::GeometryFactory::CreateSpotlight(QVector3D(-5.0f, 10.0f, -15.0f), QVector3D(0.0f, -1.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f),
		QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f));

	mpMainLight = GeometryEngine::GeometryFactory::CreateDirectionalLight(QVector3D(5.0f, 30.0f, -15.0f), QVector3D(0.0, -1.0, 0.0), QVector3D(0.1f, 0.1f, 0.1f),
		QVector3D(0.1f, 0.1f, 0.1f), QVector3D(0.1f, 0.1f, 0.1f));

	//camera

	mpCam = GeometryEngine::GeometryFactory::CreateDeferredShadingCamera(GeometryEngine::GeometryFactory::CreatePerspectiveViewport(
		QVector4D(0, 0, mSceneWidth, mSceneHeigth), 45.0f, 1.0f, 0.1f, 1000.0f),
		QVector3D(0.0f, 0.0f, 0.0f)
	);

	// Add objects to the scene
	scene->AddItem(mpSkyboxCube);
	scene->AddItem(mpSphere);
	scene->AddItem(mpFloorCube);
	scene->AddItem(mpCube);

	scene->AddLight(mpMainLight);
	scene->AddLight(mpSecondLight);

	scene->AddCamera(mpCam);

	scene->InitializeGL();
	engine->GetSceneManager()->SetActiveScene(scene);
}

void UnitTest::CBasicSceneTest::UpdateGeometry(qint64 tick)
{
	if (this->mpCube != nullptr) this->mpCube->SetRotation( SECS * ROTATION);

	QVector3D mov = MOVEMENT;
	if (mpSecondLight->GetPosition().z() < -65 || mpSecondLight->GetPosition().x() < -40) mov = MOVEMENT;
	if (mpSecondLight->GetPosition().z() > 35 || mpSecondLight->GetPosition().x() > 30) mov = -MOVEMENT;

	mpSecondLight->SetPosition(POS_LIGHT + mpSecondLight->ToModelCoordSystem(mov * SECS));

	QVector3D nDir(POINT - mpSecondLight->GetPosition());
	nDir.normalize();
	((GeometryEngine::GeometryWorldItem::GeometryLight::Spotlight*)(mpSecondLight))->SetDirection(nDir);

	evaluate();
}

void UnitTest::CBasicSceneTest::copy(const CBasicSceneTest & ref)
{
	CBaseGeometryTest::copy(ref);
	
	destroyGeometry();
	
	this->mpSphere = nullptr;
	this->mpCube = nullptr;
	this->mpSkyboxCube = nullptr;
	this->mpFloorCube = nullptr;
	this->mpMainLight = nullptr;
	this->mpSecondLight = nullptr;
	this->mpCam = nullptr;
}

void UnitTest::CBasicSceneTest::evaluate()
{
	if (!mScenePainted) return;

	unsigned char pixel[4];

	mTestFinished = true;
	mTestSuccessful = true;

	// Mid line check
	glReadPixels( (GLint)(mSceneWidth/2), (GLint)(mSceneHeigth / 2), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 225 && pixel[1] == 121 && pixel[2] == 0 && pixel[3] == 204;

	glReadPixels( (GLint)(mSceneWidth / 3), (GLint)(mSceneHeigth / 2), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 241 && pixel[1] == 127 && pixel[2] == 6 && pixel[3] == 204;
	
	glReadPixels((GLint)(mSceneWidth * 2 / 3), (GLint)(mSceneHeigth / 2), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 236 && pixel[1] == 133 && pixel[2] == 2 && pixel[3] == 204;

	// Down line checks

	glReadPixels((GLint)(mSceneWidth / 2), (GLint)(mSceneHeigth / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 75 && pixel[1] == 75 && pixel[2] == 75 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth / 3), (GLint)(mSceneHeigth / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 74 && pixel[1] == 74 && pixel[2] == 74 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth * 2 / 3), (GLint)(mSceneHeigth / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 63 && pixel[1] == 63 && pixel[2] == 63 && pixel[3] == 204;

	// Top line checks

	glReadPixels((GLint)(mSceneWidth / 2), (GLint)(mSceneHeigth * 2 / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 244 && pixel[1] == 139 && pixel[2] == 1 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth / 3), (GLint)(mSceneHeigth * 2 / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 226 && pixel[1] == 98 && pixel[2] == 1 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth * 2 / 3), (GLint)(mSceneHeigth * 2 / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 251 && pixel[1] == 182 && pixel[2] == 1 && pixel[3] == 204;
}

void UnitTest::CBasicSceneTest::destroyGeometry()
{
	if (this->mpSphere != nullptr) delete this->mpSphere;
	if (this->mpCube != nullptr) delete this->mpCube;
	if (this->mpSkyboxCube != nullptr) delete this->mpSkyboxCube;
	if (this->mpFloorCube != nullptr) delete this->mpFloorCube;
	if (this->mpMainLight != nullptr) delete this->mpMainLight;
	if (this->mpSecondLight != nullptr) delete this->mpSecondLight;
	if (this->mpCam != nullptr) delete this->mpCam;
}
