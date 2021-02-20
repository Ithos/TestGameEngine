#include "WindowApplication.h"

// TODO remove
#include <GeometryEngine.h>
#include <GeometryScene.h>
#include <SceneManager.h>
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
		GeometryEngine::CustomShading::CustomShadingInterface trAlphaColorInterface;
		trAlphaColorInterface.AddNewShadingStep< GeometryEngine::CustomShading::AlphaColorShadowMap<GeometryEngine::GeometryMaterial::AlphaColorMaterial> >(GeometryEngine::CustomShading::CUSTOM_SHADOWMAP);
		trAlphaColorInterface.AddNewShadingStep< GeometryEngine::CustomShading::DirectColorMap<GeometryEngine::GeometryMaterial::AlphaColorMaterial> >(GeometryEngine::CustomShading::CUSTOM_COLORMAP);

		GeometryEngine::GeometryMaterial::AlphaColorMaterial trMat(&trAlphaColorInterface, QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 0.0f, 0.0f), 0.1f, 0.7f, 100.0f, true); // QVector3D(1.0f, 0.4f, 0.3f)

		GeometryEngine::CustomShading::CustomShadingInterface alphaColorInterface;
		alphaColorInterface.AddNewShadingStep< GeometryEngine::CustomShading::AlphaColorShadowMap<GeometryEngine::GeometryMaterial::AlphaColorMaterial> >(GeometryEngine::CustomShading::CUSTOM_SHADOWMAP);

		GeometryEngine::GeometryScene::GeometryScene* scene = engine->GetSceneManager()->CreateScene<GeometryEngine::GeometryScene::TransparentGeometryScene>();
		GeometryEngine::GeometryMaterial::AlphaColorMaterial mat(&alphaColorInterface, QVector3D(1.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 0.0f, 0.0f), 0.1f, 0.7f, 100.0f); // QVector3D(1.0f, 0.4f, 0.3f)
		GeometryEngine::GeometryMaterial::ColorMaterial floorMat(QVector3D(0.9f, 0.9f, 0.9f), QVector3D(0.9f, 0.9f, 0.9f), QVector3D(0.9f, 0.9f, 0.9f));

		GeometryEngine::GeometryMaterial::NormalMapMultiTextureMaterial tMat(GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
																				GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
																				GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
																				GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLACK_TEXTURE,
																				GeometryEngine::GeometryMaterial::TextureConstant::NORMALMAP_TEST_BLUE_CHIP, 1000.0f);

		GeometryEngine::CustomShading::CustomShadingInterface alphaNormalMapMultiTextureInterface;
		alphaNormalMapMultiTextureInterface.AddNewShadingStep<GeometryEngine::CustomShading::AlphaMultiTextureShadowMap< GeometryEngine::GeometryMaterial::AlphaNormalMapMultiTextureMaterial > >(GeometryEngine::CustomShading::CUSTOM_SHADOWMAP);
		alphaNormalMapMultiTextureInterface.AddNewShadingStep< GeometryEngine::CustomShading::MultiTextureColorMap<GeometryEngine::GeometryMaterial::AlphaNormalMapMultiTextureMaterial> >(GeometryEngine::CustomShading::CUSTOM_COLORMAP);


		GeometryEngine::GeometryMaterial::AlphaNormalMapMultiTextureMaterial atMat( &alphaNormalMapMultiTextureInterface, GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE,
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLACK_TEXTURE,
			GeometryEngine::GeometryMaterial::TextureConstant::NORMALMAP_TEST_BLUE_CHIP, 0.1f, 0.9f, 1000.0f, true);

		GeometryEngine::CustomShading::CustomShadingInterface alphaTextureInterface;
		alphaTextureInterface.AddNewShadingStep< GeometryEngine::CustomShading::AlphaTextureShadowMap<GeometryEngine::GeometryMaterial::AlphaTextureMaterial> >(GeometryEngine::CustomShading::CUSTOM_SHADOWMAP);
		alphaTextureInterface.AddNewShadingStep< GeometryEngine::CustomShading::TextureColorMap<GeometryEngine::GeometryMaterial::AlphaTextureMaterial> >(GeometryEngine::CustomShading::CUSTOM_COLORMAP);

		GeometryEngine::GeometryMaterial::AlphaTextureMaterial grassMat( &alphaTextureInterface, GeometryEngine::GeometryMaterial::TextureConstant::TEST_GRASS_TEXTURE, 0.1f, 1.0f, 10.0f, true);
		grassMat.SetDrawBacksideFaces(true);


		GeometryEngine::GeometryMaterial::MultiTextureMaterial mtMat(GeometryEngine::GeometryMaterial::TextureConstant::TEST_RIGHT_TEXTURE, GeometryEngine::GeometryMaterial::TextureConstant::TEST_BACK_TEXTURE,
			GeometryEngine::GeometryMaterial::TextureConstant::TEST_BACK_TEXTURE, GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLACK_TEXTURE);
		/*GeometryEngine::Cube**/ testCube = new GeometryEngine::GeometryWorldItem::GeometryItem::Cube(atMat, 4.0f,QVector3D(-5.0f, 0.0f, -15.0f), QVector3D(30.0f, -30.0f, 0.0f));
		/*GeometryEngine::Cube**/ testCube2 = new GeometryEngine::GeometryWorldItem::GeometryItem::Sphere(trMat, 1.0f, 6, 12, QVector3D(-5.0f, 5.0f, -15.0f)); //QVector3D(5.0f, 0.0f, -15.0f)
		GeometryEngine::GeometryItemUtils::PerspectiveViewport viewport(QVector4D(0, 0, this->width(), this->height()), 45.0f, 1.0f, 0.1f, 1000.0f);
		/*GeometryEngine::PerspectiveCamera**/ cam = new GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera(
																			GeometryEngine::GeometryRenderData::RenderBuffersData( GeometryEngine::GeometryBuffer::CompleteColorPostProcessBuffer(), &GeometryEngine::GeometryBuffer::ShadingBuffer() ),
																			viewport, true,
																			QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0, 0, 0));
		//cam->SetPosition(cam->ToModelCoordSystem(QVector3D(-5.0f, 10.0f, -15.0f)));
		//cam->Rotate(QVector3D(90, 0, 0));

		textureQuad = new GeometryEngine::GeometryWorldItem::GeometryItem::Quad(grassMat, 1.0f, 1.0f, QVector3D(0.0f, 0.0f, -15.0f), QVector3D(0.0f, 45.0f, 0.0f));

		GeometryEngine::GeometryWorldItem::GeometryItem::Cube* floor = new GeometryEngine::GeometryWorldItem::GeometryItem::Cube(floorMat, 1.0f, QVector3D(0.0f, -6.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(100.0f, 1.0f, 100.0f));

		GeometryEngine::GeometryWorldItem::GeometryItem::Quad lightQuad(floorMat, 3.0f, 3.0f);
		
		GeometryEngine::CustomShading::CustomPostProcessStepInterface doublePassManager;
		doublePassManager.AddNewPostProcessStep< GeometryEngine::CustomShading::CallSecondStep <GeometryEngine::GeometryPostProcess::DoublePassPostProcess::BlurPostProcess> >
			(GeometryEngine::CustomShading::CustomPostProcessSteps::SECOND_STEP);

		//cam->AddPostProcess(GeometryEngine::GeometryPostProcess::SinglePassPostProcess::GreyScalePostProcess(lightQuad));
		//cam->AddPostProcess( GeometryEngine::GeometryPostProcess::DoublePassPostProcess::BlurPostProcess(lightQuad, &doublePassManager, 2) );

		GeometryEngine::GeometryWorldItem::GeometryItem::Sphere lightSphere(mat, 2.0f);

		GeometryEngine::GeometryItemUtils::PerspectiveViewport lightViewport(QVector4D(0, 0, this->width(), this->height()), 120.0f, 1.0f, 0.1f, 100.0f);
		GeometryEngine::GeometryItemUtils::OrtographicViewport lightViewportOrto(QVector4D(0, 0, this->width(), this->height()), QRect(-this->width() / 24, -this->height() / 24, this->width() / 12, this->height() / 12), 0.1f, 1000.0f);

		//mainLight = new GeometryEngine::GeometryWorldItem::GeometryLight::Spotlight(45.0f, QVector3D(0.5f, 0.3f, 0.1f), QVector3D(0.0, -1.0, 0.0), &lightSphere, QVector3D(1.0f, 1.0f, 1.0f),
		//	QVector3D(1.0f, 1.0f, 1.0f), QVector3D(1.0f, 1.0f, 1.0f), QVector3D(5.0f, 4.0f, -15.0f));

		//secondLight = new GeometryEngine::GeometryWorldItem::GeometryLight::Spotlight(45.0f, QVector3D(0.5f, 0.3f, 0.1f), QVector3D(0.0, -1.0, 0.0), &lightSphere, QVector3D(1.0f, 1.0f, 1.0f),
		//	QVector3D(1.0f, 1.0f, 1.0f), QVector3D(1.0f, 1.0f, 1.0f), QVector3D(-5.0f, 4.0f, -15.0f));

		//mainLight = new GeometryEngine::GeometryWorldItem::GeometryLight::ShadowSpotlight(45.0f, QVector3D(0.1f, 0.1f, 0.01f), lightViewport, QVector3D(0.0, -1.0, 0.0), &lightSphere, QVector3D(1.0f, 1.0f, 1.0f),
		//	QVector3D(1.0f, 1.0f, 1.0f), QVector3D(1.0f, 1.0f, 1.0f), QVector3D(5.0f, 10.0f, -15.0f));
		

		secondLight = new GeometryEngine::GeometryWorldItem::GeometryLight::ShadowSpotlight(45.0f, QVector3D(0.1f, 0.1f, 0.01f), lightViewport, QVector3D(0.0f, -1.0f, 0.0f), &lightSphere, QVector3D(1.0f, 1.0f, 1.0f),
				QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 1.0f, 1.0f), QVector3D(-5.0f, 10.0f, -15.0f), QVector3D(0.0f, 0.0f, 0.0f), 0.0003f, QVector3D(1.0f, 1.0f, 1.0f) );

		mainLight = new GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalShadowLight(lightViewportOrto, QVector3D(0.0, -1.0, 0.0), &lightQuad, QVector3D(0.1f, 0.1f, 0.1f),
			QVector3D(0.1f, 0.1f, 0.1f), QVector3D(0.1f, 0.1f, 0.1f), QVector3D(5.0f, 30.0f, -15.0f), QVector3D(0.0f, 0.0f, 0.0f), 0.001f, QVector3D(1.0f, 1.0f, 1.0f) );

		//GeometryEngine::GeometryWorldItem::GeometryItem::Cube* lightCube = new GeometryEngine::GeometryWorldItem::GeometryItem::Cube(mat, 0.2f, QVector3D(5.0f, 5.0f, -15.0f), QVector3D(0.1f, 0.1f, 0.1f), QVector3D(1.0f, 1.0f, 1.0f));
		//GeometryEngine::GeometryWorldItem::GeometryItem::Cube* 
			lightCube2 = new GeometryEngine::GeometryWorldItem::GeometryItem::Cube(floorMat, 0.2f, QVector3D(-5.0f, 10.0f, -15.0f), QVector3D(0.1f, 0.1f, 0.1f), QVector3D(1.0f, 1.0f, 1.0f));
			lightCube2->SetCastsShadows(false);

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
		skyboxCube = new GeometryEngine::GeometryWorldItem::GeometryItem::Cube(skyboxMat, 200.0f, QVector3D(.0f,.0f,.0f), QVector3D(.0f,.0f,.0f), QVector3D(1.0f, 1.0f, 1.0f), nullptr, &texCoordArray);
		skyboxCube->SetCastsShadows(false);

		//GeometryEngine::OrthographicCamera* cam2 = new GeometryEngine::OrthographicCamera(QVector4D(0, this->height() / 2, this->width()/2, this->height() / 2), QRect(-10, 10, 20, 20));
		scene->AddItem(skyboxCube);
		scene->AddItem(testCube);
		scene->AddItem(testCube2);
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
