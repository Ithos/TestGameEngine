#include <Textures.h>
#include "TextureMaterialCommons.h"
#include "../GeometryItem.h"
#include "MultiTextureMaterial.h"

GeometryEngine::GeometryMaterial::MultiTextureMaterial::MultiTextureMaterial(const std::string & ambientTexDir, const std::string & diffuseTexDir, 
	const std::string & specularTexDir, const std::string & emissiveTexDir, float shininess) : 
	Material(shininess), mpTexDirManager(nullptr), mpAmbientTexture(nullptr),
	mpDiffuseTexture(nullptr), mpSpecularTexture(nullptr), mpEmissiveTexture(nullptr)
{
	mpAmbientTexture = new TextureParameters(ambientTexDir, -1, true);
	mpDiffuseTexture = new TextureParameters(diffuseTexDir, -1, true);
	mpSpecularTexture = new TextureParameters(specularTexDir, -1, true);
	mpEmissiveTexture = new TextureParameters(emissiveTexDir, -1, true);
	initMaterial();
}

GeometryEngine::GeometryMaterial::MultiTextureMaterial::~MultiTextureMaterial()
{
	if (mpAmbientTexture != nullptr)
	{
		delete mpAmbientTexture;
	}

	if (mpDiffuseTexture != nullptr)
	{
		delete mpDiffuseTexture;
	}

	if (mpSpecularTexture != nullptr)
	{
		delete mpSpecularTexture;
	}

	if (mpEmissiveTexture != nullptr)
	{
		delete mpEmissiveTexture;
	}
}

GeometryEngine::GeometryMaterial::MultiTextureMaterial::MultiTextureMaterial(const MultiTextureMaterial & mat)
{
	copy(mat);
	initTextures();
}

void GeometryEngine::GeometryMaterial::MultiTextureMaterial::SetAmbientTexture(const std::string & ambientTexDir)
{
	if (mpAmbientTexture != nullptr)
	{
		delete mpAmbientTexture;
	}

	mpAmbientTexture = new TextureParameters(ambientTexDir, -1);
}

void GeometryEngine::GeometryMaterial::MultiTextureMaterial::SetDiffuseTexture(const std::string & diffuseTexDir)
{
	if (mpDiffuseTexture != nullptr)
	{
		delete mpDiffuseTexture;
	}

	mpDiffuseTexture = new TextureParameters(diffuseTexDir, -1);
}

void GeometryEngine::GeometryMaterial::MultiTextureMaterial::SetSpecularTexture(const std::string & specularTexDir)
{
	if (mpSpecularTexture != nullptr)
	{
		delete mpSpecularTexture;
	}

	mpSpecularTexture = new TextureParameters(specularTexDir, -1);
}

void GeometryEngine::GeometryMaterial::MultiTextureMaterial::SetEmissiveTexture(const std::string & emissiveTexDir)
{
	if (mpEmissiveTexture != nullptr)
	{
		delete mpEmissiveTexture;
	}

	mpEmissiveTexture = new TextureParameters(emissiveTexDir, -1);
}

void GeometryEngine::GeometryMaterial::MultiTextureMaterial::initMaterial()
{
	Material::initMaterial();
	mpTexDirManager = TexturesFiles::Textures::InitInstance(mpConfInstance->getTexturesFolder(), mpConfInstance->getTexturesConfig());
}

void GeometryEngine::GeometryMaterial::MultiTextureMaterial::initTextures()
{
	if (mpAmbientTexture->Texture == nullptr)
		mpAmbientTexture->Build();
	if (mpDiffuseTexture->Texture == nullptr)
		mpDiffuseTexture->Build();
	if (mpSpecularTexture->Texture == nullptr)
		mpSpecularTexture->Build();
	if (mpEmissiveTexture->Texture == nullptr)
		mpEmissiveTexture->Build();
}

