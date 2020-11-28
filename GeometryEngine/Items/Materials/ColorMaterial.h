#pragma once

#ifndef GEOMETRYCOLORMATERIAL_H
#define GEOMETRYCOLORMATERIAL_H

#include "Material.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		/// Material that defines color by its ambient, diffuse specular and emissive components.
		class ColorMaterial : public Material
		{
		public:
			/// Constructor. Calls init material
			/// Constructor 
			/// \param ambient Ambient rgb color component of the material. Each color goes from 0.0 to 1.0
			/// \param diffuse Diffuse rgb color component of the material. Each color goes from 0.0 to 1.0
			/// \param specular Specular rgb color component of the material. Each color goes from 0.0 to 1.0
			/// \param emissive Emissive rgb color component of the material. Each color goes from 0.0 to 1.0
			/// \param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders. 
			ColorMaterial(const QVector3D& ambient = QVector3D(0.0f, 0.0, 0.0f), const QVector3D& diffuse = QVector3D(0.0f, 0.0f, 0.0f), 
				const QVector3D& specular = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D& emissive = QVector3D(0.0f, 0.0f, 0.0f), float shininess = 10.0f);
			/// Copy constructor
			/// \param ref Object to be copied.
			ColorMaterial(const ColorMaterial& mat);
			/// Destructor
			virtual ~ColorMaterial();
			///Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual Material* Clone() const override;

			const QVector3D& GetAmbient() const { return mAmbient; }
			const QVector3D& GetDiffuse() const { return mDiffuse; }
			const QVector3D& GetSpecular() const { return mSpecular; }
			const QVector3D& GetEmissive() const { return mEmissive; }
			float GetShininess() const { return mShininess; }

			void SetAmbient(const QVector3D& ambient) { mAmbient.setX(ambient.x()); mAmbient.setY(ambient.y()); mAmbient.setZ(ambient.z()); }
			void SetDiffuse(const QVector3D& diffuse) { mDiffuse.setX(diffuse.x()); mDiffuse.setY(diffuse.y()); mDiffuse.setZ(diffuse.z()); }
			void SetSpecular(const QVector3D& specular) { mSpecular.setX(specular.x()); mSpecular.setY(specular.y()); mSpecular.setZ(specular.z()); }
			void SetEmissive(const QVector3D& emissive) { mEmissive.setX(emissive.x()); mEmissive.setY(emissive.y()); mEmissive.setZ(emissive.z()); }

		protected:
			QVector3D mAmbient;
			QVector3D mDiffuse;
			QVector3D mSpecular;
			QVector3D mEmissive;

			/// Sets the shaders that should be loaded
			virtual void initShaders();
			/// Sends parameters to the shaders.
			/// \param projection Projection matrix
			/// \param view View matrix			 
			/// \param parent geometry item
			virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent) override;
			/// Binds shaders and draws.
			/// \param vertexBuf Vertex buffer
			/// \param indexBuf IndexBuffer
			/// \param totalVertexNum Number of vetices
			/// \param titalIndexNum Number of indices
			virtual void drawMaterial(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) override;
			/// Copies the data of a Material object to the current object
			/// \param ref Material to be copied
			virtual void copy(const ColorMaterial& mat);
		};
	}
}
#endif