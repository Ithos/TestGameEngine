#pragma once

#ifndef MULTITEXTUREMATERIAL_H
#define MULTITEXTUREMATERIAL_H

#include <list>
#include <qopengltexture.h>
#include "Material.h"

namespace TexturesFiles
{
	class Textures;
}

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		class TextureParameters;
		/// Material tha uses different textures for the different components of the light (ambient, diffuse, specular, emissive). Used to create complex light effects for certain materials like skin.
		class MultiTextureMaterial : public Material
		{
		public:
			///Constructor
			/// param ambientTexDir Key to a texture that will be used as ambient color
			/// param diffuseTexDir Key to a texture that will be used as diffuse color
			/// param specularTexDir Key to a texture that will be used as specular color
			/// param emissiveTexDir Key to a texture that will be used as emissive color
			/// param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders.
			MultiTextureMaterial(const std::string& ambientTexDir, const std::string& diffuseTexDir, const std::string& specularTexDir, const std::string& emissiveTexDir, float shininess = 10.0f);
			///Destructor
			virtual ~MultiTextureMaterial();
			/// Copy constructor
			/// param ref Object to be copied.
			MultiTextureMaterial(const MultiTextureMaterial& mat);
			/// Sets the ambient texture
			/// param ambientTexDir Key to a texture that will be used as ambient color 
			void SetAmbientTexture(const std::string& ambientTexDir);
			/// Sets the diffuse texture
			/// param diffuseTexDir Key to a texture that will be used as diffuse color 
			void SetDiffuseTexture(const std::string& diffuseTexDir);
			/// Sets the specular texture
			/// param specularTexDir Key to a texture that will be used as specular color 
			void SetSpecularTexture(const std::string& specularTexDir);
			/// Sets the emissive texture
			/// param emissiveTexDir Key to a texture that will be used as emissive color 
			void SetEmissiveTexture(const std::string& emissiveTexDir);
			/// Copy constructor
			/// param ref Object to be copied.
			virtual Material* Clone() const override { return new MultiTextureMaterial((*this)); };

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
			MultiTextureMaterial() {};

			/// Calls parent initMaterial and dets a pointer to the material manager singelton
			virtual void initMaterial() override;
			/// Sets the keys of the shaders to be used
			virtual void initShaders() override;
			/// Builds all the textures
			virtual void initTextures();

			/// Sends parameters to the shaders.
			/// param projection Projection matrix
			/// param view View matrix	
			/// param parent geometry item to be drawn
			virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent) override;
			/// Binds shaders and draws.
			/// param vertexBuf Vertex buffer
			/// param indexBuf IndexBuffer
			/// param totalVertexNum Number of vetices
			/// param totalIndexNumber Number of indices
			virtual void drawMaterial(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) override;
			/// Binds textures to specific texture units. Used before drawing the object
			virtual void bindTextures();
			/// Copies the data of a Material object to the current object
			/// param ref Material to be copied
			virtual void copy(const MultiTextureMaterial& mat);
		};
	}
}

#endif
