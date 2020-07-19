#pragma once

#ifndef GEOMETRYALPHATEXTUREMATERIAL_H
#define GEOMETRYALPHATEXTUREMATERIAL_H

#include "../TransparentMaterial.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		///Material that loads a color texture and admits transparencies
		class AlphaTextureMaterial : public TransparentMaterial
		{
		public:
			/// Constructor. Calls init material
			/// param customShading Interface to an object that implements custom shadow shading for this material.
			/// param texDir Key of the texture to be loaded
			/// param thresholdValue Min alpha value below which the fragment is dropped completely. This value is clamped to the range [0, 1]
			/// param globalAlphaValue Main alpha value for the whole object, clamps to 0.0 to 1.0 range. This value gets multiplied to every fragment color alpha.
			/// param shininess Shininess component. Has to be > 0 if invalid takes the value 0.0001. Equation: spec contribution = cos(alpha) ^ shininessIf shininess is <= 0 it is set to 0.001 to avoid errors in the shaders.
			AlphaTextureMaterial(const CustomShading::CustomShadingInterface* const customShading, const std::string & texDir, float thresholdValue = 0.0f, float globalAlphaValue = 1.0f, float shininess = 10.0f, bool translucent = false);
			/// Copy constructor
			/// param ref Object to be copied.
			AlphaTextureMaterial(const AlphaTextureMaterial& ref);
			/// Destructor
			virtual ~AlphaTextureMaterial();
			///Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual AlphaTextureMaterial* Clone() const override { return new AlphaTextureMaterial((*this)); }
			/// Sets the emissive texture
			/// param emissiveTexDir Key to a texture that will be used as emissive color 
			void SetMaterialTexture(const std::string& texDir);
			/// Returns the initial texture unit for this material
			int GetInitialTextureUnit() { return TEXTURE_UNIT; }
			/// Binds textures to specific texture units. Used before drawing the object
			virtual void BindTextures();

		protected:
			/// Texture unit that the textures in the material will be binded to
			static const int TEXTURE_UNIT = 0;
			TextureParameters* mpTexture;
			TexturesFiles::Textures * mpTexDirManager;

			///Empty constructor
			///Called from child objects copy constructor to avoid double initialization 
			AlphaTextureMaterial() {}

			/// Inits the material and the textures and gets/initializes an instance of the texture manager
			virtual void initMaterial();
			/// Builds all the textures
			virtual void initTextures();
			/// Sets the shaders that should be loaded
			virtual void initShaders();
			/// Sends parameters to the shaders.
			/// param projection Projection matrix
			/// param view View matrix			 
			/// param parent geometry item
			virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent) override;
			/// Binds shaders and draws.
			/// param vertexBuf Vertex buffer
			/// param indexBuf IndexBuffer
			/// param totalVertexNum Number of vetices
			/// param titalIndexNum Number of indices
			virtual void drawMaterial(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) override;
			/// Copies the data of a Material object to the current object
			/// param ref Material to be copied
			virtual void copy(const AlphaTextureMaterial& mat);


		};
	}
}
#endif