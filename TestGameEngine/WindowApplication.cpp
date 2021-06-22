#include "WindowApplication.h"

// TODO remove
#include <GeometryEngine.h>
#include <GeometryScene.h>
#include <SceneManager.h>

#include <GeometryFactory.h>

#include <Items/Geometries/Cube.h>
#include <Items/Geometries/Sphere.h>
#include <Items/Geometries/Quad.h>
#include <Items/GeometryItem.h>
#include <Items/WorldItem.h>
#include <Items/GraphicItems/Camera.h>
#include <Items/GraphicItems/Cameras/DeferredShadingCamera.h>
#include <Items/Item Utils/Viewports/OrtographicViewport.h>
#include <Items/Item Utils/Viewports/PerspectiveViewport.h>
#include <Items/Materials/ColorMaterial.h>
#include <Items/Materials/TransparentMaterials/AlphaColorMaterial.h>
#include <Items/Materials/TextureMaterial.h>
#include <Items/Materials/MultiTextureMaterial.h>
#include <Items/Materials/NormalMapTextureMaterial.h>
#include <Items/Materials/NormalMapMultiTextureMaterial.h>
#include <Items/Materials/SkyboxMaterial.h>
#include <Items\Materials\TextureMaterialCommons.h>
#include <Items/Materials/TransparentMaterials/AlphaTextureMaterial.h>
#include <Items/Materials/TransparentMaterials/AlphaNormalMapTextureMaterial.h>
#include <Items/Materials/TransparentMaterials/AlphaMultiTextureMaterial.h>
#include <Items/Materials/TransparentMaterials/AlphaNormalMapMultiTextureMaterial.h>
#include <Textures.h>
#include <Items\GraphicItems\Lights\AmbientLight.h>
#include <Items\GraphicItems\Lights\DirectionalLight.h>
#include <Items\GraphicItems\Lights\PointLight.h>
#include <Items\GraphicItems\Lights\Spotlight.h>
#include <Items\GraphicItems\Lights\ShadowCastingLights\DirectionalShadowLight.h>
#include <Items\GraphicItems\Lights\ShadowCastingLights\ShadowSpotlight.h>
#include <Items\CommonInerfaces\CustomShadingInterface.h>
#include <Items\CommonInerfaces\CustomShadingStep.h>
#include <Items\CommonInerfaces\ShadingSteps\AlphaColorShadowMap.h>
#include <Items\CommonInerfaces\ShadingSteps\AlphaTextureShadowMap.h>
#include <Items\CommonInerfaces\ShadingSteps\AlphaMultiTextureShadowMap.h>
#include <Items\CommonInerfaces\ShadingSteps\DirectColorMap.h>
#include <Items\CommonInerfaces\ShadingSteps\TextureColorMap.h>
#include <Items\CommonInerfaces\ShadingSteps\MultiTextureColorMap.h>

#include <Scenes/DeferredShadingScene.h>
#include <Scenes\PostProcessScene.h>
#include <Scenes\DynamicShadowsScene.h>
#include <Scenes\TransparentGeometryScene.h>

#include <Render Utils\Gbuffers\CompleteColorBuffer.h>
#include <Render Utils\Gbuffers\SingleColorTextureBuffer.h>
#include <Render Utils\Gbuffers\CompleteColorPostProcessBuffer.h>
#include <Render Utils\RenderBuffersData.h>
#include <Render Utils\ShadingBuffer.h>
#include <Items\PostProcess\DoublePassPostProcess\BlurPostProcess.h>
#include <Items\PostProcess\SinglePassPostProcess\GreyScalePostProcess.h>
#include <Items\PostProcess\PostProcess.h>

#include <Items\CommonInerfaces\CustomPostProcessStepInterface.h>
#include <Items\CommonInerfaces\CustomPostProcessStep.h>
#include <Items\CommonInerfaces\PostProcessSteps\CallSecondStep.h>

