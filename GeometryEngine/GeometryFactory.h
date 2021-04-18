#pragma once

#ifndef GEOMETRYFACTORY_H

#include <string>
#include <qvector3d.h>
#include "Items/Materials/TextureMaterialCommons.h"

namespace GeometryEngine
{
	namespace LightUtils
	{
		class LightComponentManager;
	}

	namespace GeometryMaterial
	{
		class Material;
	}

	namespace GeometryWorldItem
	{
		class WorldItem;

		namespace GeometryCamera
		{
			class Camera;
		}

		namespace GeometryItem
		{
			class Cube;
			class Sphere;
		}

		namespace GeometryLight
		{
			class Light;
		}
	}

	namespace GeometryPostProcess
	{
		class PostProcess;
	}

	namespace GeometryItemUtils
	{
		class Viewport;
	}

	/// Different modes of applying the texture
	/// REPEAT -> Replicates the texture on every face
	/// MAP -> Maps the texture to cover all faces of the geometry
	enum TEXTURE_MODE
	{
		REPEAT,
		MAP
	};

	/// Factory to simplify the creation of scene objects. The property of the objects created is delegated to the caller.
	class GeometryFactory
	{
	public:

		///Helper method to create a skybox
		/// \param textureKey Key of the texture to be loaded 
		/// \param skyboxSize Size of the skybox cube
		static GeometryWorldItem::GeometryItem::GeometryItem* CreateSkyboxCube(const std::string& textureKey, float skyboxSize );

		///Helper method to create a sphere
		/// \param mat Material to be used for rendering
		/// \param size Radius of the sphere
		/// \param pos Initial position of the sphere
		/// \param parent Pointer to the sphere parent item, nullptr if none.
		static GeometryWorldItem::GeometryItem::GeometryItem* CreateSphere(const GeometryMaterial::Material & mat, float size, const QVector3D & pos, GeometryWorldItem::WorldItem * parent = nullptr);

		///Convenience method that overrides CreateSphere to be called using a temporary material pointer. This method deletes the material pointer after creating the item.
		/// \param mat Temporary material pointer to be used for rendering. The pointer will be deleted after creating the item.
		/// \param size Radius of the sphere
		/// \param pos Initial position of the sphere
		/// \param parent Pointer to the sphere parent item, nullptr if none.
		static GeometryWorldItem::GeometryItem::GeometryItem* CreateSphere(GeometryMaterial::Material* mat, float size, const QVector3D & pos, GeometryWorldItem::WorldItem * parent = nullptr);

		///Helper method to create a cube
		/// \param mat Material to be used for rendering
		/// \param mode Mode of aplying the texture. In MAP mode vertex coordinates should be like the ones in the file skyboxTexture in settings/textures
		/// \param size Side lenght for the cube
		/// \param pos Initial position of the item
		/// \param rot Initial rotaion of the item
		/// \param scale Initial scale to be applied to this item model
		/// \param parent Pointer to this items parent item, nullptr if none
		static GeometryWorldItem::GeometryItem::GeometryItem* CreateCube(const GeometryMaterial::Material& mat, TEXTURE_MODE mode, float size, const QVector3D& pos, 
			const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), GeometryWorldItem::WorldItem* parent = nullptr);

