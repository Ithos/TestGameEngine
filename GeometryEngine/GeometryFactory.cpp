#include <vector>
#include <qvector2d.h>
#include "Items\Materials\ColorMaterial.h"
#include "Items\Materials\SkyboxMaterial.h"
#include "Items\Materials\TextureMaterial.h"
#include "Items\Materials\MultiTextureMaterial.h"
#include "Items\Materials\NormalMapTextureMaterial.h"
#include "Items\Materials\NormalMapMultiTextureMaterial.h"
#include "Items\Materials\TransparentMaterials\AlphaColorMaterial.h"
#include "Items\Materials\TransparentMaterials\AlphaTextureMaterial.h"
#include "Items\Materials\TransparentMaterials\AlphaMultiTextureMaterial.h"
#include "Items\Materials\TransparentMaterials\AlphaNormalMapTextureMaterial.h"
#include "Items\Materials\TransparentMaterials\AlphaNormalMapMultiTextureMaterial.h"
#include "Items\CommonInerfaces\CustomShadingInterface.h"
#include "Items\CommonInerfaces\ShadingSteps\AlphaColorShadowMap.h"
#include "Items\CommonInerfaces\ShadingSteps\DirectColorMap.h"
#include "Items\CommonInerfaces\ShadingSteps\AlphaColorShadowMap.h"
#include "Items\CommonInerfaces\ShadingSteps\AlphaTextureShadowMap.h"
#include "Items\CommonInerfaces\ShadingSteps\AlphaMultiTextureShadowMap.h"
#include "Items\CommonInerfaces\ShadingSteps\MultiTextureColorMap.h"
#include "Items\CommonInerfaces\ShadingSteps\TextureColorMap.h"
#include "Items\Materials\TextureMaterialCommons.h"
#include "Items\Geometries\Cube.h"
#include "Items\Geometries\Sphere.h"
#include "Items\Geometries\Quad.h"
#include "Items\GraphicItems\Lights\PointLight.h"
#include "Items\GraphicItems\Lights\AmbientLight.h"
#include "Items\GraphicItems\Lights\DirectionalLight.h"
#include "Items\GraphicItems\Lights\Spotlight.h"
#include "Items\GraphicItems\Lights\ShadowCastingLights\ShadowSpotlight.h"
#include "Items\GraphicItems\Lights\ShadowCastingLights\DirectionalShadowLight.h"
#include "Items\Item Utils\Viewports\OrtographicViewport.h"
#include "Items\Item Utils\Viewports\PerspectiveViewport.h"
#include "Items\GraphicItems\Cameras\DeferredShadingCamera.h"
#include "Render Utils\RenderBuffersData.h"
#include "Render Utils\Gbuffers\CompleteColorPostProcessBuffer.h"
#include "Render Utils\ShadingBuffer.h"
#include <Items\PostProcess\DoublePassPostProcess\BlurPostProcess.h>
#include <Items\PostProcess\SinglePassPostProcess\GreyScalePostProcess.h>
#include <Items\CommonInerfaces\CustomPostProcessStepInterface.h>
#include <Items\CommonInerfaces\CustomPostProcessStep.h>
#include <Items\CommonInerfaces\PostProcessSteps\CallSecondStep.h>
#include "GeometryFactory.h"

GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem * GeometryEngine::GeometryFactory::CreateSkyboxCube(const std::string & textureKey, float skyboxSize)
{
	GeometryEngine::GeometryMaterial::SkyboxMaterial skyboxMat(textureKey);
	GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem* mpSkyboxCube = CreateCube(skyboxMat, TEXTURE_MODE::MAP, skyboxSize, 
		QVector3D(.0f, .0f, .0f), QVector3D(.0f, .0f, .0f), QVector3D(1.0f, 1.0f, 1.0f), nullptr);
	mpSkyboxCube->SetCastsShadows(false);
	return mpSkyboxCube;
}

GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem * GeometryEngine::GeometryFactory::CreateSphere(const GeometryMaterial::Material & mat, float size, const QVector3D & pos, GeometryWorldItem::WorldItem * parent)
{
	return new GeometryEngine::GeometryWorldItem::GeometryItem::Sphere(mat, size, 6, 12, pos, QVector3D(.0f,.0f,.0f), QVector3D(1.0f, 1.0f, 1.0f), parent );
}

GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem * GeometryEngine::GeometryFactory::CreateSphere(GeometryMaterial::Material * mat, float size, const QVector3D & pos, GeometryWorldItem::WorldItem * parent)
{
	GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem* tmp = CreateSphere(*mat, size, pos, parent);
	delete mat;
	return tmp;
}

GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem * GeometryEngine::GeometryFactory::CreateCube(const GeometryMaterial::Material & mat, TEXTURE_MODE mode, float size, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, GeometryWorldItem::WorldItem * parent)
{
	std::vector<QVector2D> texCoordArray;

	if (mode == TEXTURE_MODE::MAP)
	{
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
	}

	return new GeometryEngine::GeometryWorldItem::GeometryItem::Cube(mat, size, pos, rot, scale, parent, texCoordArray.size() > 0 ? &texCoordArray: nullptr);
}

GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem * GeometryEngine::GeometryFactory::CreateCube(GeometryMaterial::Material * mat, TEXTURE_MODE mode, float size, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, GeometryWorldItem::WorldItem * parent)
{
	GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem* tmp = CreateCube(*mat, mode, size, pos, rot, scale, parent);
	delete mat;
	return tmp;
}

GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem * GeometryEngine::GeometryFactory::CreateQuad(const GeometryMaterial::Material & mat, float width, float height, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, GeometryWorldItem::WorldItem * parent)
{
	return new GeometryEngine::GeometryWorldItem::GeometryItem::Quad(mat, width, height, pos, rot, scale, parent);
}

GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem * GeometryEngine::GeometryFactory::CreateQuad(GeometryMaterial::Material * mat, float width, float height, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, GeometryWorldItem::WorldItem * parent)
{
	GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem* tmp = CreateQuad(*mat, width, height, pos, rot, scale, parent);
	delete mat;
	return tmp;
}

GeometryEngine::GeometryMaterial::Material * GeometryEngine::GeometryFactory::CreateColorMaterial(const QVector3D & ambient, const QVector3D & diffuse, const QVector3D & specular, const QVector3D & emissive, float shininess)
{
	return new GeometryEngine::GeometryMaterial::ColorMaterial(ambient, diffuse, specular, emissive, shininess);
}

GeometryEngine::GeometryMaterial::Material * GeometryEngine::GeometryFactory::CreateAlphaColorMaterial(const QVector3D & ambient, const QVector3D & diffuse, const QVector3D & specular, const QVector3D & emissive, float alpha, bool translucent, float shininess)
{
	GeometryEngine::CustomShading::CustomShadingInterface trAlphaColorInterface;
	trAlphaColorInterface.AddNewShadingStep< GeometryEngine::CustomShading::AlphaColorShadowMap<GeometryEngine::GeometryMaterial::AlphaColorMaterial> >(GeometryEngine::CustomShading::CUSTOM_SHADOWMAP);
	if(translucent)trAlphaColorInterface.AddNewShadingStep< GeometryEngine::CustomShading::DirectColorMap<GeometryEngine::GeometryMaterial::AlphaColorMaterial> >(GeometryEngine::CustomShading::CUSTOM_COLORMAP);
	return new GeometryEngine::GeometryMaterial::AlphaColorMaterial(&trAlphaColorInterface, ambient, diffuse, specular, emissive, 0.1f, alpha, shininess, translucent);
}

GeometryEngine::GeometryMaterial::Material * GeometryEngine::GeometryFactory::CreateTextureMaterial(const std::string & texDir, float shininess)
{
	return new GeometryEngine::GeometryMaterial::TextureMaterial(texDir, shininess);
}