namespace Application
{
	CWindowApplication::CWindowApplication(QWidget *parent) :
		QOpenGLWidget(parent),
		mpGeomInstance(nullptr)
	{
		mWindowSize = QVector2D(this->width(), this->height());
		mpMovementArray = new bool[6]{ false, false, false, false, false, false };
	}

	CWindowApplication::~CWindowApplication()
	{
		// Make sure the context is current when deleting the texture
		// and the buffers.
		makeCurrent();
		GeometryEngine::GeometryEngine::Release();
		doneCurrent();
	}

	void CWindowApplication::mousePressEvent(QMouseEvent * e)
	{
		/// TODO -- get the mouse pressed position and send it to an input manager -- ///
	}

	void CWindowApplication::mouseReleaseEvent(QMouseEvent * e)
	{
		/// TODO -- get the mouse released position and send it to an input manager -- ///
	}

	void CWindowApplication::keyPressEvent(QKeyEvent * e)
	{
		/// TODO -- get key pressed and sent it to an input manager -- ///
		if (e->key() == Qt::Key_W)
		{
			mpMovementArray[0] = true;
		}
		if (e->key() == Qt::Key_S)
		{
			mpMovementArray[1] = true;
		}
		if (e->key() == Qt::Key_A)
		{
			mpMovementArray[2] = true;
		}
		if (e->key() == Qt::Key_D)
		{
			mpMovementArray[3] = true;
		}
		if (e->key() == Qt::Key_Q)
		{
			mpMovementArray[4] = true;
		}
		if (e->key() == Qt::Key_E)
		{
			mpMovementArray[5] = true;
		}
	}

	void CWindowApplication::keyReleaseEvent(QKeyEvent * e)
	{
		/// TODO -- get key released and sent it to an input manager -- ///
		if (e->key() == Qt::Key_W)
		{
			mpMovementArray[0] = false;
		}
		if (e->key() == Qt::Key_S)
		{
			mpMovementArray[1] = false;
		}
		if (e->key() == Qt::Key_A)
		{
			mpMovementArray[2] = false;
		}
		if (e->key() == Qt::Key_D)
		{
			mpMovementArray[3] = false;
		}
		if (e->key() == Qt::Key_Q)
		{
			mpMovementArray[4] = false;
		}
		if (e->key() == Qt::Key_E)
		{
			mpMovementArray[5] = false;
		}
	}

	void CWindowApplication::timerEvent(QTimerEvent * e)
	{
		/// TODO -- send tick  -- ///

		update();
	}

	void CWindowApplication::initializeGL()
	{
		mpGeomInstance = GeometryEngine::GeometryEngine::GetInstance();
		initGeometry(mpGeomInstance);
		// Use QBasicTimer because its faster than QTimer
		timer.start(12, this);
	}

	void CWindowApplication::resizeGL(int w, int h)
	{
		GeometryEngine::GeometryScene::GeometryScene * scene = mpGeomInstance->GetSceneManager()->GetActiveScene();

		if (scene != nullptr)
		{
			scene->ResizeScene(w, h, mWindowSize.x(), mWindowSize.y());
			mWindowSize.setX(w); mWindowSize.setY(h);
		}
	}

