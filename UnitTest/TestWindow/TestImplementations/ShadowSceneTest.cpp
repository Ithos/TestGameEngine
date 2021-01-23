#include "stdafx.h"

#include <GeometryEngine.h>
#include <GeometryScene.h>
#include <SceneManager.h>

#include <Items/Materials/ColorMaterial.h>
#include <Items/Materials/NormalMapMultiTextureMaterial.h>
#include <Items/Materials/SkyboxMaterial.h>
#include <Items/Materials/TextureMaterialCommons.h>

#include <Items\Geometries\Quad.h>
#include <Items\Geometries\Sphere.h>
#include <Items\Geometries\Cube.h>

#include <Items\GraphicItems\Lights\ShadowCastingLights\DirectionalShadowLight.h>
#include <Items\GraphicItems\Lights\ShadowCastingLights\ShadowSpotlight.h>
#include <Items\GraphicItems\Cameras\DeferredShadingCamera.h>

#include <Items\Item Utils\Viewports\PerspectiveViewport.h>
#include <Items\Item Utils\Viewports\OrtographicViewport.h>

#include <Render Utils\RenderBuffersData.h>
#include <Render Utils\Gbuffers\CompleteColorPostProcessBuffer.h>
#include <Render Utils\ShadingBuffer.h>

#include <Scenes/DynamicShadowsScene.h>

#include "ShadowSceneTest.h"

const float UnitTest::CShadowSceneTest::SECS = 50.0f;
const QVector3D UnitTest::CShadowSceneTest::ROTATION = QVector3D(0.0f, 0.3f, 0.0f);
const QVector3D UnitTest::CShadowSceneTest::MOVEMENT = QVector3D(0.1f, 0.0f, 0.1f);
const QVector3D UnitTest::CShadowSceneTest::POINT = QVector3D(-5.0f, 0.0f, -15.0f);
const QVector3D UnitTest::CShadowSceneTest::POS_LIGHT = QVector3D(-5.0f, 10.0f, -15.0f);

UnitTest::CShadowSceneTest::~CShadowSceneTest()
{
	destroyGeometry();
}

void UnitTest::CShadowSceneTest::InitGeometry(GeometryEngine::GeometryEngine * engine, int sceneWidth, int sceneHeight)
{
	mSceneWidth = sceneWidth;
	mSceneHeigth = sceneHeight;

	//Create the scene
	GeometryEngine::GeometryScene::GeometryScene* scene = engine->GetSceneManager()->CreateScene<GeometryEngine::GeometryScene::DynamicShadowsScene>();

	// Create scene objects
	GeometryEngine::GeometryMaterial::ColorMaterial sphereMat(QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 0.0f, 0.0f), 100.0f);
	mpSphere = new GeometryEngine::GeometryWorldItem::GeometryItem::Sphere(sphereMat, 1.0f, 6, 12, QVector3D(5.0f, 0.0f, -15.0f));

	GeometryEngine::GeometryMaterial::ColorMaterial floorMat(QVector3D(0.9f, 0.9f, 0.9f), QVector3D(0.9f, 0.9f, 0.9f), QVector3D(0.9f, 0.9f, 0.9f));
	mpFloorCube = new GeometryEngine::GeometryWorldItem::GeometryItem::Cube(floorMat, 1.0f, QVector3D(0.0f, -6.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(100.0f, 1.0f, 100.0f));

	GeometryEngine::GeometryMaterial::NormalMapMultiTextureMaterial cubeMat(GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
		GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
		GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
		GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLACK_TEXTURE,
		GeometryEngine::GeometryMaterial::TextureConstant::NORMALMAP_TEST_BLUE_CHIP, 1000.0f);
	mpCube = new GeometryEngine::GeometryWorldItem::GeometryItem::Cube(cubeMat, 4.0f, QVector3D(-5.0f, 0.0f, -15.0f), QVector3D(30.0f, -30.0f, 0.0f));


	std::vector<QVector2D> texCoordArray;
	// FRONT
	texCoordArray.push_back(QVector2D(1.0f, 0.3333f)); texCoordArray.push_back(QVector2D(0.75f, 0.3333f));
	texCoordArray.push_back(QVector2D(1.0f, 0.66666f)); texCoordArray.push_back(QVector2D(0.75f, 0.66666f));
	// RIGHT
	texCoordArray.push_back(QVector2D(0.75f, 0.3333f)); texCoordArray.push_back(QVector2D(0.5f, 0.3333f));
	texCoordArray.push_back(QVector2D(0.75f, 0.6666f)); texCoordArray.push_back(QVector2D(0.5f, 0.6666f));
	// BACK
	texCoordArray.push_back(QVector2D(0.5f, 0.3333f)); texCoordArray.push_back(QVector2D(0.25f, 0.3333f));
	texCoordArray.push_back(QVector2D(0.5f, 0.66666f)); texCoordArray.push_back(QVector2D(0.25f, 0.6666f));
	// LEFT
	texCoordArray.push_back(QVector2D(0.25f, 0.3333f)); texCoordArray.push_back(QVector2D(0.0f, 0.3333f));
	texCoordArray.push_back(QVector2D(0.25f, 0.6666f)); texCoordArray.push_back(QVector2D(0.0f, 0.6666f));
	// BOTTOM
	texCoordArray.push_back(QVector2D(0.25f, 0.3333f)); texCoordArray.push_back(QVector2D(0.5f, 0.3333f));
	texCoordArray.push_back(QVector2D(0.25f, 0.0f)); texCoordArray.push_back(QVector2D(0.5f, 0.0f));
	// TOP
	texCoordArray.push_back(QVector2D(0.25f, 1.0f)); texCoordArray.push_back(QVector2D(0.5f, 1.0f));
	texCoordArray.push_back(QVector2D(0.25f, 0.6666f)); texCoordArray.push_back(QVector2D(0.5f, 0.66666f));

	GeometryEngine::GeometryMaterial::SkyboxMaterial skyboxMat(GeometryEngine::GeometryMaterial::TextureConstant::TEST_SKYBOX_TEXTURE);
	mpSkyboxCube = new GeometryEngine::GeometryWorldItem::GeometryItem::Cube(skyboxMat, 200.0f, QVector3D(.0f, .0f, .0f), QVector3D(.0f, .0f, .0f), QVector3D(1.0f, 1.0f, 1.0f), nullptr, &texCoordArray);
	mpSkyboxCube->SetCastsShadows(false);

	// lights

	GeometryEngine::GeometryItemUtils::PerspectiveViewport lightViewport(QVector4D(0, 0, mSceneWidth, mSceneHeigth), 120.0f, 1.0f, 0.1f, 100.0f);
	GeometryEngine::GeometryItemUtils::OrtographicViewport lightViewportOrto(QVector4D(0, 0, mSceneWidth, mSceneHeigth), QRect(-mSceneWidth / 24, -mSceneHeigth / 24, 
																				mSceneWidth / 12, mSceneHeigth / 12), 0.1f, 1000.0f);


	GeometryEngine::GeometryWorldItem::GeometryItem::Sphere lightSphere(floorMat, 2.0f);
	GeometryEngine::GeometryWorldItem::GeometryItem::Quad lightQuad(floorMat, 3.0f, 3.0f);

	mpSecondLight = new GeometryEngine::GeometryWorldItem::GeometryLight::ShadowSpotlight(45.0f, QVector3D(0.1f, 0.1f, 0.01f), lightViewport, QVector3D(0.0f, -1.0f, 0.0f), &lightSphere, QVector3D(1.0f, 1.0f, 1.0f),
		QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f), POS_LIGHT, QVector3D(0.0f, 0.0f, 0.0f), 0.0003f, QVector3D(1.0f, 1.0f, 1.0f));

	mpMainLight = new GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalShadowLight(lightViewportOrto, QVector3D(0.0, -1.0, 0.0), &lightQuad, QVector3D(0.1f, 0.1f, 0.1f),
		QVector3D(0.1f, 0.1f, 0.1f), QVector3D(0.1f, 0.1f, 0.1f), QVector3D(5.0f, 30.0f, -15.0f), QVector3D(0.0f, 0.0f, 0.0f), 0.001f, QVector3D(1.0f, 1.0f, 1.0f));

	//camera

	GeometryEngine::GeometryItemUtils::PerspectiveViewport viewport(QVector4D(0, 0, mSceneWidth, mSceneHeigth), 45.0f, 1.0f, 0.1f, 1000.0f);

	mpCam = new GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera(
		GeometryEngine::GeometryRenderData::RenderBuffersData(GeometryEngine::GeometryBuffer::CompleteColorPostProcessBuffer(), &GeometryEngine::GeometryBuffer::ShadingBuffer()),
		viewport, true,
		QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0, 0, 0));

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

