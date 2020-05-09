#pragma once

#ifndef GEOMETRYSHADOWMAPLIGHT_H
#define GEOMETRYSHADOWMAPLIGHT_H

#include "../../StencilTestLight.h"

namespace GeometryEngine
{
	namespace GeometryItemUtils
	{
		class Viewport;
	}

	namespace GeometryWorldItem
	{
		namespace GeometryLight
		{
			/// Struct that contains string constants with that represent keys for shadow map shaders
			struct ShadowMapConstants
			{
				static const std::string SHADOW_MAP_FRAGMENT_SHADER;
				static const std::string POSITION_VERTEX_SHADER;
				static const std::string DIRECTIONAL_SHADOW_LIGHT_FRAGMENT_SHADER;
				static const std::string DIRECTIONAL_SHADOW_LIGHT_VERTEX_SHADER;
				static const std::string SPOTLIGHT_SHADOW_LIGHT_FRAGMENT_SHADER;
				static const std::string SPOTLIGHT_SHADOW_LIGHT_VERTEX_SHADER;
			};

			/// Abstract class that defines common methods for lights using shadow maps
			class ShadowMapLight : public StencilTestLight
			{
			public:
				/// Constructor
				/// param viewport Viewport that will be used for the shadowmap calculation.
				/// param direction Shadowmaps work only with single direction lights 
				/// param boundingBox Geomtry used to render the light. Light will be applied to every part of the scene that the geometry is drawn on top of.
				/// param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
				/// param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
				/// param specular Specular component of the light. Each color goes from 0.0 to 1.0
				/// param pos Initial position of the item
				/// param rot Initial rotaion of the item
				/// param scale Initial scale to be applied to this item model
				/// param parent Pointer to this items parent item, nullptr if none.
				ShadowMapLight(const GeometryItemUtils::Viewport& viewport, const QVector3D& direction, GeometryItem::GeometryItem* boundingBox = nullptr, 
					const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);

				/// Copy constructor
				/// param ref Const reference to StencilTestLight to be copied
				ShadowMapLight(const ShadowMapLight& ref) { copy(ref); };

				/// Destructor
				virtual ~ShadowMapLight();

				/// Applies the shadow map shaders.
				/// param vertexBuf Pointer to the vertex buffer
				/// param indexBuffer Pointer to the index buffer
				/// param modelMatrix model matrix of the item to be added to the shadow map
				/// param totalVertexNum Total amount of vertices
				/// param totalIndexNum Total amount of indices
				virtual void CalculateShadowMap(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& modelMatrix, unsigned int totalVertexNum, unsigned int totalIndexNum) override;

				/// Updates the model matrix of this Item and updates the View matrix of the viewport
				/// param calculateChildren true if children matrix should be updated
				virtual void UpdateModelMatrix(bool updateChildren = false) override;

				/// Returns true if the light calculates shadows
				virtual bool GetCastShadows() override { return true; }
				///  Resizes the viewport used to calculate the shadowmap
				virtual void ResizeElements(int screenWidth, int screenHeight) override;
				///Sets the light direction. This direction is affected by the item rotation
				void SetDirection(const QVector3D& dir) { mDirection = dir; }
				///Returs the light direction
				const QVector3D& GetDirection() { return mDirection; }

				/// Factory method. Returns a copy of this object.
				/// return A copy of this object.
				virtual ShadowMapLight* Clone() const = 0;

			protected:
				/// Private constructor for object copies
				ShadowMapLight() {}

				/// Shadow map shader
				QOpenGLShaderProgram* mpShadowMapProgram; 

				std::string mSMapVertexShaderKey;
				std::string mSMapFragmentShaderKey;

				GeometryItemUtils::Viewport* mpViewport;
				QVector3D mDirection;

				/// Initializes managers and shaders for the shadow map calculation
				virtual void initShadow();
				/// Loads and compiles shadow map shader programs
				virtual void initShadowProgram();
				/// Sets the shaders that should be loaded for the shadow map calculation
				virtual void initShadowShaders();
				/// Sends parameters to the shadow map shaders.
				virtual void setShadowProgramParameters(const QMatrix4x4& modelMatrix);
				/// Binds shaders and calculates the shadow map.
				/// param vertexBuf Vertex buffer
				/// param indexBuf IndexBuffer
				/// param totalVertexNum Number of vetices
				/// param titalIndexNum Number of indices
				virtual void renderShadowMap(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum);
				/// Copies the data from a ShadowMapLight into this object
				/// param ref ShadowMapLight to be copied
				virtual void copy(const ShadowMapLight& ref);
			};
		}
	}
}
#endif