GeometryEngine::GeometryMaterial::Material * GeometryEngine::GeometryFactory::CreateAlphaTextureMaterial(const std::string & texDir, float alpha, bool translucent, float shininess)
{
	GeometryEngine::CustomShading::CustomShadingInterface trAlphaColorInterface;
	trAlphaColorInterface.AddNewShadingStep< GeometryEngine::CustomShading::AlphaTextureShadowMap<GeometryEngine::GeometryMaterial::AlphaTextureMaterial> >(GeometryEngine::CustomShading::CUSTOM_SHADOWMAP);
	if (translucent)trAlphaColorInterface.AddNewShadingStep< GeometryEngine::CustomShading::TextureColorMap<GeometryEngine::GeometryMaterial::AlphaTextureMaterial> >(GeometryEngine::CustomShading::CUSTOM_COLORMAP);
	return new GeometryEngine::GeometryMaterial::AlphaTextureMaterial(&trAlphaColorInterface, texDir, 0.1f, alpha, shininess, translucent);
}

GeometryEngine::GeometryMaterial::Material * GeometryEngine::GeometryFactory::CreateTextureMaterial(const std::string & texDir, const std::string & normalMapTexDir, float shininess)
{
	return new GeometryEngine::GeometryMaterial::NormalMapTextureMaterial(texDir, normalMapTexDir, shininess);
}

GeometryEngine::GeometryMaterial::Material * GeometryEngine::GeometryFactory::CreateAlphaTextureMaterial(const std::string & texDir, const std::string & normalMapTexDir, float alpha, bool translucent, float shininess)
{
	GeometryEngine::CustomShading::CustomShadingInterface trAlphaColorInterface;
	trAlphaColorInterface.AddNewShadingStep< GeometryEngine::CustomShading::AlphaTextureShadowMap<GeometryEngine::GeometryMaterial::AlphaNormalMapTextureMaterial> >(GeometryEngine::CustomShading::CUSTOM_SHADOWMAP);
	if (translucent)trAlphaColorInterface.AddNewShadingStep< GeometryEngine::CustomShading::TextureColorMap<GeometryEngine::GeometryMaterial::AlphaNormalMapTextureMaterial> >(GeometryEngine::CustomShading::CUSTOM_COLORMAP);
	return new GeometryEngine::GeometryMaterial::AlphaNormalMapTextureMaterial(&trAlphaColorInterface,texDir, normalMapTexDir, 0.1, alpha, shininess, translucent);
}

GeometryEngine::GeometryMaterial::Material * GeometryEngine::GeometryFactory::CreateTextureMaterial(const std::string & ambientTexDir, const std::string & diffuseTexDir, const std::string & specularTexDir, const std::string & emissiveTexDir, float shininess)
{
	return new GeometryEngine::GeometryMaterial::MultiTextureMaterial(ambientTexDir, diffuseTexDir, specularTexDir, emissiveTexDir, shininess);
}

GeometryEngine::GeometryMaterial::Material * GeometryEngine::GeometryFactory::CreateAlphaTextureMaterial(const std::string & ambientTexDir, const std::string & diffuseTexDir, const std::string & specularTexDir, const std::string & emissiveTexDir, float alpha, bool translucent, float shininess)
{
	GeometryEngine::CustomShading::CustomShadingInterface trAlphaColorInterface;
	trAlphaColorInterface.AddNewShadingStep< GeometryEngine::CustomShading::AlphaMultiTextureShadowMap<GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial> >(GeometryEngine::CustomShading::CUSTOM_SHADOWMAP);
	if (translucent)trAlphaColorInterface.AddNewShadingStep< GeometryEngine::CustomShading::MultiTextureColorMap<GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial> >(GeometryEngine::CustomShading::CUSTOM_COLORMAP);
	return new GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial(&trAlphaColorInterface, ambientTexDir, diffuseTexDir, specularTexDir, emissiveTexDir, 0.1, alpha, shininess, translucent);
}

