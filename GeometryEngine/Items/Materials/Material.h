#pragma once

#ifndef GEOMETRYMATERIAL_H
#define GEOMETRYMATERIAL_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QopenGlFunctions>

#include <list>

#include <ConfigurationManager.h>
#include <ShaderManager.h>

#include "../GeometryItem.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryItem
		{
			class GeometryItem;
		}
	}

	namespace GeometryMaterial
	{
		struct MaterialConstants
		{
			static const std::string TEXTURE_MATERIAL_VERTEX_SHADER;
			static const std::string TEXTURE_MATERIAL_FRAGMENT_SHADER;
			static const std::string COLOR_MATERIAL_VERTEX_SHADER;
			static const std::string COLOR_MATERIAL_FRAGMENT_SHADER;
			static const std::string VERTEX_COLOR_MATERIAL_VERTEX_SHADER;
			static const std::string VERTEX_COLOR_MATERIAL_FRAGMENT_SHADER;
			static const std::string MULTI_TEXTURE_MATERIAL_FRAGMENT_SHADER;
		};

		class Material
		{
		public:
			Material(const QVector3D& ambient = QVector3D(0.0f, 0.0, 0.0f), const QVector3D& diffuse = QVector3D(0.0f, 0.0f, 0.0f),
				const QVector3D& specular = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D& emissive = QVector3D(0.0f, 0.0f, 0.0f), float shininess = 10.0f);

			Material(const Material& mat);

			~Material();

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

			virtual void Draw(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber, 
				const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent);
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

			virtual void initMaterial();
			virtual void initShaders() = 0;
			virtual void initProgram();
			virtual void copy(const Material& mat);

			virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent) = 0;
			virtual void drawMaterial(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) = 0;
		};
	}
}

#endif