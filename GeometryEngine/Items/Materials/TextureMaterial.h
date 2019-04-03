#pragma once

#ifndef GEOMETRYTEXMATERIAL_H
#define GEOMETRYTEXMATERIAL_H

#include <qopengltexture.h>

#include <list>
#include <Textures.h>

#include "TextureMaterialCommons.h"
#include "Material.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		class TextureMaterial : public Material
		{
		public:
			TextureMaterial(const std::string& texDir, float shininess = 10.0f);

			TextureMaterial(const std::list< TextureParameters* > & textureDirs, float shininess = 10.0f);

			TextureMaterial(const TextureMaterial& mat);

			virtual Material* Clone() const override;

			virtual ~TextureMaterial();
			virtual void AddTexture(const TextureParameters& texDir);
			virtual void AddTextures(std::list< TextureParameters*> textureDirs);
			virtual void InsertTexture(const TextureParameters& texDir, int index);
			virtual void DeleteTexture(int index);
			virtual void DeleteAllTextures();

		protected:
			static const int TEXTURE_UNIT = 0;

			std::list<TextureParameters *> mTexturesList;

			TexturesFiles::Textures * mpTexDirManager;

			virtual void initTextures(const std::list<TextureParameters* > & textureDirs);
			virtual void initMaterial(const std::list<TextureParameters* >& textureDirs);
			virtual void initShaders() override;
			virtual void buildTextures(const std::list<TextureParameters* >& textureDirs);

			virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent) override;
			virtual void drawMaterial(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) override;
			virtual void copy(const TextureMaterial& mat);
		};
	}
}

#endif