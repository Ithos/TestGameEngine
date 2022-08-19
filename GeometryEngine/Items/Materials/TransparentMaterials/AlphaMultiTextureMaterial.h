#pragma once

#ifndef GEOMETRYALPHAMULTITEXTUREMATERIAL_H
#define GEOMETRYALPHAMULTITEXTUREMATERIAL_H

#include "../TransparentMaterial.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		// Material tha uses different textures that may contain transparencies for the different components of the light (ambient, diffuse, specular, emissive). 
		class AlphaMultiTextureMaterial : public TransparentMaterial
		{
		public:
			/// Constructor. Calls init material
			/// \param multiShadingInterface Interface to an object that implements custom shading for this material
			/// \param ambientTexDir Key to a texture that will be used as ambient color
			/// \param diffuseTexDir Key to a texture that will be used as diffuse color
			/// \param specularTexDir Key to a texture that will be used as specular color
			/// \param emissiveTexDir Key to a texture that will be used as emissive color
			/// \param thresholdValue Min alpha value below which the fragment is dropped completely. This value is clamped to the range [0, 1]
			/// \param globalAlphaValue Main alpha value for the whole object, clamps to 0.0 to 1.0 range. This value gets multiplied to every fragment color alpha.
			/// \param shininess Shininess component. Has to be > 0 if invalid takes the value 0.0001. Equation: spec contribution = cos(alpha) ^ shininessIf shininess is <= 0 it is set to 0.001 to avoid errors in the shaders.
			/// \param translucent Indicates whether the material casts a translucent shadow or not
			AlphaMultiTextureMaterial(const CustomShading::MultiShadingInterface* const multiShadingInterface, const std::string & ambientTexDir, const std::string & diffuseTexDir,
				const std::string & specularTexDir, const std::string & emissiveTexDir,
				float thresholdValue = 0.0f, float globalAlphaValue = 1.0f, float shininess = 10.0f, bool translucent = false);
			/// Copy constructor
			/// \param ref Object to be copied.
			AlphaMultiTextureMaterial(const AlphaMultiTextureMaterial& ref);
			/// Destructor
			virtual ~AlphaMultiTextureMaterial();
			///Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual AlphaMultiTextureMaterial* Clone() const override { return new AlphaMultiTextureMaterial((*this)); }
			/// Sets the ambient texture
			/// \param ambientTexDir Key to a texture that will be used as ambient color 
			void SetAmbientTexture(const std::string& ambientTexDir);
			/// Sets the diffuse texture
			/// \param diffuseTexDir Key to a texture that will be used as diffuse color 
			void SetDiffuseTexture(const std::string& diffuseTexDir);
			/// Sets the specular texture
			/// \param specularTexDir Key to a texture that will be used as specular color 
			void SetSpecularTexture(const std::string& specularTexDir);
			/// Sets the emissive texture
			/// \param emissiveTexDir Key to a texture that will be used as emissive color 
			void SetEmissiveTexture(const std::string& emissiveTexDir);
			/// Binds textures to specific texture units. Used before drawing the object
			virtual void BindTextures();
			/// Unbinds textures.
			virtual void UnbindTextures();
			/// Returns the initial texture unit for this material
			int GetInitialTextureUnit() { return TEXTURE_UNIT; }

		protected:
			/// Texture unit to be used by the textures
			static const int TEXTURE_UNIT = 0;

			TextureParameters* mpAmbientTexture;
			TextureParameters* mpDiffuseTexture;
			TextureParameters* mpSpecularTexture;
			TextureParameters* mpEmissiveTexture;
			TexturesFiles::Textures * mpTexDirManager;

			///Empty constructor
			///Called from child objects copy constructor to avoid double initialization 
			AlphaMultiTextureMaterial() {};

			/// Calls parent initMaterial and dets a pointer to the material manager singelton
			virtual void initMaterial() override;
			/// Builds all the textures
			virtual void initTextures();
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
			virtual void copy(const AlphaMultiTextureMaterial& mat);


		};
	}
}
#endif