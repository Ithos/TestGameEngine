#pragma once

#ifndef GEOMETRYSKYBOXMATERIAL_H
#define GEOMETRYSKYBOXMATERIAL_H

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

		class SkyboxMaterial : public Material
		{
		public:
			///Constructor
			/// param textureDir key to the texture of the skybox. It will be considered as an emissive texture to avoid illumination.
			SkyboxMaterial(const std::string& textureDir);
			///Destructor
			virtual ~SkyboxMaterial();
			/// Copy constructor
			/// param ref Object to be copied.
			SkyboxMaterial(const SkyboxMaterial& mat);
			/// Sets the texture of the skybox. It is treated as an emmissive texture.
			/// param textureDir Key to a texture that will be used as emmissive color 
			void SetTexture(const std::string& textureDir);
			/// Copy constructor
			/// param ref Object to be copied.
			virtual Material* Clone() const override { return new SkyboxMaterial((*this)); };

		protected:
			/// Texture unit to be used by the textures
			static const int TEXTURE_UNIT = 0;

			TextureParameters* mpAmbientTexture;
			TextureParameters* mpDiffuseTexture;
			TextureParameters* mpSpecularTexture;
			TextureParameters* mpSkyboxTexture;
			TexturesFiles::Textures * mpTexDirManager;

			QMatrix4x4 mIdentityMatrix;

			///Empty constructor
			///Called from child objects copy constructor to avoid double initialization 
			SkyboxMaterial() {};

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
			virtual void copy(const SkyboxMaterial& mat);
		};
	}
}

#endif