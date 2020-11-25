#pragma once

#ifndef GEOMETRYALPHANORMALTEXTUREMATERIAL_H
#define GEOMETRYALPHANORMALTEXTUREMATERIAL_H

#include "AlphaTextureMaterial.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		///Material that loads a color texture that may contain transparencies and a normal map texture. 
		class AlphaNormalMapTextureMaterial : public AlphaTextureMaterial
		{
		public:
			/// Constructor. Calls init material
			/// param customShading Interface to an object that implements custom shadow shading for this material.
			/// param texDir Key of the texture to be loaded
			/// param mormalMapDir Key of the normal map texture to be used
			/// param thresholdValue Min alpha value below which the fragment is dropped completely. This value is clamped to the range [0, 1]
			/// param globalAlphaValue Main alpha value for the whole object, clamps to 0.0 to 1.0 range. This value gets multiplied to every fragment color alpha.
			/// param shininess Shininess component. Has to be > 0 if invalid takes the value 0.0001. Equation: spec contribution = cos(alpha) ^ shininessIf shininess is <= 0 it is set to 0.001 to avoid errors in the shaders.
			AlphaNormalMapTextureMaterial(const CustomShading::CustomShadingInterface* const customShading, const std::string & texDir, const std::string & normalMapTexDir, float thresholdValue = 0.0f, 
				float globalAlphaValue = 1.0f, float shininess = 10.0f,
				bool translucent = false);
			/// Copy constructor
			/// param ref Object to be copied.
			AlphaNormalMapTextureMaterial(const AlphaNormalMapTextureMaterial& ref);
			/// Destructor
			virtual ~AlphaNormalMapTextureMaterial();
			///Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual AlphaNormalMapTextureMaterial* Clone() const override { return new AlphaNormalMapTextureMaterial((*this)); }
			/// Sets the emissive texture
			/// param emissiveTexDir Key to a texture that will be used as emissive color 
			void SetNormalMapTexture(const std::string& normalMapTexDir);
			/// Binds textures to specific texture units. Used before drawing the object
			virtual void BindTextures() override;
			/// Unbinds textures.
			virtual void UnbindTextures() override;
		protected:
			TextureParameters* mpNormalMapTexture;
			/// Builds all the textures
			virtual void initTextures() override;
			/// Sets the shaders that should be loaded
			virtual void initShaders();
			/// Sends parameters to the shaders.
			/// param projection Projection matrix
			/// param view View matrix			 
			/// param parent geometry item
			virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent) override;
			/// Copies the data of a Material object to the current object
			/// param ref Material to be copied
			virtual void copy(const AlphaNormalMapTextureMaterial& mat);


		};
	}
}
#endif