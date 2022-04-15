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
	class BuffersInfo;

	namespace LightUtils
	{
		class LightComponentManager;
	}

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
				/// \param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
				/// \param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
				/// \param specular Specular component of the light. Each color goes from 0.0 to 1.0
				/// \param pos Initial position of the item
				/// \param rot Initial rotaion of the item
				/// \param scale Initial scale to be applied to this item model
				/// \param manager Light component manager, defaults to nullptr.
				/// \param parent Pointer to this items parent item, nullptr if none.
				Light(const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), const LightUtils::LightComponentManager* const manager = nullptr, WorldItem* parent = nullptr);

				/// Copy constructor
				/// \param ref Const reference to Light to be copied
				Light(const Light& ref) { copy(ref); };

				/// Destructor
				virtual ~Light();

				/// Calls the light shader program.
				/// \param vertexBuf Pointer to the vertex buffer
				/// \param indexBuffer Pointer to the index buffer
				/// \param transformData Light source matrices
				/// \param gBufferTextureInfo Geometry buffer textures, where the lighting result image will be stored
				/// \param viewPos Position of the camera, used for reflected lights
				/// \param totalVertexNum Total amount of vertices
				/// \param totalIndexNum Total amount of indices
				virtual void CalculateLighting(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const LightingTransformationData& transformData, 
					const BuffersInfo& buffInfo, const QVector3D& viewPos, unsigned int totalVertexNum, unsigned int totalIndexNum);

				/// Returns the Light functionalities manager for the light
				/// \return Pointer to the light functionalities manager
				virtual LightUtils::LightComponentManager* GetLightFunctionalities() { return mpFunctionalitiesManager; }

				/// Method called when the viewport is resized to be implemented by child classes. Updates textures used to calculate the lighting.
				/// \param screenWidth New width of the screen in pixels
				/// \param screenHeight New Height of the screen in pixels
				virtual void ResizeElements(int screenWidth, int screenHeight) {}
				/// Method to obtain light transformation matrices by name
				/// \return LightingTransformationData pointer, null if the transformation does not exist
				virtual const LightingTransformationData* const GetLightTransformationMatrices(LightTransformationMatrices transformation) { return mMatricesMap[transformation]; }
				/// Factory method. Returns a copy of this object.
				/// \return A copy of this object.
				virtual Light* Clone() const = 0;
			protected:
				QVector3D mColorDiffuse; // Vec3 color  + float intensity
				QVector3D mColorAmbient;
				QVector3D mColorSpecular;
				/// Lighting shader
				QOpenGLShaderProgram* mpProgram; 
				Configuration::ConfigurationManager* mpConfInstance;
				ShaderFiles::ShaderManager* mpShaderManager;
				LightUtils::LightComponentManager* mpFunctionalitiesManager;
				std::string mVertexShaderKey;
				std::string mFragmentShaderKey;

				std::map<LightTransformationMatrices, const LightingTransformationData*> mMatricesMap;
				///Method that checks what light functionalities the manager contains and acts on them
				virtual void checkLightFunctionalities() {}
				/// Initializes managers and shaders
				virtual void initLight();
				/// Loads and compiles light shader programs
				virtual void initLightProgram();
				/// Abstract method. Sets the shaders that should be loaded
				virtual void initLightShaders() = 0;
				/// Abstract method. Sends parameters to the shaders.
				/// \param transformData Matrices of the light
				/// \param buffInfo Data from the textures of the buffers
				/// \param viewPos Position of the camera
				virtual void setProgramParameters(const LightingTransformationData& transformData, const BuffersInfo& buffInfo, const QVector3D& viewPos) = 0;
				/// Abstract method. Binds shaders and draws.
				/// \param vertexBuf Vertex buffer
				/// \param indexBuf IndexBuffer
				/// \param totalVertexNum Number of vetices
				/// \param titalIndexNum Number of indices
				virtual void calculateContribution(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum) = 0;
				/// Copies the data from a Light into this object
				/// \param ref Light to be copied
				virtual void copy(const Light& ref);
			};
		}
	}
}

#endif