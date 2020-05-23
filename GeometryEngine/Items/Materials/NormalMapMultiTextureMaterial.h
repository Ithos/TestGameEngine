#pragma once

#ifndef GEOMETRYNORMALMAPMULTI_H
#define GEOMETRYNORMALMAPMULTI_H

#include "MultiTextureMaterial.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		/// Material tha uses different textures for the different components of the light (ambient, diffuse, specular, emissive) and also stores a normal map texture.
		class NormalMapMultiTextureMaterial : public MultiTextureMaterial
		{
		public:
			///Constructor
			/// param ambientTexDir Key to a texture that will be used as ambient color
			/// param diffuseTexDir Key to a texture that will be used as diffuse color
			/// param specularTexDir Key to a texture that will be used as specular color
			/// param emissiveTexDir Key to a texture that will be used as emissive color
			/// param normalMapTexDir Key to a texture that contains the object normal map
			/// param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders.
			NormalMapMultiTextureMaterial(const std::string& ambientTexDir, const std::string& diffuseTexDir, const std::string& specularTexDir, 
				const std::string& emissiveTexDir, const std::string& normalMapTexDir, float shininess = 10.0f);
			/// Copy constructor
			/// param ref Object to be copied.
			NormalMapMultiTextureMaterial(const NormalMapMultiTextureMaterial& mat);
			///Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual Material* Clone() const override;

			/// Destructor
			virtual ~NormalMapMultiTextureMaterial();

		protected:
			/// Builds all the textures
			virtual void initTextures() override;

		private:
			TextureParameters* mpNormalMapTexture;

			/// Sets the keys of the shaders to be used
			virtual void initShaders() override;

			/// Sends parameters to the shaders.
			/// param projection Projection matrix
			/// param view View matrix	
			/// param parent geometry item to be drawn
			virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent) override;
			/// Binds textures to specific texture units. Used before drawing the object
			virtual void bindTextures() override;
			/// Copies the data of a Material object to the current object
			/// param ref Material to be copied
			virtual void copy(const NormalMapMultiTextureMaterial& mat);
		};
	}
}

#endif