GeometryEngine::GeometryMaterial::Material * GeometryEngine::GeometryFactory::CreateTextureMaterial(const std::string & ambientTexDir, const std::string & diffuseTexDir, const std::string & specularTexDir, const std::string & normalMapTexDir, const std::string & emissiveTexDir, float shininess)
{
	return new GeometryEngine::GeometryMaterial::NormalMapMultiTextureMaterial(ambientTexDir, diffuseTexDir, specularTexDir, emissiveTexDir, normalMapTexDir, shininess);
}

GeometryEngine::GeometryMaterial::Material * GeometryEngine::GeometryFactory::CreateAlphaTextureMaterial(const std::string & ambientTexDir, const std::string & diffuseTexDir, const std::string & specularTexDir, const std::string & normalMapTexDir, const std::string & emissiveTexDir, float alpha, bool translucent, float shininess)
{
	GeometryEngine::CustomShading::CustomShadingInterface trAlphaColorInterface;
	trAlphaColorInterface.AddNewShadingStep< GeometryEngine::CustomShading::AlphaMultiTextureShadowMap<GeometryEngine::GeometryMaterial::AlphaNormalMapMultiTextureMaterial> >(GeometryEngine::CustomShading::CUSTOM_SHADOWMAP);
	if (translucent)trAlphaColorInterface.AddNewShadingStep< GeometryEngine::CustomShading::MultiTextureColorMap<GeometryEngine::GeometryMaterial::AlphaNormalMapMultiTextureMaterial> >(GeometryEngine::CustomShading::CUSTOM_COLORMAP);
	return new GeometryEngine::GeometryMaterial::AlphaNormalMapMultiTextureMaterial(&trAlphaColorInterface, ambientTexDir, diffuseTexDir, specularTexDir, emissiveTexDir, normalMapTexDir, 0.1, alpha, shininess, translucent);
}

GeometryEngine::GeometryWorldItem::GeometryLight::Light * GeometryEngine::GeometryFactory::CreateSpotlight(const QVector3D& pos, const QVector3D& direction,
	const QVector3D& diffuse, const QVector3D& ambient, const QVector3D& specular, float maxLightAngle, const QVector3D& attParams, const QVector3D & rot, GeometryWorldItem::WorldItem* parent)
{
	return new GeometryEngine::GeometryWorldItem::GeometryLight::Spotlight(maxLightAngle, attParams, direction, 
		&GeometryEngine::GeometryWorldItem::GeometryItem::Sphere(GeometryMaterial::ColorMaterial()), diffuse, ambient, specular, pos, rot, QVector3D(1.0f, 1.0f, 1.0f), nullptr, parent);
}

GeometryEngine::GeometryWorldItem::GeometryLight::Light * GeometryEngine::GeometryFactory::CreateAmbientLight(const QVector3D & pos, const QVector3D & diffuse, const QVector3D & ambient, const QVector3D & specular, const QVector3D & rot, float lightViewportSize, GeometryWorldItem::WorldItem * parent)
{
	GeometryEngine::GeometryWorldItem::GeometryItem::Quad lightQuad(GeometryMaterial::ColorMaterial(), lightViewportSize, lightViewportSize);
	return new GeometryEngine::GeometryWorldItem::GeometryLight::AmbientLight(&lightQuad, diffuse, ambient, specular, pos, rot, QVector3D(1.0f, 1.0f, 1.0f), nullptr, parent);
}

GeometryEngine::GeometryWorldItem::GeometryLight::Light * GeometryEngine::GeometryFactory::CreateDirectionalLight(const QVector3D & pos, const QVector3D & direction, 
	const QVector3D & diffuse, const QVector3D & ambient, const QVector3D & specular, const QVector3D & rot, float lightViewportSize, GeometryWorldItem::WorldItem * parent)
{
	GeometryEngine::GeometryWorldItem::GeometryItem::Quad lightQuad(GeometryMaterial::ColorMaterial(), lightViewportSize, lightViewportSize);
	return new GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalLight(direction, &lightQuad, diffuse, ambient, specular, pos, rot, QVector3D(1.0f, 1.0f, 1.0f), nullptr, parent);
}