void UnitTest::CShadowSceneTest::UpdateGeometry(qint64 tick)
{
	if (this->mpCube != nullptr) this->mpCube->SetRotation(SECS * ROTATION);

	QVector3D mov = MOVEMENT;
	if (mpSecondLight->GetPosition().z() < -65 || mpSecondLight->GetPosition().x() < -40) mov = MOVEMENT;
	if (mpSecondLight->GetPosition().z() > 35 || mpSecondLight->GetPosition().x() > 30) mov = -MOVEMENT;

	mpSecondLight->SetPosition( POS_LIGHT + mpSecondLight->ToModelCoordSystem(mov * SECS));

	QVector3D nDir(POINT - mpSecondLight->GetPosition());
	nDir.normalize();
	((GeometryEngine::GeometryWorldItem::GeometryLight::ShadowSpotlight*)(mpSecondLight))->SetDirection(nDir);

	evaluate();
}

void UnitTest::CShadowSceneTest::copy(const CShadowSceneTest & ref)
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

void UnitTest::CShadowSceneTest::evaluate()
{
	if (!mScenePainted) return;

	unsigned char pixel[4];

	mTestFinished = true;
	mTestSuccessful = true;

	// Mid line check
	glReadPixels((GLint)(mSceneWidth / 2), (GLint)(mSceneHeigth / 2), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 225 && pixel[1] == 121 && pixel[2] == 0 && pixel[3] == 204;

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
	mTestSuccessful &= pixel[0] == 1 && pixel[1] == 21 && pixel[2] == 34 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth / 4), (GLint)(mSceneHeigth / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 3 && pixel[1] == 18 && pixel[2] == 29 && pixel[3] == 204;

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

void UnitTest::CShadowSceneTest::destroyGeometry()
{
	if (this->mpSphere != nullptr) delete this->mpSphere;
	if (this->mpCube != nullptr) delete this->mpCube;
	if (this->mpSkyboxCube != nullptr) delete this->mpSkyboxCube;
	if (this->mpFloorCube != nullptr) delete this->mpFloorCube;
	if (this->mpMainLight != nullptr) delete this->mpMainLight;
	if (this->mpSecondLight != nullptr) delete this->mpSecondLight;
	if (this->mpCam != nullptr) delete this->mpCam;
}
