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

#include <Items\GraphicItems\Lights\DirectionalLight.h>
#include <Items\GraphicItems\Lights\Spotlight.h>
#include <Items\GraphicItems\Cameras\DeferredShadingCamera.h>

#include <Items\Item Utils\Viewports\PerspectiveViewport.h>

#include <Items\PostProcess\DoublePassPostProcess\BlurPostProcess.h>
#include <Items\PostProcess\SinglePassPostProcess\GreyScalePostProcess.h>

#include <Render Utils\RenderBuffersData.h>
#include <Render Utils\Gbuffers\CompleteColorPostProcessBuffer.h>
#include <Render Utils\ShadingBuffer.h>

#include <Scenes/PostProcessScene.h>

#include "PostProcessTest.h"

const float UnitTest::CPostProcessTest::SECS = 50.0f;
const QVector3D UnitTest::CPostProcessTest::ROTATION = QVector3D(0.0f, 0.3f, 0.0f);
const QVector3D UnitTest::CPostProcessTest::MOVEMENT = QVector3D(0.1f, 0.0f, 0.1f);
const QVector3D UnitTest::CPostProcessTest::POINT = QVector3D(-5.0f, 0.0f, -15.0f);
const QVector3D UnitTest::CPostProcessTest::POS_LIGHT = QVector3D(-5.0f, 10.0f, -15.0f);

UnitTest::CPostProcessTest::~CPostProcessTest()
{
	destroyGeometry();
}

void UnitTest::CPostProcessTest::InitGeometry(GeometryEngine::GeometryEngine * engine, int sceneWidth, int sceneHeight)
{
	mSceneWidth = sceneWidth;
	mSceneHeigth = sceneHeight;

	//Create the scene
	GeometryEngine::GeometryScene::GeometryScene* scene = engine->GetSceneManager()->CreateScene<GeometryEngine::GeometryScene::PostProcessScene>();

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

	GeometryEngine::GeometryWorldItem::GeometryItem::Sphere lightSphere(floorMat, 2.0f);
	GeometryEngine::GeometryWorldItem::GeometryItem::Quad lightQuad(floorMat, 3.0f, 3.0f);

	mpSecondLight = new GeometryEngine::GeometryWorldItem::GeometryLight::Spotlight(45.0f, QVector3D(0.1f, 0.1f, 0.01f), QVector3D(0.0f, -1.0f, 0.0f), &lightSphere, QVector3D(1.0f, 1.0f, 1.0f),
		QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f), POS_LIGHT, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f));

	mpMainLight = new GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalLight(QVector3D(0.0, -1.0, 0.0), &lightQuad, QVector3D(0.1f, 0.1f, 0.1f),
		QVector3D(0.1f, 0.1f, 0.1f), QVector3D(0.1f, 0.1f, 0.1f), QVector3D(5.0f, 30.0f, -15.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f));

	//camera

	GeometryEngine::GeometryItemUtils::PerspectiveViewport viewport(QVector4D(0, 0, mSceneWidth, mSceneHeigth), 45.0f, 1.0f, 0.1f, 1000.0f);

	mpCam = new GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera(
		GeometryEngine::GeometryRenderData::RenderBuffersData(GeometryEngine::GeometryBuffer::CompleteColorPostProcessBuffer(), &GeometryEngine::GeometryBuffer::ShadingBuffer()),
		viewport, true,
		QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0, 0, 0));

	// Add post processes to the camera
	mpCam->AddPostProcess( GeometryEngine::GeometryPostProcess::SinglePassPostProcess::GreyScalePostProcess(lightQuad) );
	mpCam->AddPostProcess( GeometryEngine::GeometryPostProcess::DoublePassPostProcess::BlurPostProcess(lightQuad) );

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

void UnitTest::CPostProcessTest::UpdateGeometry(qint64 tick)
{
	if (this->mpCube != nullptr) this->mpCube->SetRotation(SECS * ROTATION);

	QVector3D mov = MOVEMENT;
	if (mpSecondLight->GetPosition().z() < -65 || mpSecondLight->GetPosition().x() < -40) mov = MOVEMENT;
	if (mpSecondLight->GetPosition().z() > 35 || mpSecondLight->GetPosition().x() > 30) mov = -MOVEMENT;

	mpSecondLight->SetPosition(POS_LIGHT + mpSecondLight->ToModelCoordSystem(mov * SECS));

	QVector3D nDir(POINT - mpSecondLight->GetPosition());
	nDir.normalize();
	((GeometryEngine::GeometryWorldItem::GeometryLight::Spotlight*)(mpSecondLight))->SetDirection(nDir);

	evaluate();
}

void UnitTest::CPostProcessTest::copy(const CPostProcessTest & ref)
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

void UnitTest::CPostProcessTest::evaluate()
{
	if (!mScenePainted) return;

	unsigned char pixel[4];

	mTestFinished = true;
	mTestSuccessful = true;

	// Mid line check
	glReadPixels((GLint)(mSceneWidth / 2), (GLint)(mSceneHeigth / 2), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 128 && pixel[1] == 128 && pixel[2] == 128 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth / 3), (GLint)(mSceneHeigth / 2), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 135 && pixel[1] == 135 && pixel[2] == 135 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth * 2 / 3), (GLint)(mSceneHeigth / 2), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 144 && pixel[1] == 144 && pixel[2] == 144 && pixel[3] == 204;

	// Down line checks

	glReadPixels((GLint)(mSceneWidth / 2), (GLint)(mSceneHeigth / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 73 && pixel[1] == 73 && pixel[2] == 73 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth / 3), (GLint)(mSceneHeigth / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 69 && pixel[1] == 69 && pixel[2] == 69 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth * 2 / 3), (GLint)(mSceneHeigth / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 62 && pixel[1] == 62 && pixel[2] == 62 && pixel[3] == 204;

	// Top line checks

	glReadPixels((GLint)(mSceneWidth / 2), (GLint)(mSceneHeigth * 2 / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 150 && pixel[1] == 150 && pixel[2] == 150 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth / 3), (GLint)(mSceneHeigth * 2 / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 116 && pixel[1] == 116 && pixel[2] == 116 && pixel[3] == 204;

	glReadPixels((GLint)(mSceneWidth * 2 / 3), (GLint)(mSceneHeigth * 2 / 3), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	mTestSuccessful &= pixel[0] == 176 && pixel[1] == 176 && pixel[2] == 176 && pixel[3] == 204;

}

void UnitTest::CPostProcessTest::destroyGeometry()
{
	if (this->mpSphere != nullptr) delete this->mpSphere;
	if (this->mpCube != nullptr) delete this->mpCube;
	if (this->mpSkyboxCube != nullptr) delete this->mpSkyboxCube;
	if (this->mpFloorCube != nullptr) delete this->mpFloorCube;
	if (this->mpMainLight != nullptr) delete this->mpMainLight;
	if (this->mpSecondLight != nullptr) delete this->mpSecondLight;
	if (this->mpCam != nullptr) delete this->mpCam;
}