void GeometryEngine::GeometryMaterial::MultiTextureMaterial::initShaders()
{
	mVertexShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::TEXTURE_MATERIAL_VERTEX_SHADER;
	mFragmentShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::MULTI_TEXTURE_MATERIAL_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryMaterial::MultiTextureMaterial::setProgramParameters(const QMatrix4x4 & projection, const QMatrix4x4 & view, const GeometryWorldItem::GeometryItem::GeometryItem & parent)
{
	assert(mpProgram != nullptr && "Multi Texture Material --> Shader Program Null");
	if (mpProgram != nullptr)
	{
		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", projection * view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelViewMatrix", view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelMatrix", parent.GetModelMatrix());
		mpProgram->setUniformValue("mShininess", mShininess);

		mpProgram->setUniformValue("textureAmbient", TEXTURE_UNIT);
		mpProgram->setUniformValue("textureDiffuse", TEXTURE_UNIT + 1);
		mpProgram->setUniformValue("textureReflective", TEXTURE_UNIT + 2);
		mpProgram->setUniformValue("textureEmissive", TEXTURE_UNIT + 3);
	}
}

void GeometryEngine::GeometryMaterial::MultiTextureMaterial::drawMaterial(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber)
{
	assert(mpProgram != nullptr && "Multi Texture Material --> Shader Program Null");
	if (mpProgram != nullptr)
	{
		// Tell OpenGL which VBOs to use
		vertexBuf->bind();
		indexBuf->bind();

		// Tell OpenGL programmable pipeline how to locate vertex position data
		int vertexLocation = mpProgram->attributeLocation("posAttr");
		mpProgram->enableAttributeArray(vertexLocation);
		mpProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

		// Tell OpenGL programmable pipeline how to locate texture coordinates
		int textureCoordinate = mpProgram->attributeLocation("TexCoord");
		mpProgram->enableAttributeArray(textureCoordinate);
		mpProgram->setAttributeBuffer(textureCoordinate, GL_FLOAT, VertexData::TEXTURE_COORDINATES_OFFSET, 2, sizeof(VertexData));

		// Tell OpenGL programmable pipeline how to locate normals
		int normalVector = mpProgram->attributeLocation("aNormal");
		mpProgram->enableAttributeArray(normalVector);
		mpProgram->setAttributeBuffer(normalVector, GL_FLOAT, VertexData::NORMALS_OFFSET, 3, sizeof(VertexData));

		bindTextures();

		glDrawElements(GL_TRIANGLE_STRIP, totalIndexNumber, GL_UNSIGNED_SHORT, 0);

		unbindTextures();

		mpProgram->disableAttributeArray(vertexLocation);
		mpProgram->disableAttributeArray(textureCoordinate);
		mpProgram->disableAttributeArray(normalVector);

		vertexBuf->release();
		indexBuf->release();
		
	}
}

void GeometryEngine::GeometryMaterial::MultiTextureMaterial::bindTextures()
{
	if (mpAmbientTexture->Texture != nullptr) mpAmbientTexture->Texture->bind(TEXTURE_UNIT);
	if (mpDiffuseTexture->Texture != nullptr) mpDiffuseTexture->Texture->bind(TEXTURE_UNIT + 1);
	if (mpSpecularTexture->Texture != nullptr) mpSpecularTexture->Texture->bind(TEXTURE_UNIT + 2);
	if (mpEmissiveTexture->Texture != nullptr) mpEmissiveTexture->Texture->bind(TEXTURE_UNIT + 3);
}

void GeometryEngine::GeometryMaterial::MultiTextureMaterial::unbindTextures()
{
	if (mpAmbientTexture->Texture != nullptr) mpAmbientTexture->Texture->release();
	if (mpDiffuseTexture->Texture != nullptr) mpDiffuseTexture->Texture->release();
	if (mpSpecularTexture->Texture != nullptr) mpSpecularTexture->Texture->release();
	if (mpEmissiveTexture->Texture != nullptr) mpEmissiveTexture->Texture->release();
}

void GeometryEngine::GeometryMaterial::MultiTextureMaterial::copy(const MultiTextureMaterial & mat)
{
	mpAmbientTexture = new TextureParameters((*mat.mpAmbientTexture));
	mpDiffuseTexture = new TextureParameters((*mat.mpDiffuseTexture));
	mpSpecularTexture = new TextureParameters((*mat.mpSpecularTexture));
	mpEmissiveTexture = new TextureParameters((*mat.mpEmissiveTexture));
	initMaterial();
}