GeometryEngine::GeometryWorldItem::GeometryLight::Light * GeometryEngine::GeometryFactory::CreatePointLight(const QVector3D & pos, const QVector3D & diffuse, const QVector3D & ambient, 
	const QVector3D & specular, const QVector3D& attParams, const QVector3D & rot, GeometryWorldItem::WorldItem * parent)
{
	return new GeometryEngine::GeometryWorldItem::GeometryLight::PointLight(attParams, &GeometryEngine::GeometryWorldItem::GeometryItem::Sphere(GeometryMaterial::ColorMaterial()), 
		diffuse, ambient, specular, pos, rot, QVector3D(1.0f, 1.0f, 1.0f), nullptr, parent);
}

GeometryEngine::GeometryWorldItem::GeometryLight::Light * GeometryEngine::GeometryFactory::CreateShadowSpotlight(const QVector3D & pos, const QVector3D & direction, 
	const GeometryEngine::GeometryItemUtils::Viewport& viewport, const QVector3D & diffuse, const QVector3D & ambient, const QVector3D & specular, float maxLightAngle, 
	const QVector3D & attParams, const QVector3D & rot, float maxShadowBias, GeometryWorldItem::WorldItem * parent)
{
	return new GeometryEngine::GeometryWorldItem::GeometryLight::ShadowSpotlight(maxLightAngle, attParams, viewport, direction, 
		&GeometryEngine::GeometryWorldItem::GeometryItem::Sphere(GeometryMaterial::ColorMaterial()), 
		diffuse, ambient, specular, pos, rot, maxShadowBias,
		QVector3D(1.0f, 1.0f, 1.0f), nullptr, parent);
}

GeometryEngine::GeometryWorldItem::GeometryLight::Light * GeometryEngine::GeometryFactory::CreateShadowSpotlight(const QVector3D & pos, const QVector3D & direction, 
	GeometryItemUtils::Viewport * viewport, const QVector3D & diffuse, const QVector3D & ambient, const QVector3D & specular, float maxLightAngle, const QVector3D & attParams, 
	const QVector3D & rot, float maxShadowBias, GeometryWorldItem::WorldItem * parent)
{
	GeometryEngine::GeometryWorldItem::GeometryLight::Light* tmp = CreateShadowSpotlight(pos, direction, *viewport, diffuse, ambient, specular, maxLightAngle, 
		attParams, rot, maxShadowBias, parent);
	delete viewport;
	return tmp;
}

GeometryEngine::GeometryWorldItem::GeometryLight::Light * GeometryEngine::GeometryFactory::CreateDirectionalShadowLight(const QVector3D & pos, const QVector3D & direction, 
	const GeometryEngine::GeometryItemUtils::Viewport & viewport, const QVector3D & diffuse, const QVector3D & ambient, const QVector3D & specular, 
	const QVector3D & rot, float maxShadowBias, float lightViewportSize, GeometryWorldItem::WorldItem * parent)
{
	GeometryEngine::GeometryWorldItem::GeometryItem::Quad lightQuad(GeometryMaterial::ColorMaterial(), lightViewportSize, lightViewportSize);
	return new GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalShadowLight(viewport, direction, &lightQuad, diffuse, ambient, specular, pos, rot, 
		maxShadowBias, QVector3D(1.0f, 1.0f, 1.0f), nullptr, parent);
}

GeometryEngine::GeometryWorldItem::GeometryLight::Light * GeometryEngine::GeometryFactory::CreateDirectionalShadowLight(const QVector3D & pos, const QVector3D & direction, 
	GeometryItemUtils::Viewport * viewport, const QVector3D & diffuse, const QVector3D & ambient, const QVector3D & specular, const QVector3D & rot, float maxShadowBias, float lightViewportSize,
	GeometryWorldItem::WorldItem * parent)
{
	GeometryEngine::GeometryWorldItem::GeometryLight::Light* tmp = CreateDirectionalShadowLight(pos, direction, *viewport, diffuse, ambient, specular, rot,
		maxShadowBias, lightViewportSize, parent);
	delete viewport;
	return tmp;
}

