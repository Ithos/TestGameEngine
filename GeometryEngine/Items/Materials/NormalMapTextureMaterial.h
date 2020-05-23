#pragma once

#ifndef GEOMETRYNORMALMAPMATERIAL_H
#define GEOMETRYNORMALMAPMATERIAL_H

#include "TextureMaterial.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		///Material that loads a color texture and a normal map texture. 
		///Normal map textures are used to simulate small protrusions and holes in objects by modifying the light reflection.
		class NormalMapTextureMaterial : public TextureMaterial
		{
		public:
			/// Constructor
			/// param texDir Key of the texture to be loaded
			/// param mormalMapDir Key of the normal map texture to be used
			/// param shininess parameter. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders.
			NormalMapTextureMaterial(const std::string& texDir, const std::string& mormalMapDir,  float shininess = 10.0f);
			/// Copy constructor
			/// param ref Object to be copied.
			NormalMapTextureMaterial(const NormalMapTextureMaterial& mat);
			///Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual Material* Clone() const override;

			/// Destructor
			virtual ~NormalMapTextureMaterial();


		protected:
			/// Builds the textures
			/// param textureDirs list of material textures to be built
			virtual void buildTextures(const std::list<TextureParameters* >& textureDirs);

		private:
			TextureParameters* mpNormalMapTexture;
			/// Sets the keys for the shaders to be used
			virtual void initShaders() override;

			/// Sends parameters to the shaders.
			/// param projection Projection matrix
			/// param view View matrix			 
			/// param parent geometry item
			virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent) override;
			/// Binds textures to specific texture units. Used before drawing the object
			virtual void bindTextures() override;
			/// Copies the data of a Material object to the current object
			/// param ref Material to be copied
			virtual void copy(const NormalMapTextureMaterial& mat);
		};
	}
}

#endif