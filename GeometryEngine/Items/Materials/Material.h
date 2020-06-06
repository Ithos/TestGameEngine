#pragma once

#ifndef GEOMETRYMATERIAL_H
#define GEOMETRYMATERIAL_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QopenGlFunctions>

#include <list>

#include <ConfigurationManager.h>
#include <ShaderManager.h>

#include "Items\CommonInerfaces\CustomShadingInterface.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryItem
		{
			class GeometryItem;
		}
	}

	///namespace for all materials
	namespace GeometryMaterial
	{
		/// Struct that contains the keys of the material shaders
		struct MaterialConstants
		{
			static const std::string TEXTURE_MATERIAL_VERTEX_SHADER;
			static const std::string TEXTURE_MATERIAL_FRAGMENT_SHADER;
			static const std::string COLOR_MATERIAL_VERTEX_SHADER;
			static const std::string COLOR_MATERIAL_FRAGMENT_SHADER;
			static const std::string VERTEX_COLOR_MATERIAL_VERTEX_SHADER;
			static const std::string VERTEX_COLOR_MATERIAL_FRAGMENT_SHADER;
			static const std::string MULTI_TEXTURE_MATERIAL_FRAGMENT_SHADER;
			static const std::string TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER;
			static const std::string MULTI_TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER;
			static const std::string NORMALMAP_TEXTURE_MATERIAL_VERTEX_SHADER;

			static const std::string ALPHA_COLOR_MATERIAL_VERTEX_SHADER;
			static const std::string ALPHA_COLOR_MATERIAL_FRAGMENT_SHADER;
			static const std::string ALPHA_TEXTURE_MATERIAL_VERTEX_SHADER;
			static const std::string ALPHA_MULTI_TEXTURE_MATERIAL_FRAGMENT_SHADER;
			static const std::string ALPHA_MULTI_TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER;
			static const std::string ALPHA_TEXTURE_MATERIAL_FRAGMENT_SHADER;
			static const std::string ALPHA_NORMALMAP_TEXTURE_MATERIAL_VERTEX_SHADER;
			static const std::string ALPHA_TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER;

			static const std::string POSITION_VERTEX_SHADER;
			static const std::string POSITION_TEX_COORD_VERTEX_SHADER;
			static const std::string ALPHA_COLOR_SHADOWMAP;
			static const std::string ALPHA_TEXTURE_SHADOWMAP;
			static const std::string ALPHA_MULTI_TEXTURE_SHADOWMAP;
		};

		/// Base class for all materials for world objects. 
		class Material : public CustomShadingInterface
		{
		public:
			/// Constructor 
			/// param ambient Ambient rgb color component of the material. Each color goes from 0.0 to 1.0
			/// param diffuse Diffuse rgb color component of the material. Each color goes from 0.0 to 1.0
			/// param specular Specular rgb color component of the material. Each color goes from 0.0 to 1.0
			/// param emissive Emissive rgb color component of the material. Each color goes from 0.0 to 1.0
			/// param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders. 
			Material(float shininess = 10.0f);

			/// Copy constructor
			/// param ref Object to be copied.
			Material(const Material& mat);

			/// Destructor
			virtual ~Material();

			void SetShininess(float shininess) { mShininess = checkShininessValue(shininess); }

			/// Render material shaders
			/// param vertexBuf Pointer to the vertex buffer
			/// param indexBuffer Pointer to the index buffer
			/// param totalVertexNumber Total amount of vertices
			/// param totalIndexNumberTotal amount of indices
			/// param projection Projection Matrix
			/// param view View matrix
			/// param parent Parent geometry item
			virtual void Draw(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber,
				const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent);
			/// Abstract method. Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual Material* Clone() const = 0;

			/// Returns whether the material allows transparencies. Method to be overwritten by child classes. By default returns false.
			virtual bool IsTransparent() { return false; }

			/// Method to be implemented by child classes, by default triggers an assert. Applies the material shadow map shaders.
			/// param vertexBuf Pointer to the vertex buffer
			/// param indexBuffer Pointer to the index buffer
			/// param modelMatrix model matrix of the item to be added to the shadow map
			/// param totalVertexNum Total amount of vertices
			/// param totalIndexNum Total amount of indices
			virtual void CalculateCustomShadowMap(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& modelViewProjection, unsigned int totalVertexNum, unsigned int totalIndexNum) override
			{
				assert(GetApplyCustomShadowMap() && "Custom shadow map not found");
			}

			/// Method to be reimplemented by child classes. Returns whether custom shadow map shaders should be used.
			virtual bool GetApplyCustomShadowMap() override
			{
				return false;
			}

			/// Method to be reimplemented by child classes, by default triggers an assert. Sets whether custom shadow map shaders should be used
			/// param applyCustom true if custom shadow maps should be used false otherwise.
			virtual void SetApplyCustomShadowMap(bool applyCustom) override
			{
				assert(GetApplyCustomShadowMap() && "Custom shadow map not found");
			}

			/// If set to true both frontfaces and backfaces of the model will be drawn
			void SetDrawBacksideFaces(bool drawBackside) { mDrawBothFaces = drawBackside; }
			/// Returns if bckside faces will be drawn
			bool GetDrawBacksideFaces() { return mDrawBothFaces; }

		protected:
			float mShininess;
			bool mLit;
			bool mApplyCustomShadowMap;
			bool mDrawBothFaces;

			QOpenGLShaderProgram* mpProgram; // Lighting shader
			ShaderFiles::ShaderManager* mpShaderManager;
			Configuration::ConfigurationManager* mpConfInstance;

			std::string mVertexShaderKey;
			std::string mFragmentShaderKey;

			/// Initializes managers and shaders
			virtual void initMaterial();
			/// Abstract method. Sets the shaders that should be loaded
			virtual void initShaders() = 0;
			/// Loads and compiles material shader program
			virtual void initProgram();
			/// Copies the data of a Material object to the current object
			/// param ref Material to be copied
			virtual void copy(const Material& mat);

			/// Abstract method. Sends parameters to the shaders.
			/// param projection Projection matrix
			/// param view View matrix			 
			/// param parent geometry item
			virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent) = 0;
			/// Abstract method. Binds shaders and draws.
			/// param vertexBuf Vertex buffer
			/// param indexBuf IndexBuffer
			/// param totalVertexNum Number of vetices
			/// param titalIndexNum Number of indices
			virtual void drawMaterial(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) = 0;
			/// Applies internal modifiers and calls draw material.
			/// param vertexBuf Vertex buffer
			/// param indexBuf IndexBuffer
			/// param totalVertexNum Number of vetices
			/// param titalIndexNum Number of indices
			virtual void modifyDrawMaterial(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber);
		private:
			float checkShininessValue(float shininessValue) { if (shininessValue <= 0.0f) return 0.0001f; return shininessValue; }
		};
	}
}

#endif