	void CWindowApplication::paintGL()
	{
		// Draw cube geometry
		GeometryEngine::GeometryScene::GeometryScene * scene = mpGeomInstance->GetSceneManager()->GetActiveScene();

		QVector3D rotation = QVector3D(0.0f, 0.3f, 0.0f);

		testCube->Rotate(testCube->ToModelCoordSystem(rotation));
		testCube2->Rotate(testCube2->ToModelCoordSystem(rotation));
		topSphere->Rotate(topSphere->ToModelCoordSystem(rotation));

		if (mpMovementArray[0])
		{
			cam->Move(cam->ToModelCoordSystem(QVector3D(0.0, 0.0, -0.2)));
		}
		if (mpMovementArray[1])
		{
			cam->Move(cam->ToModelCoordSystem(QVector3D(0.0, 0.0, 0.2)));
		}
		if (mpMovementArray[2])
		{
			cam->Move(cam->ToModelCoordSystem(QVector3D(-0.2, 0.0, 0.0)));
		}
		if (mpMovementArray[3])
		{
			cam->Move(cam->ToModelCoordSystem(QVector3D(0.2, 0.0, 0.0)));
		}
		if (mpMovementArray[4])
		{
			cam->Rotate(cam->ToModelCoordSystem(QVector3D(0.0, -0.6, 0.0)));
		}
		if (mpMovementArray[5])
		{
			cam->Rotate(cam->ToModelCoordSystem(QVector3D(0.0, 0.6, 0.0)));
		}

		//cam->Rotate(rotation);
		//mainLight->Rotate(mainLight->ToModelCoordSystem(QVector3D(0.3f, 0.0f, 0.0f)));

		static QVector3D mov(0.1f, 0.0f, 0.1f);
		if (secondLight->GetPosition().z() < -65 || secondLight->GetPosition().x() < -40) mov = QVector3D(0.1f, 0.0f, 0.1f);
		if (secondLight->GetPosition().z() > 35 || secondLight->GetPosition().x() > 30) mov = QVector3D(-0.1f, 0.0f, -0.1f);

		secondLight->Move(secondLight->ToModelCoordSystem(mov) );
		lightCube2->Move(lightCube2->ToModelCoordSystem(mov));

		QVector3D nDir(QVector3D(-5.0f, 0.0f, -15.0f) - secondLight->GetPosition());
		nDir.normalize();
		((GeometryEngine::GeometryWorldItem::GeometryLight::ShadowMapLight*)(secondLight))->SetDirection(nDir);

		if (scene != nullptr)
		{
			scene->Draw();
		}

		
	}
	void CWindowApplication::initGeometry(GeometryEngine::GeometryEngine* engine)
	{
		GeometryEngine::GeometryScene::GeometryScene* scene = engine->GetSceneManager()->CreateScene<GeometryEngine::GeometryScene::TransparentGeometryScene>();

		testCube = GeometryEngine::GeometryFactory::CreateCube(
			GeometryEngine::GeometryFactory::CreateAlphaTextureMaterial(
				GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
				GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
				GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
				GeometryEngine::GeometryMaterial::TextureConstant::NORMALMAP_TEST_BLUE_CHIP,
				GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLACK_TEXTURE, 0.8f, true, 1000.0f),
			GeometryEngine::TEXTURE_MODE::REPEAT, 4.0f, QVector3D(-5.0f, 0.0f, -15.0f), QVector3D(30.0f, -30.0f, 0.0f)
		);

		testCube2 = GeometryEngine::GeometryFactory::CreateSphere(
				GeometryEngine::GeometryFactory::CreateAlphaColorMaterial(QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f),
					QVector3D(0.0f, 0.0f, 0.0f), 0.7f, true, 100.0f),
				1.0, QVector3D(-5.0f, 5.0f, -15.0f)
			);

