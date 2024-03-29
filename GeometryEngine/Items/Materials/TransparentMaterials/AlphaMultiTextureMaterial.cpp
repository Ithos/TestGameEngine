#include <Textures.h>
#include "../TextureMaterialCommons.h"
#include "../../GeometryItem.h"
#include "AlphaMultiTextureMaterial.h"

GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::AlphaMultiTextureMaterial(const CustomShading::MultiShadingInterface* const multiShadingInterface,
	const std::string & ambientTexDir, const std::string & diffuseTexDir,
	const std::string & specularTexDir, const std::string & emissiveTexDir, float thresholdValue, float globalAlphaValue, float shininess, bool translucent) : 
	TransparentMaterial(multiShadingInterface, thresholdValue, globalAlphaValue, shininess, translucent),
	mpTexDirManager(nullptr), mpAmbientTexture(nullptr), mpDiffuseTexture(nullptr), mpSpecularTexture(nullptr), mpEmissiveTexture(nullptr)
{
	mpAmbientTexture = new TextureParameters(ambientTexDir, -1, true);
	mpDiffuseTexture = new TextureParameters(diffuseTexDir, -1, true);
	mpSpecularTexture = new TextureParameters(specularTexDir, -1, true);
	mpEmissiveTexture = new TextureParameters(emissiveTexDir, -1, true);
	initMaterial();
}

GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::AlphaMultiTextureMaterial(const AlphaMultiTextureMaterial & ref)
{
	copy(ref);
	initMaterial();
	initTextures();
}

GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::~AlphaMultiTextureMaterial()
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

void GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::SetAmbientTexture(const std::string & ambientTexDir)
{
	if (mpAmbientTexture != nullptr)
	{
		delete mpAmbientTexture;
	}

	mpAmbientTexture = new TextureParameters(ambientTexDir, -1);
}

void GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::SetDiffuseTexture(const std::string & diffuseTexDir)
{
	if (mpDiffuseTexture != nullptr)
	{
		delete mpDiffuseTexture;
	}

	mpDiffuseTexture = new TextureParameters(diffuseTexDir, -1);
}

void GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::SetSpecularTexture(const std::string & specularTexDir)
{
	if (mpSpecularTexture != nullptr)
	{
		delete mpSpecularTexture;
	}

	mpSpecularTexture = new TextureParameters(specularTexDir, -1);
}

void GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::SetEmissiveTexture(const std::string & emissiveTexDir)
{
	if (mpEmissiveTexture != nullptr)
	{
		delete mpEmissiveTexture;
	}

	mpEmissiveTexture = new TextureParameters(emissiveTexDir, -1);
}

void GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::initMaterial()
{
	TransparentMaterial::initMaterial();
	mpTexDirManager = TexturesFiles::Textures::InitInstance(mpConfInstance->getTexturesFolder(), mpConfInstance->getTexturesConfig());
}

void GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::initShaders()
{
	mVertexShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_TEXTURE_MATERIAL_VERTEX_SHADER;
	mFragmentShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_MULTI_TEXTURE_MATERIAL_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::initTextures()
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

void GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::setProgramParameters(const QMatrix4x4 & projection, const QMatrix4x4 & view, 
	const GeometryWorldItem::GeometryItem::GeometryItem & parent)
{
	assert(mpProgram != nullptr && "Alpha Multi Texture Material --> Shader Program Null");
	if (mpProgram != nullptr)
	{
		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", projection * view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelViewMatrix", view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelMatrix", parent.GetModelMatrix());

		mpProgram->setUniformValue("textureAmbient", TEXTURE_UNIT);
		mpProgram->setUniformValue("textureDiffuse", TEXTURE_UNIT + 1);
		mpProgram->setUniformValue("textureReflective", TEXTURE_UNIT + 2);
		mpProgram->setUniformValue("textureEmissive", TEXTURE_UNIT + 3);

		mpProgram->setUniformValue("mGlobalAlphaValue", mGlobalAlphaValue);
		mpProgram->setUniformValue("mThresholdAlphaValue", mThresholdValue);
		mpProgram->setUniformValue("mShininess", mShininess);
	}
}

void GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::drawMaterial(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber)
{
	assert(mpProgram != nullptr && "Alpha Multi Texture Material --> Shader Program Null");
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

		BindTextures();

		glDrawElements(GL_TRIANGLE_STRIP, totalIndexNumber, GL_UNSIGNED_SHORT, 0);

		UnbindTextures();

		mpProgram->disableAttributeArray(vertexLocation);
		mpProgram->disableAttributeArray(textureCoordinate);
		mpProgram->disableAttributeArray(normalVector);

		vertexBuf->release();
		indexBuf->release();
	}
}

void GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::BindTextures()
{
	if (mpAmbientTexture->Texture != nullptr) mpAmbientTexture->Texture->bind(TEXTURE_UNIT);
	if (mpDiffuseTexture->Texture != nullptr) mpDiffuseTexture->Texture->bind(TEXTURE_UNIT + 1);
	if (mpSpecularTexture->Texture != nullptr) mpSpecularTexture->Texture->bind(TEXTURE_UNIT + 2);
	if (mpEmissiveTexture->Texture != nullptr) mpEmissiveTexture->Texture->bind(TEXTURE_UNIT + 3);
}

void GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::UnbindTextures()
{
	if (mpAmbientTexture->Texture != nullptr) mpAmbientTexture->Texture->release();
	if (mpDiffuseTexture->Texture != nullptr) mpDiffuseTexture->Texture->release();
	if (mpSpecularTexture->Texture != nullptr) mpSpecularTexture->Texture->release();
	if (mpEmissiveTexture->Texture != nullptr) mpEmissiveTexture->Texture->release();
}

void GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::copy(const AlphaMultiTextureMaterial & mat)
{
	TransparentMaterial::copy(mat);
	mpAmbientTexture = new TextureParameters((*mat.mpAmbientTexture));
	mpDiffuseTexture = new TextureParameters((*mat.mpDiffuseTexture));
	mpSpecularTexture = new TextureParameters((*mat.mpSpecularTexture));
	mpEmissiveTexture = new TextureParameters((*mat.mpEmissiveTexture));
}
