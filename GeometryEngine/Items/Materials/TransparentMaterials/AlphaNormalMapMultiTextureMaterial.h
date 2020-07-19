#pragma once

#ifndef GEOMETRYALPHANORMALMULTIMATERIAL_H
#define GEOMETRYALPHANORMALMULTIMATERIAL_H

#include "AlphaMultiTextureMaterial.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		/// Material tha uses different textures that may contain transparencies for the different components of the light (ambient, diffuse, specular, emissive) and also stores a normal map texture.
		class AlphaNormalMapMultiTextureMaterial : public AlphaMultiTextureMaterial
		{
		public:
			/// Constructor. Calls init material
			/// param customShading Interface to an object that implements custom shadow shading for this material
			/// param ambientTexDir Key to a texture that will be used as ambient color
			/// param diffuseTexDir Key to a texture that will be used as diffuse color
			/// param specularTexDir Key to a texture that will be used as specular color
			/// param emissiveTexDir Key to a texture that will be used as emissive color
			/// param normalMapTexDir Key to a texture that contains the object normal map
			/// param thresholdValue Min alpha value below which the fragment is dropped completely. This value is clamped to the range [0, 1]
			/// param globalAlphaValue Main alpha value for the whole object, clamps to 0.0 to 1.0 range. This value gets multiplied to every fragment color alpha.
			/// param shininess Shininess component. Has to be > 0 if invalid takes the value 0.0001. Equation: spec contribution = cos(alpha) ^ shininessIf shininess is <= 0 it is set to 0.001 to avoid errors in the shaders.
			AlphaNormalMapMultiTextureMaterial(const CustomShading::CustomShadingInterface* const customShading, const std::string & ambientTexDir, const std::string & diffuseTexDir,
				const std::string & specularTexDir, const std::string & emissiveTexDir, const std::string & normalMapTexDir, float thresholdValue = 0.0f,
				float globalAlphaValue = 1.0f, float shininess = 10.0f, bool translucent = false);
			/// Copy constructor
			/// param ref Object to be copied.
			AlphaNormalMapMultiTextureMaterial(const AlphaNormalMapMultiTextureMaterial& ref);
			/// Destructor
			virtual ~AlphaNormalMapMultiTextureMaterial();
			///Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual AlphaNormalMapMultiTextureMaterial* Clone() const override { return new AlphaNormalMapMultiTextureMaterial((*this)); }
			/// Sets the emissive texture
			/// param emissiveTexDir Key to a texture that will be used as emissive color 
			void SetNormalMapTexture(const std::string& normalMapTexDir);
			/// Binds textures to specific texture units. Used before drawing the object
			virtual void BindTextures();

		protected:
			TextureParameters* mpNormalMapTexture;

			/// Builds all the textures
			virtual void initTextures();
			/// Sets the shaders that should be loaded
			virtual void initShaders();
			/// Sends parameters to the shaders.
			/// param projection Projection matrix
			/// param view View matrix			 
			/// param parent geometry item
			virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent) override;
			/// Copies the data of a Material object to the current object
			/// param ref Material to be copied
			virtual void copy(const AlphaNormalMapMultiTextureMaterial& mat);


		};
	}
}
#endif