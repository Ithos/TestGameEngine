#pragma once

#ifndef GEOMETRYMATERIAL_H
#define GEOMETRYMATERIAL_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QopenGlFunctions>

#include <list>

#include <ConfigurationManager.h>
#include <ShaderManager.h>


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
		};

		/// Base class for all materials for world objects. 
		class Material
		{
		public:
			/// Constructor 
			/// param ambient Ambient rgb color component of the material. Each color goes from 0.0 to 1.0
			/// param diffuse Diffuse rgb color component of the material. Each color goes from 0.0 to 1.0
			/// param specular Specular rgb color component of the material. Each color goes from 0.0 to 1.0
			/// param emissive Emissive rgb color component of the material. Each color goes from 0.0 to 1.0
			/// param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess
			Material(const QVector3D& ambient = QVector3D(0.0f, 0.0, 0.0f), const QVector3D& diffuse = QVector3D(0.0f, 0.0f, 0.0f),
				const QVector3D& specular = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D& emissive = QVector3D(0.0f, 0.0f, 0.0f), float shininess = 10.0f);

			/// Copy constructor
			/// param ref Object to be copied.
			Material(const Material& mat);

			/// Destructor
			virtual ~Material();

			const QVector3D& GetAmbient() const { return mAmbient; }
			const QVector3D& GetDiffuse() const { return mDiffuse; }
			const QVector3D& GetSpecular() const { return mSpecular; }
			const QVector3D& GetEmissive() const { return mEmissive; }
			float GetShininess() const { return mShininess; }

			void SetAmbient(const QVector3D& ambient) { mAmbient.setX(ambient.x()); mAmbient.setY(ambient.y()); mAmbient.setZ(ambient.z()); }
			void SetDiffuse(const QVector3D& diffuse) { mDiffuse.setX(diffuse.x()); mDiffuse.setY(diffuse.y()); mDiffuse.setZ(diffuse.z()); }
			void SetSpecular(const QVector3D& specular) { mSpecular.setX(specular.x()); mSpecular.setY(specular.y()); mSpecular.setZ(specular.z()); }
			void SetEmissive(const QVector3D& emissive) { mEmissive.setX(emissive.x()); mEmissive.setY(emissive.y()); mEmissive.setZ(emissive.z()); }
			void SetShininess(float shininess) { mShininess = shininess; }

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



		protected:
			QVector3D mAmbient;
			QVector3D mDiffuse;
			QVector3D mSpecular;
			QVector3D mEmissive;

			float mShininess;
			bool mLit;

			QOpenGLShaderProgram* mpProgram; // Lighting shader
			ShaderFiles::ShaderManager* mpShaderManager;
			Configuration::ConfigurationManager* mpConfInstance;

			std::string mVertexShaderKey;
			std::string mFragmentShaderKey;

			/// Initializes managers and shaders
			virtual void initMaterial();
			/// Abstract method. Sets the shaders that should be loaded
			virtual void initShaders() = 0;
			/// Loads and compiles light shader programs
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
		};
	}
}

#endif