GeometryEngine::GeometryItemUtils::Viewport * GeometryEngine::GeometryFactory::CreateOrtographicViewport(const QVector4D & viewportSize, const QRect & orthoSize, GLdouble zNear, GLdouble zFar)
{
	return new GeometryEngine::GeometryItemUtils::OrtographicViewport(viewportSize, orthoSize, zNear, zFar);
}

GeometryEngine::GeometryItemUtils::Viewport * GeometryEngine::GeometryFactory::CreatePerspectiveViewport(const QVector4D & viewportSize, GLdouble fovy, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar)
{
	return new GeometryEngine::GeometryItemUtils::PerspectiveViewport(viewportSize, fovy, aspectRatio, zNear, zFar );
}

GeometryEngine::GeometryWorldItem::GeometryCamera::Camera * GeometryEngine::GeometryFactory::CreateDeferredShadingCamera(const GeometryItemUtils::Viewport & viewport, const QVector3D & pos, bool autoResize, const QVector3D & rot, const QVector3D & scale, const GeometryWorldItem::GeometryCamera::CameraTargets& target, GeometryWorldItem::WorldItem * parent)
{
	return new GeometryEngine::GeometryWorldItem::GeometryCamera::DeferredShadingCamera(
		GeometryEngine::GeometryRenderData::RenderBuffersData(GeometryEngine::GeometryBuffer::CompleteColorPostProcessBuffer(), &GeometryEngine::GeometryBuffer::ShadingBuffer()),
		viewport, autoResize, pos, rot, QVector3D(1.0f, 1.0f, 1.0f), target, parent);
}

GeometryEngine::GeometryWorldItem::GeometryCamera::Camera * GeometryEngine::GeometryFactory::CreateDeferredShadingCamera(GeometryItemUtils::Viewport * viewport, const QVector3D & pos, bool autoResize, const QVector3D & rot, const QVector3D & scale, const GeometryWorldItem::GeometryCamera::CameraTargets& target, GeometryWorldItem::WorldItem * parent)
{
	GeometryEngine::GeometryWorldItem::GeometryCamera::Camera* tmp = CreateDeferredShadingCamera(*viewport, pos, autoResize, rot, scale, target, parent);
	delete viewport;
	return tmp;
}

GeometryEngine::GeometryPostProcess::PostProcess * GeometryEngine::GeometryFactory::CreateGreyScalePostProcess(float viewportSize)
{
	GeometryEngine::GeometryWorldItem::GeometryItem::Quad tmpQuad(GeometryMaterial::ColorMaterial(), viewportSize, viewportSize);
	return new GeometryEngine::GeometryPostProcess::SinglePassPostProcess::GreyScalePostProcess(tmpQuad);
}

GeometryEngine::GeometryPostProcess::PostProcess * GeometryEngine::GeometryFactory::CreateBlurPostProcess(float viewportSize)
{
	GeometryEngine::CustomShading::CustomPostProcessStepInterface doublePassManager;
	doublePassManager.AddNewPostProcessStep< GeometryEngine::CustomShading::CallSecondStep <GeometryEngine::GeometryPostProcess::DoublePassPostProcess::BlurPostProcess> >
		(GeometryEngine::CustomShading::CustomPostProcessSteps::SECOND_STEP);

	GeometryEngine::GeometryWorldItem::GeometryItem::Quad tmpQuad(GeometryMaterial::ColorMaterial(), viewportSize, viewportSize);
	return new GeometryEngine::GeometryPostProcess::DoublePassPostProcess::BlurPostProcess(tmpQuad, &doublePassManager);
}

void GeometryEngine::GeometryFactory::AddTmpPostProcess(GeometryWorldItem::GeometryCamera::Camera & cam, GeometryPostProcess::PostProcess * proc)
{
	cam.AddPostProcess(*proc);
	delete proc;
}
