#pragma once

#ifndef MULTITEXTUREMATERIAL_H
#define MULTITEXTUREMATERIAL_H

#include <list>
#include <qopengltexture.h>
#include <Textures.h>
#include "TextureMaterialCommons.h"
#include "Material.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{

		class MultiTextureMaterial : public Material
		{
		public:
			MultiTextureMaterial(const std::string& ambientTexDir, const std::string& diffuseTexDir, const std::string& specularTexDir, const std::string& emissiveTexDir, float shininess = 10.0f);
			virtual ~MultiTextureMaterial();
			MultiTextureMaterial(const MultiTextureMaterial& mat);
			void SetAmbientTexture(const std::string& ambientTexDir);
			void SetDiffuseTexture(const std::string& diffuseTexDir);
			void SetSpecularTexture(const std::string& specularTexDir);
			void SetEmissiveTexture(const std::string& emissiveTexDir);
			virtual Material* Clone() const override { return new MultiTextureMaterial((*this)); };

		protected:
			static const int TEXTURE_UNIT = 0;

			TextureParameters* mpAmbientTexture;
			TextureParameters* mpDiffuseTexture;
			TextureParameters* mpSpecularTexture;
			TextureParameters* mpEmissiveTexture;
			TexturesFiles::Textures * mpTexDirManager;

			virtual void initMaterial() override;
			virtual void initShaders() override;
			virtual void initTextures();

			virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent) override;
			virtual void drawMaterial(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) override;
			virtual void copy(const MultiTextureMaterial& mat);
		};
	}
}

#endif
