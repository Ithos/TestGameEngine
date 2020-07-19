#pragma once

#ifndef GEOMETRYLIGHT_H
#define GEOMETRYLIGHT_H

#include <map>

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include <ConfigurationManager.h>
#include <ShaderManager.h>

#include "../WorldItem.h"

namespace GeometryEngine
{
	class LightingTransformationData;
	class GBufferTextureInfo;

	namespace GeometryWorldItem
	{
		///namespace for all lights
		namespace GeometryLight
		{
			/// Struct that contains string constants with that represent keys for Light shaders
			struct LightShaderConstants
			{
				static const std::string AMBIENT_LIGHT_VERTEX_SHADER;
				static const std::string AMBIENT_LIGHT_FRAGMENT_SHADER;

				static const std::string DIRECTIONAL_LIGHT_VERTEX_SHADER;
				static const std::string DIRECTIONAL_LIGHT_FRAGMENT_SHADER;

				static const std::string POINT_LIGHT_VERTEX_SHADER;
				static const std::string POINT_LIGHT_FRAGMENT_SHADER;

				static const std::string FLASHLIGHT_VERTEX_SHADER;
				static const std::string FLASHLIGHT_FRAGMENT_SHADER;

				static const std::string DEFERRED_SHADING_VERTEX_SHADER;

				static const std::string NULL_FRAGMENT_SHADER;
				static const std::string POSITION_VERTEX_SHADER;

				static const std::string EMISSIVE_LIGHTING_FRAGMENT_SHADER;
			};

			enum LightTransformationMatrices
			{
				LIGHTSPACE_TRANSFORMATION_MATRICES
			};

			/// Base class for world objects that represent a light source in the scene
			class Light : public WorldItem
			{
			public:
				/// Constructor
				/// param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
				/// param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
				/// param specular Specular component of the light. Each color goes from 0.0 to 1.0
				/// param pos Initial position of the item
				/// param rot Initial rotaion of the item
				/// param scale Initial scale to be applied to this item model
				/// param parent Pointer to this items parent item, nullptr if none.
				Light(const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);

				/// Copy constructor
				/// param ref Const reference to Light to be copied
				Light(const Light& ref) { copy(ref); };

				/// Destructor
				virtual ~Light();

				/// Calls the light shader program.
				/// param vertexBuf Pointer to the vertex buffer
				/// param indexBuffer Pointer to the index buffer
				/// param transformData Light source matrices
				/// param gBufferTextureInfo Geometry buffer textures, where the lighting result image will be stored
				/// param viewPos Position of the camera, used for reflected lights
				/// param totalVertexNum Total amount of vertices
				/// param totalIndexNum Total amount of indices
				virtual void CalculateLighting(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const LightingTransformationData& transformData, 
					const GBufferTextureInfo& gBuffTexInfo, const QVector3D& viewPos, unsigned int totalVertexNum, unsigned int totalIndexNum);

				/// Method to be implemented by child classes. Calculates the lighting just within the volume defined by a geometry. 2D geometries apply lighting to everything that the camera sees throug it.
				/// param projectionMatrix Camera projection matrix
				/// param viewMatrix Camera view matrix
				/// param gBuffInfo Textures from the geometry buffer of the camera
				/// param viewPos Position of the camera
				virtual void LightFromBoundignGeometry(const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const GBufferTextureInfo& gBuffTexInfo, const QVector3D& viewPos)
				{
					assert(GetBoundingGeometry() != nullptr && "Bounding geometry not found");
				}

				/// Method to be implemented by child classes. Updates the stencil buffer which indicates where the light calculations will be applied
				/// param projectionMatrix Camera projection matrix
				/// param viewMatrix View matrix of the camera
				virtual void CalculateStencil(const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix)
				{
					assert(GetStencilTest() && "Stencil test not found");
				}

				/// Method to expose the default (solid) shadow map calculation that some lights might have. Lights that cast shadows are supposed to override this method. By default triggers an assert.
				/// param vertexBuf Pointer to the vertex buffer
				/// param indexBuffer Pointer to the index buffer
				/// param modelMatrix model matrix of the item to be added to the shadow map
				/// param totalVertexNum Total amount of vertices
				/// param totalIndexNum Total amount of indices
				virtual void CalculateShadowMap(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& modelMatrix, unsigned int totalVertexNum, unsigned int totalIndexNum) 
				{
					assert(GetCastShadows() && "Shadow map calculation not found");
				}

				/// Method to be implemented by child classes. Returns a pointer to the boundng geometry of the ligh or nullptr if there is none
				/// return Pointer to the boundng geometry of the ligh or nullptr if there is none
				virtual WorldItem* const GetBoundingGeometry() { return nullptr; }
				/// Method to be implemented by child classes. Returns true if the light performs a stencil test.
				virtual bool GetStencilTest() { return false; }
				/// Method to be implemented by child classes. Returns true if the light calculates shadows
				virtual bool GetCastShadows() { return false; }
				/// Method to be implemented by child classes. Updates elements when the screen is resized.
				virtual void ResizeElements(int screenWidth, int screenHeight) {}
				/// Method to obtain light transformation matrices by name
				/// return LightingTransformationData pointer, null if the transformation does not exist
				virtual const LightingTransformationData* const GetLightTransformationMatrices(LightTransformationMatrices transformation) { return mMatricesMap[transformation]; }
				/// Factory method. Returns a copy of this object.
				/// return A copy of this object.
				virtual Light* Clone() const = 0;
			protected:
				QVector3D mColorDiffuse; // Vec3 color  + float intensity
				QVector3D mColorAmbient;
				QVector3D mColorSpecular;
				/// Lighting shader
				QOpenGLShaderProgram* mpProgram; 
				Configuration::ConfigurationManager* mpConfInstance;
				ShaderFiles::ShaderManager* mpShaderManager;
				std::string mVertexShaderKey;
				std::string mFragmentShaderKey;

				std::map<LightTransformationMatrices, const LightingTransformationData*> mMatricesMap;

				/// Initializes managers and shaders
				virtual void initLight();
				/// Loads and compiles light shader programs
				virtual void initLightProgram();
				/// Abstract method. Sets the shaders that should be loaded
				virtual void initLightShaders() = 0;
				/// Abstract method. Sends parameters to the shaders.
				/// param transformData Matrices of the light
				/// param GBufferTextureInfo Data from the textures of the geometry buffer 
				/// param viewPos Position of the camera
				virtual void setProgramParameters(const LightingTransformationData& transformData, const GBufferTextureInfo& GBuffTexInfo, const QVector3D& viewPos) = 0;
				/// Abstract method. Binds shaders and draws.
				/// param vertexBuf Vertex buffer
				/// param indexBuf IndexBuffer
				/// param totalVertexNum Number of vetices
				/// param titalIndexNum Number of indices
				virtual void calculateContribution(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum) = 0;
				/// Copies the data from a Light into this object
				/// param ref Light to be copied
				virtual void copy(const Light& ref);
			};
		}
	}
}

#endif