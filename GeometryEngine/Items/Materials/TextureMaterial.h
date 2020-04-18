#pragma once

#ifndef GEOMETRYTEXMATERIAL_H
#define GEOMETRYTEXMATERIAL_H

#include <qopengltexture.h>

#include <list>
#include <Textures.h>

#include "Material.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		class TextureParameters;
		///Material that loads a color texture. 
		///Alternatively multiple textures can be loaded indicating to how many vertices it should be applied. The textures will be applied to the vertices in the order they are drawn 
		///and in the order the textures are included in the textures list.
		class TextureMaterial : public Material
		{
		public:
			/// Constructor
			/// param texDir Key of the texture to be loaded
			/// param shininess parameter. Equation: spec contribution = cos(alpha) ^ shininess
			/// param getFromConf if false texDir is treated as a path to the texture if true its considered a key for the internal texture map
			TextureMaterial(const std::string& texDir, float shininess = 10.0f, bool getFromConf = true);
			/// Constructor
			/// param textureDirs List of textures to be loaded
			/// param shininess parameter. Equation: spec contribution = cos(alpha) ^ shininess
			TextureMaterial(const std::list< TextureParameters* > & textureDirs, float shininess = 10.0f);
			/// Copy constructor
			/// param ref Object to be copied.
			TextureMaterial(const TextureMaterial& mat);
			///Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual Material* Clone() const override;

			/// Destructor
			virtual ~TextureMaterial();
			///Adds a texture to the material list
			virtual void AddTexture(const TextureParameters& texDir);
			///Adds multiple textures to the materials list
			virtual void AddTextures(std::list< TextureParameters*> textureDirs);
			///Adds a texture in a certain position in the material list
			virtual void InsertTexture(const TextureParameters& texDir, int index);
			///Deletes a texture in a certain position in the material list 
			virtual void DeleteTexture(int index);
			/// Deletes all textures from the material
			virtual void DeleteAllTextures();

		protected:
			/// Texture unit that the textures in the material will be binded to
			static const int TEXTURE_UNIT = 0;

			std::list<TextureParameters *> mTexturesList;

			TexturesFiles::Textures * mpTexDirManager;

			///Empty constructor
			///Called from child objects copy constructor to avoid double initialization 
			TextureMaterial() {}

			/// Copies the list of textures
			/// param textureDirs list of pointers to texture data to be copied
			virtual void initTextures(const std::list<TextureParameters* > & textureDirs);
			/// Inits the material and the textures and gets/initializes an instance of the texture manager
			/// param textureDirs list of pointers to texture data to be copied
 			virtual void initMaterial(const std::list<TextureParameters* >& textureDirs);
			/// Sets the keys for the shaders to be used
			virtual void initShaders() override;
			/// Builds the textures
			/// param textureDirs list of material textures to be built
			virtual void buildTextures(const std::list<TextureParameters* >& textureDirs);

			/// Sends parameters to the shaders.
			/// param projection Projection matrix
			/// param view View matrix			 
			/// param parent geometry item
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
			virtual void copy(const TextureMaterial& mat);
		};
	}
}

#endif