		///Convenience method that overrides CreateCube to be called using a temporary material pointer. This method deletes the material pointer after creating the item.
		/// \param mat Temporary material pointer to be used for rendering. The pointer will be deleted after creating the item.
		/// \param mode Mode of aplying the texture. In MAP mode vertex coordinates should be like the ones in the file skyboxTexture in settings/textures
		/// \param size Side lenght for the cube
		/// \param pos Initial position of the item
		/// \param rot Initial rotaion of the item
		/// \param scale Initial scale to be applied to this item model
		/// \param parent Pointer to this items parent item, nullptr if none
		static GeometryWorldItem::GeometryItem::GeometryItem* CreateCube(GeometryMaterial::Material* mat, TEXTURE_MODE mode, float size, const QVector3D& pos,
			const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), GeometryWorldItem::WorldItem* parent = nullptr);

		///Helper method to create a 2D quad
		/// \param mat Material to be used for rendering
		/// \param width Quad Width
		/// \param size Quad height
		/// \param pos Initial position of the item
		/// \param rot Initial rotaion of the item
		/// \param scale Initial scale to be applied to this item model
		/// \param parent Pointer to this items parent item, nullptr if none
		static GeometryWorldItem::GeometryItem::GeometryItem* CreateQuad(const GeometryMaterial::Material& mat, float width, float height, const QVector3D& pos,
			const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), GeometryWorldItem::WorldItem* parent = nullptr);

		///Convenience method that overrides CreateQuad to be called using a temporary material pointer. This method deletes the material pointer after creating the item.
		/// \param mat Temporary material pointer to be used for rendering. The pointer will be deleted after creating the item
		/// \param width Quad Width
		/// \param size Quad height
		/// \param pos Initial position of the item
		/// \param rot Initial rotaion of the item
		/// \param scale Initial scale to be applied to this item model
		/// \param parent Pointer to this items parent item, nullptr if none
		static GeometryWorldItem::GeometryItem::GeometryItem* CreateQuad(GeometryMaterial::Material* mat, float width, float height, const QVector3D& pos,
			const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), GeometryWorldItem::WorldItem* parent = nullptr);

		///Helper method to create a color material 
		/// \param ambient Ambient rgb color component of the material. Each color goes from 0.0 to 1.0
		/// \param diffuse Diffuse rgb color component of the material. Each color goes from 0.0 to 1.0
		/// \param specular Specular rgb color component of the material. Each color goes from 0.0 to 1.0
		/// \param emissive Emissive rgb color component of the material. Each color goes from 0.0 to 1.0
		/// \param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders. 
		static GeometryMaterial::Material* CreateColorMaterial(const QVector3D& ambient, const QVector3D& diffuse, const QVector3D& specular, 
			const QVector3D& emissive = QVector3D(0.0f, 0.0f, 0.0f), float shininess = 10.0f);

		///Helper method to create a transparent color material 
		/// \param ambient Ambient rgb color component of the material. Each color goes from 0.0 to 1.0
		/// \param diffuse Diffuse rgb color component of the material. Each color goes from 0.0 to 1.0
		/// \param specular Specular rgb color component of the material. Each color goes from 0.0 to 1.0
		/// \param emissive Emissive rgb color component of the material. Each color goes from 0.0 to 1.0
		/// \param alpha Main alpha value for the whole object, clamps to 0.0 to 1.0 range. This value gets multiplied to every fragment color alpha.
		/// \param translucent Indicates whether the material casts a translucent shadow or not
		/// \param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders. 
		static GeometryMaterial::Material* CreateAlphaColorMaterial(const QVector3D& ambient, const QVector3D& diffuse, const QVector3D& specular, 
			const QVector3D& emissive, float = 1.0f, bool translucent = false, float shininess = 10.0f);

		///Helper method to create a texture material
		/// \param texDir Key of the texture to be loaded
		/// \param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders. 
		static GeometryMaterial::Material* CreateTextureMaterial(const std::string& texDir, float shininess = 10.0f);

		///Helper method to create a transparent texture material
		/// \param texDir Key of the texture to be loaded
		/// \param alpha Main alpha value for the whole object, clamps to 0.0 to 1.0 range. This value gets multiplied to every fragment color alpha.
		/// \param translucent Indicates whether the material casts a translucent shadow or not
		/// \param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders. 
		static GeometryMaterial::Material* CreateAlphaTextureMaterial(const std::string& texDir, float alpha = 1.0f, bool translucent = false, float shininess = 10.0f);

		///Helper method to create a texture material with a normalmap
		/// \param texDir Key of the texture to be loaded
		/// \param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders. 
		static GeometryMaterial::Material* CreateTextureMaterial(const std::string& texDir, const std::string& normalMapTexDir, float shininess = 10.0f);

		///Helper method to create a transparent texture material with a normalmap
		/// \param texDir Key of the texture to be loaded
		/// \param alpha Main alpha value for the whole object, clamps to 0.0 to 1.0 range. This value gets multiplied to every fragment color alpha.
		/// \param translucent Indicates whether the material casts a translucent shadow or not
		/// \param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders. 
		static GeometryMaterial::Material* CreateAlphaTextureMaterial(const std::string& texDir, const std::string& normalMapTexDir, float alpha = 1.0f, bool translucent = false, 
			float shininess = 10.0f);

		///Helper method to create a multi texture material
		/// \param ambientTexDir Key to a texture that will be used as ambient color
		/// \param diffuseTexDir Key to a texture that will be used as diffuse color
		/// \param specularTexDir Key to a texture that will be used as specular color
		/// \param emissiveTexDir Key to a texture that will be used as emissive color
		/// \param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders. 
		static GeometryMaterial::Material* CreateTextureMaterial(const std::string& ambientTexDir, const std::string& diffuseTexDir, const std::string& specularTexDir, 
			const std::string& emissiveTexDir = GeometryMaterial::TextureConstant::TEST_BLACK_TEXTURE, float shininess = 10.0f);

		///Helper method to create a transparent multi texture material with a normalmap
		/// \param ambientTexDir Key to a texture that will be used as ambient color
		/// \param diffuseTexDir Key to a texture that will be used as diffuse color
		/// \param specularTexDir Key to a texture that will be used as specular color
		/// \param emissiveTexDir Key to a texture that will be used as emissive color
		/// \param alpha Main alpha value for the whole object, clamps to 0.0 to 1.0 range. This value gets multiplied to every fragment color alpha.
		/// \param translucent Indicates whether the material casts a translucent shadow or not
		/// \param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders. 
		static GeometryMaterial::Material* CreateAlphaTextureMaterial(const std::string& ambientTexDir, const std::string& diffuseTexDir, const std::string& specularTexDir,
			const std::string& emissiveTexDir = GeometryMaterial::TextureConstant::TEST_BLACK_TEXTURE, float alpha = 1.0f, bool translucent = false, float shininess = 10.0f);
		
		///Helper method to create a multi texture material with a normalmap
		/// \param ambientTexDir Key to a texture that will be used as ambient color
		/// \param diffuseTexDir Key to a texture that will be used as diffuse color
		/// \param specularTexDir Key to a texture that will be used as specular color
		/// \param normalMapTexDir Key to a texture that contains the object normal map
		/// \param emissiveTexDir Key to a texture that will be used as emissive color
		/// \param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders. 
		static GeometryMaterial::Material* CreateTextureMaterial(const std::string& ambientTexDir, const std::string& diffuseTexDir, const std::string& specularTexDir,
			const std::string& normalMapTexDir, const std::string& emissiveTexDir = GeometryMaterial::TextureConstant::TEST_BLACK_TEXTURE, float shininess = 10.0f);

		///Helper method to create a transparent multi texture material with a normalmap
		/// \param ambientTexDir Key to a texture that will be used as ambient color
		/// \param diffuseTexDir Key to a texture that will be used as diffuse color
		/// \param specularTexDir Key to a texture that will be used as specular color
		/// \param normalMapTexDir Key to a texture that contains the object normal map
		/// \param emissiveTexDir Key to a texture that will be used as emissive color
		/// \param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders. 
		static GeometryMaterial::Material* CreateAlphaTextureMaterial(const std::string& ambientTexDir, const std::string& diffuseTexDir, const std::string& specularTexDir,
			const std::string& normalMapTexDir, const std::string& emissiveTexDir = GeometryMaterial::TextureConstant::TEST_BLACK_TEXTURE, 
			float alpha = 1.0f, bool translucent = false, float shininess = 10.0f);

		///Helper method to create a spotlight
		/// \param pos Initial position of the item
		/// \param direction Direction of the light.
		/// \param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
		/// \param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
		/// \param specular Specular component of the light. Each color goes from 0.0 to 1.0
		/// \param maxLightAngle Max angle for the light. Attenuation will be applied based on angle deviation
		/// \param attParams Attenuation polynomial for the light
		/// \param boundingBox Geomtry used to render the light. Light will be applied to every part of the scene that the geometry is drawn on top of
		/// \param rot Initial rotaion of the item
		/// \param parent Pointer to this items parent item, nullptr if none
		static GeometryWorldItem::GeometryLight::Light* CreateSpotlight(const QVector3D& pos, const QVector3D& direction, 
			const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f), 
			float maxLightAngle = 45.0f, const QVector3D& attParams = QVector3D(0.1f, 0.1f, 0.01f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), 
			GeometryWorldItem::WorldItem* parent = nullptr);

		///Helper method to create an ambiental light
		/// \param pos Initial position of the item
		/// \param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
		/// \param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
		/// \param specular Specular component of the light. Each color goes from 0.0 to 1.0
		/// \param lightViewportSize Size of the square shaped viewport used to render the light
		/// \param rot Initial rotaion of the item
		/// \param parent Pointer to this items parent item, nullptr if none
		static GeometryWorldItem::GeometryLight::Light* CreateAmbientLight(const QVector3D& pos, const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), 
			const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
			float lightViewportSize = 3.0f, GeometryWorldItem::WorldItem* parent = nullptr);

		///Helper method to create a directional light
		/// \param pos Initial position of the item
		/// \param direction Direction of the light.
		/// \param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
		/// \param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
		/// \param specular Specular component of the light. Each color goes from 0.0 to 1.0
		/// \param rot Initial rotaion of the item
		/// \param lightViewportSize Size of the square shaped viewport used to render the light
		/// \param parent Pointer to this items parent item, nullptr if none
		static GeometryWorldItem::GeometryLight::Light* CreateDirectionalLight(const QVector3D& pos, const QVector3D& direction, 
			const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f),
			const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), 
			float lightViewportSize = 3.0f, GeometryWorldItem::WorldItem* parent = nullptr);

		///Helper method to create a point light
		/// \param pos Initial position of the item
		/// \param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
		/// \param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
		/// \param specular Specular component of the light. Each color goes from 0.0 to 1.0
		/// \param attParams Attenuation polynomial for the light
		/// \param rot Initial rotaion of the item
		/// \param parent Pointer to this items parent item, nullptr if none
		static GeometryWorldItem::GeometryLight::Light* CreatePointLight(const QVector3D& pos, const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), 
			const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& attParams = QVector3D(0.1f, 0.1f, 0.01f),
			const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), GeometryWorldItem::WorldItem* parent = nullptr);

		///Helper method to create a shadow spotlight
		/// \param pos Initial position of the item
		/// \param direction light direction vector
		/// \param viewport Viewport that will be used for the shadowmap calculation.
		/// \param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
		/// \param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
		/// \param specular Specular component of the light. Each color goes from 0.0 to 1.0
		/// \param maxLightAngle Max angle for the light. Attenuation will be applied based on angle deviation
		/// \param attParams Attenuation polynomial for the light
		/// \param rot Initial rotaion of the item
		/// \param mMaxShadowBias Max shadow bias value allowed when calculating dynamic shadow bias. A greater value prevents shadow acne but may cause some shadows disappear suddenly.
		/// \param parent Pointer to this items parent item, nullptr if none.
		static GeometryWorldItem::GeometryLight::Light* CreateShadowSpotlight(const QVector3D& pos, const QVector3D& direction, const GeometryItemUtils::Viewport& viewport,
			const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
			float maxLightAngle = 45.0f, const QVector3D& attParams = QVector3D(0.1f, 0.1f, 0.01f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), float maxShadowBias = 0.001f,
			GeometryWorldItem::WorldItem* parent = nullptr);

		///Convenience method that overrides CreateShadowSpotlight to be called using a temporary viewport pointer. This method deletes the viewport pointer after creating the item.
		/// \param pos Initial position of the item
		/// \param direction light direction vector
		/// \param viewport Temporary viewport pointer to be used for rendering. The pointer will be deleted after creating the item.
		/// \param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
		/// \param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
		/// \param specular Specular component of the light. Each color goes from 0.0 to 1.0
		/// \param maxLightAngle Max angle for the light. Attenuation will be applied based on angle deviation
		/// \param attParams Attenuation polynomial for the light
		/// \param rot Initial rotaion of the item
		/// \param mMaxShadowBias Max shadow bias value allowed when calculating dynamic shadow bias. A greater value prevents shadow acne but may cause some shadows disappear suddenly.
		/// \param parent Pointer to this items parent item, nullptr if none.
		static GeometryWorldItem::GeometryLight::Light* CreateShadowSpotlight(const QVector3D& pos, const QVector3D& direction, GeometryItemUtils::Viewport* viewport,
			const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
			float maxLightAngle = 45.0f, const QVector3D& attParams = QVector3D(0.1f, 0.1f, 0.01f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), float maxShadowBias = 0.001f,
			GeometryWorldItem::WorldItem* parent = nullptr);


		///Helper method to create directional shadow light
		/// \param pos Initial position of the item
		/// \param direction light direction vector
		/// \param viewport Viewport that will be used for the shadowmap calculation.
		/// \param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
		/// \param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
		/// \param specular Specular component of the light. Each color goes from 0.0 to 1.0
		/// \param rot Initial rotaion of the item
		/// \param mMaxShadowBias Max shadow bias value allowed when calculating dynamic shadow bias. A greater value prevents shadow acne but may cause some shadows disappear suddenly.
		/// \param lightViewportSize Size of the square shaped viewport used to render the light
		/// \param parent Pointer to this items parent item, nullptr if none.
		static GeometryWorldItem::GeometryLight::Light* CreateDirectionalShadowLight(const QVector3D& pos, const QVector3D& direction, const GeometryItemUtils::Viewport& viewport,
			const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
			 const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), float maxShadowBias = 0.0f, float lightViewportSize = 3.0f,
			GeometryWorldItem::WorldItem* parent = nullptr);

		///Convenience method that overrides CreateDirectionalShadowLight to be called using a temporary viewport pointer. This method deletes the viewport pointer after creating the item.
		/// \param pos Initial position of the item
		/// \param direction light direction vector
		/// \param viewport Temporary viewport pointer to be used for rendering. The pointer will be deleted after creating the item.
		/// \param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
		/// \param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
		/// \param specular Specular component of the light. Each color goes from 0.0 to 1.0
		/// \param rot Initial rotaion of the item
		/// \param mMaxShadowBias Max shadow bias value allowed when calculating dynamic shadow bias. A greater value prevents shadow acne but may cause some shadows disappear suddenly.
		/// \param lightViewportSize Size of the square shaped viewport used to render the light
		/// \param parent Pointer to this items parent item, nullptr if none.
		static GeometryWorldItem::GeometryLight::Light* CreateDirectionalShadowLight(const QVector3D& pos, const QVector3D& direction, GeometryItemUtils::Viewport* viewport,
			const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
			const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), float maxShadowBias = 0.0f, float lightViewportSize = 3.0f,
			GeometryWorldItem::WorldItem* parent = nullptr);

		///Helper method to create an ortographic viewport
		/// \param viewportSize Size of the viewport. The values indicate in order x position, yposition, width, height.
		/// \param fovy Angle that the perspective frustrum opens 
		/// \param aspectRatio Relation between width and height of the image
		/// \param zNear Nearest z coordinate to be rendered
		/// \param zFar Farthest z coordinate to be rendered
		static GeometryItemUtils::Viewport* CreateOrtographicViewport(const QVector4D& viewportSize, const QRect& orthoSize, GLdouble zNear = 0.1, GLdouble zFar = 30.0);

		///Helper method to create a perspective viewport
		/// \param viewportSize Size of the viewport. The values indicate in order x position, yposition, width, height.
		/// \param fovy Angle that the perspective frustrum opens 
		/// \param aspectRatio Relation between width and height of the image
		/// \param zNear Nearest z coordinate to be rendered
		/// \param zFar Farthest z coordinate to be rendered
		static GeometryItemUtils::Viewport* CreatePerspectiveViewport(const QVector4D& viewportSize, GLdouble fovy = 45.0, GLdouble aspectRatio = 1.0, GLdouble zNear = 0.1, GLdouble zFar = 30.0);

		///Helper method to create a deferred shading camera
		/// \param viewport Viewport object to be copied into the camera
		/// \param autoResize If true the size of the viewport changes with the size of the window
		/// \param pos Initial position of the item
		/// \param rot Initial rotaion of the item
		/// \param scale Initial scale to be applied to this item model
		/// \param parent Pointer to this items parent item, nullptr if none.
		static GeometryWorldItem::GeometryCamera::Camera* CreateDeferredShadingCamera(const GeometryItemUtils::Viewport& viewport, const QVector3D& pos, bool autoResize = true,
			const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), GeometryWorldItem::WorldItem* parent = nullptr);

		///Convenience method that overrides CreateDeferredShadingCamera to be called using a temporary material pointer. This method deletes the viewport pointer after creating the item.
		/// \param viewport Temporary viewport pointer to be copied into the camera. The pointer will be deleted after creating the camera.
		/// \param autoResize If true the size of the viewport changes with the size of the window
		/// \param pos Initial position of the item
		/// \param rot Initial rotaion of the item
		/// \param scale Initial scale to be applied to this item model
		/// \param parent Pointer to this items parent item, nullptr if none.
		static GeometryWorldItem::GeometryCamera::Camera* CreateDeferredShadingCamera(GeometryItemUtils::Viewport* viewport, const QVector3D& pos, bool autoResize = true,
			const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), GeometryWorldItem::WorldItem* parent = nullptr);

		///Helper method to create a grey scale post process
		/// \param viewportSize Size of the quad used to render the post process. It should cover the viewport completely.
		static GeometryPostProcess::PostProcess* CreateGreyScalePostProcess(float viewportSize = 3.0f);

		///Helper method to create a blur post process
		/// \param viewportSize Size of the quad used to render the post process. It should cover the viewport completely.
		static GeometryPostProcess::PostProcess* CreateBlurPostProcess(float viewportSize = 3.0f);

		///Convenience method that copies a post process to the camera and then deletes the received post process pointer
		/// \param cam Camera where the post process will be added
		/// \param proc Temporary PostProcess pointer to be copied into the camera. It will be deletad after it is copied.
		static void AddTmpPostProcess(GeometryWorldItem::GeometryCamera::Camera& cam, GeometryPostProcess::PostProcess* proc);


	private:
		/// Constructor. Private because this is a static singleton
		GeometryFactory() {}

		/// Destructor. Private because this is a static singleton
		~GeometryFactory() {}

	};
}
#endif // !GEOMETRYFACTORY_H