		topSphere = GeometryEngine::GeometryFactory::CreateSphere(
			GeometryEngine::GeometryFactory::CreateAlphaColorMaterial(QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f),
				QVector3D(0.0f, 0.0f, 0.0f), 0.7f, true, 100.0f),
			1.0, QVector3D(5.0f, 0.0f, -15.0f)
		);

		cam = GeometryEngine::GeometryFactory::CreateDeferredShadingCamera(GeometryEngine::GeometryFactory::CreatePerspectiveViewport(
			QVector4D(0, 0, this->width(), this->height()), 45.0f, 1.0f, 0.1f, 1000.0f),
			QVector3D(0.0f, 0.0f, 0.0f)
		);

		GeometryEngine::GeometryMaterial::Material* tmp =
			GeometryEngine::GeometryFactory::CreateAlphaTextureMaterial(GeometryEngine::GeometryMaterial::TextureConstant::TEST_GRASS_TEXTURE, 1.0f);
		tmp->SetDrawBacksideFaces(true);
		textureQuad = GeometryEngine::GeometryFactory::CreateQuad( tmp, 1.0f, 1.0f, QVector3D(0.0f, 0.0f, -15.0f), QVector3D(0.0f, 45.0f, 0.0f));

		GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem* floor = GeometryEngine::GeometryFactory::CreateCube(
			GeometryEngine::GeometryFactory::CreateColorMaterial(QVector3D(0.9f, 0.9f, 0.9f), QVector3D(0.9f, 0.9f, 0.9f), QVector3D(0.9f, 0.9f, 0.9f)),
			GeometryEngine::TEXTURE_MODE::REPEAT, 1.0f, QVector3D(0.0f, -6.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(100.0f, 1.0f, 100.0f)
		);
		
		GeometryEngine::CustomShading::CustomPostProcessStepInterface doublePassManager;
		doublePassManager.AddNewPostProcessStep< GeometryEngine::CustomShading::CallSecondStep <GeometryEngine::GeometryPostProcess::DoublePassPostProcess::BlurPostProcess> >
			(GeometryEngine::CustomShading::CustomPostProcessSteps::SECOND_STEP);

		secondLight = GeometryEngine::GeometryFactory::CreateShadowSpotlight(QVector3D(-5.0f, 10.0f, -15.0f), QVector3D(0.0f, -1.0f, 0.0f),
			GeometryEngine::GeometryFactory::CreatePerspectiveViewport(QVector4D(0, 0, this->width(), this->height()), 120.0f, 1.0f, 0.1f, 100.0f),
			QVector3D(1.0f, 1.0f, 1.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f), 45.0f, QVector3D(0.1f, 0.1f, 0.01f), QVector3D(0.0f, 0.0f, 0.0f), 0.0003f);


		mainLight = GeometryEngine::GeometryFactory::CreateDirectionalShadowLight(QVector3D(5.0f, 30.0f, -15.0f), QVector3D(0.0, -1.0, 0.0),
			GeometryEngine::GeometryFactory::CreateOrtographicViewport(QVector4D(0, 0, this->width(), this->height()), QRect(-this->width() / 24, -this->height() / 24,
				this->width() / 12, this->height() / 12), 0.1f, 1000.0f),
			QVector3D(0.1f, 0.1f, 0.1f),
			QVector3D(0.1f, 0.1f, 0.1f), QVector3D(0.1f, 0.1f, 0.1f));

			lightCube2 = GeometryEngine::GeometryFactory::CreateCube(
				GeometryEngine::GeometryFactory::CreateColorMaterial(QVector3D(0.9f, 0.9f, 0.9f), QVector3D(0.9f, 0.9f, 0.9f), QVector3D(0.9f, 0.9f, 0.9f)), 
				GeometryEngine::TEXTURE_MODE::REPEAT, 0.2f, QVector3D(-5.0f, 10.0f, -15.0f), QVector3D(0.1f, 0.1f, 0.1f) );
			
			lightCube2->SetCastsShadows(false);

		skyboxCube = GeometryEngine::GeometryFactory::CreateSkyboxCube(GeometryEngine::GeometryMaterial::TextureConstant::TEST_SKYBOX_TEXTURE, 200.0f);

		//GeometryEngine::OrthographicCamera* cam2 = new GeometryEngine::OrthographicCamera(QVector4D(0, this->height() / 2, this->width()/2, this->height() / 2), QRect(-10, 10, 20, 20));
		scene->AddItem(skyboxCube);
		scene->AddItem(testCube);
		scene->AddItem(testCube2);
		scene->AddItem(topSphere);
		scene->AddItem(textureQuad);
		//scene->AddItem(lightCube);
		scene->AddItem(lightCube2);
		scene->AddItem(floor);
		scene->AddCamera(cam);
		scene->AddLight(mainLight);
		scene->AddLight(secondLight);
		//scene->AddCamera(cam2);
		scene->InitializeGL();
		engine->GetSceneManager()->SetActiveScene(scene);
	}
}
