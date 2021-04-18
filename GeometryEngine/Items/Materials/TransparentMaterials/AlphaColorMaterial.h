#pragma once

#ifndef GEOMETRYALPHACOLORMATERIAL_H
#define GEOMETRYALPHACOLORMATERIAL_H

#include "../TransparentMaterial.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		/// Material that defines color by its ambient, diffuse specular and emissive components. Also defines a global alpha value to stablish the transparency of the material.
		class AlphaColorMaterial : public TransparentMaterial
		{
		public:
			/// Constructor. Calls init material
			/// \param customShading Interface to an object that implements custom shadow shading for this material
			/// \param ambient Ambient rgb color component of the material. Each color goes from 0.0 to 1.0
			/// \param diffuse Diffuse rgb color component of the material. Each color goes from 0.0 to 1.0
			/// \param specular Specular rgb color component of the material. Each color goes from 0.0 to 1.0
			/// \param emissive Emissive rgb color component of the material. Each color goes from 0.0 to 1.0
			/// \param globalAlphaValue Main alpha value for the whole object, clamps to 0.0 to 1.0 range. This value gets multiplied to every fragment color alpha.
			/// \param shininess Shininess component. Has to be > 0 if invalid takes the value 0.0001. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders.
			/// \param translucent Indicates whether the material casts a translucent shadow or not
			AlphaColorMaterial(const CustomShading::CustomShadingInterface* const customShading, const QVector3D& ambient = QVector3D(0.0f, 0.0, 0.0f), 
				const QVector3D& diffuse = QVector3D(0.0f, 0.0f, 0.0f),
				const QVector3D& specular = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D& emissive = QVector3D(0.0f, 0.0f, 0.0f), float thresholdValue = 0.0f,
				float globalAlphaValue = 1.0f, float shininess = 10.0f, bool translucent = false);
			/// Copy constructor
			/// \param ref Object to be copied.
			AlphaColorMaterial(const AlphaColorMaterial& ref);
			/// Destructor
			virtual ~AlphaColorMaterial();
			///Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual AlphaColorMaterial* Clone() const override { return new AlphaColorMaterial((*this)); }

			const QVector3D& GetAmbient() const { return mAmbient; }
			const QVector3D& GetDiffuse() const { return mDiffuse; }
			const QVector3D& GetSpecular() const { return mSpecular; }
			const QVector3D& GetEmissive() const { return mEmissive; }

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
			virtual void copy(const AlphaColorMaterial& mat);
		};
	}
}
#endif