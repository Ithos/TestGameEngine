#include "stdafx.h"

#include <GeometryEngine.h>
#include <GeometryScene.h>
#include <SceneManager.h>

#include <GeometryFactory.h>

#include <Items\GeometryItem.h>
#include <Items\GraphicItems\Lights\ShadowCastingLights\ShadowSpotlight.h>

#include <Scenes/TransparentGeometryScene.h>

#include "TranslucentShadowingTest.h"

const float UnitTest::CTranslucentShadowingTest::SECS = 50.0f;
const QVector3D UnitTest::CTranslucentShadowingTest::ROTATION = QVector3D(0.0f, 0.3f, 0.0f);
const QVector3D UnitTest::CTranslucentShadowingTest::MOVEMENT = QVector3D(0.1f, 0.0f, 0.1f);
const QVector3D UnitTest::CTranslucentShadowingTest::POINT = QVector3D(-5.0f, 0.0f, -15.0f);
const QVector3D UnitTest::CTranslucentShadowingTest::POS_LIGHT = QVector3D(-5.0f, 10.0f, -15.0f);

UnitTest::CTranslucentShadowingTest::~CTranslucentShadowingTest()
{
	destroyGeometry();
}

void UnitTest::CTranslucentShadowingTest::InitGeometry(GeometryEngine::GeometryEngine * engine, int sceneWidth, int sceneHeight)
{
	mSceneWidth = sceneWidth;
	mSceneHeigth = sceneHeight;

	//Create the scene
	GeometryEngine::GeometryScene::GeometryScene* scene = engine->GetSceneManager()->CreateScene<GeometryEngine::GeometryScene::TransparentGeometryScene>();

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
	mpCam = GeometryEngine::GeometryFactory::CreateDeferredShadingCamera(GeometryEngine::GeometryFactory::CreatePerspectiveViewport(
		QVector4D(0, 0, mSceneWidth, mSceneHeigth), 45.0f, 1.0f, 0.1f, 1000.0f),
		QVector3D(0.0f, 0.0f, 0.0f)
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

void UnitTest::CTranslucentShadowingTest::UpdateGeometry(qint64 tick)
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

void UnitTest::CTranslucentShadowingTest::copy(const CTranslucentShadowingTest & ref)
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

void UnitTest::CTranslucentShadowingTest::evaluate()
{
	if (!mScenePainted) return;

	unsigned char pixel[4];

	mTestFinished = true;
	mTestSuccessful = true;

	// Mid line check
	glReadPixels((GLint)(mSceneWidth / 2), (GLint)(mSceneHeigth / 2), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 21 && pixel[1] == 31 && pixel[2] == 2 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth / 3), (GLint)(mSceneHeigth / 2), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 241 && pixel[1] == 127 && pixel[2] == 6 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth * 2 / 3), (GLint)(mSceneHeigth / 2), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 236 && pixel[1] == 133 && pixel[2] == 2 && pixel[3] == 204;

	// Extra top line check
	glReadPixels((GLint)(mSceneWidth / 2), (GLint)(mSceneHeigth * 3 / 4), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 233 && pixel[1] == 128 && pixel[2] == 0 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth / 3), (GLint)(mSceneHeigth * 3 / 4), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 236 && pixel[1] == 132 && pixel[2] == 0 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth * 2 / 3), (GLint)(mSceneHeigth * 3 / 4), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 247 && pixel[1] == 165 && pixel[2] == 1 && pixel[3] == 204;

	// Down line checks

	glReadPixels((GLint)(mSceneWidth / 2), (GLint)(mSceneHeigth / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 54 && pixel[1] == 54 && pixel[2] == 54 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth / 3), (GLint)(mSceneHeigth / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 54 && pixel[1] == 54 && pixel[2] == 54 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth * 2 / 3), (GLint)(mSceneHeigth / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 47 && pixel[1] == 47 && pixel[2] == 47 && pixel[3] == 204;

	// Extra down line check
	glReadPixels((GLint)(mSceneWidth / 5), (GLint)(mSceneHeigth / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 11 && pixel[1] == 24 && pixel[2] == 34 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth / 4), (GLint)(mSceneHeigth / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 12 && pixel[1] == 23 && pixel[2] == 30 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth * 0.72f), (GLint)(mSceneHeigth * 0.13f), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 87 && pixel[1] == 87 && pixel[2] == 87 && pixel[3] == 204;

	// Top line checks

	glReadPixels((GLint)(mSceneWidth / 2), (GLint)(mSceneHeigth * 2 / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 244 && pixel[1] == 139 && pixel[2] == 1 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth / 3), (GLint)(mSceneHeigth * 2 / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 226 && pixel[1] == 98 && pixel[2] == 1 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth * 2 / 3), (GLint)(mSceneHeigth * 2 / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 251 && pixel[1] == 182 && pixel[2] == 1 && pixel[3] == 204;
}

void UnitTest::CTranslucentShadowingTest::destroyGeometry()
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
