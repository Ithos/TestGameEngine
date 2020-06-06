#include "../TextureMaterialCommons.h"
#include "AlphaNormalmapMultiTextureMaterial.h"

GeometryEngine::GeometryMaterial::AlphaNormalMapMultiTextureMaterial::AlphaNormalMapMultiTextureMaterial(const std::string & ambientTexDir, const std::string & diffuseTexDir, 
	const std::string & specularTexDir, const std::string & emissiveTexDir, const std::string & normalMapTexDir, float thresholdValue, float globalAlphaValue, float shininess):
	AlphaMultiTextureMaterial(ambientTexDir, diffuseTexDir, specularTexDir, emissiveTexDir, thresholdValue, globalAlphaValue, shininess), mpNormalMapTexture(nullptr)
{
		mpNormalMapTexture = new TextureParameters(normalMapTexDir, -1, true);
}

GeometryEngine::GeometryMaterial::AlphaNormalMapMultiTextureMaterial::AlphaNormalMapMultiTextureMaterial(const AlphaNormalMapMultiTextureMaterial & mat) : AlphaMultiTextureMaterial()
{
	copy(mat);
	initMaterial();
	initTextures();
}

GeometryEngine::GeometryMaterial::AlphaNormalMapMultiTextureMaterial::~AlphaNormalMapMultiTextureMaterial()
{
	if (mpNormalMapTexture != nullptr)
	{
		delete mpNormalMapTexture;
		mpNormalMapTexture = nullptr;
	}
}

void GeometryEngine::GeometryMaterial::AlphaNormalMapMultiTextureMaterial::SetNormalMapTexture(const std::string & normalMapTexDir)
{
	if (mpNormalMapTexture != nullptr)
	{
		delete mpNormalMapTexture;
	}
	mpNormalMapTexture = new TextureParameters(normalMapTexDir, -1);
}

void GeometryEngine::GeometryMaterial::AlphaNormalMapMultiTextureMaterial::initTextures()
{
	AlphaMultiTextureMaterial::initTextures();
	if (mpNormalMapTexture->Texture == nullptr)mpNormalMapTexture->Build();
}

void GeometryEngine::GeometryMaterial::AlphaNormalMapMultiTextureMaterial::initShaders()
{
	mVertexShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_NORMALMAP_TEXTURE_MATERIAL_VERTEX_SHADER;
	mFragmentShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_MULTI_TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryMaterial::AlphaNormalMapMultiTextureMaterial::setProgramParameters(const QMatrix4x4 & projection, const QMatrix4x4 & view, 
	const GeometryWorldItem::GeometryItem::GeometryItem & parent)
{
	assert(mpProgram != nullptr && "NormalMap Multi Texture Material --> Shader Program Null");
	GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::setProgramParameters(projection, view, parent);
	if (mpProgram != nullptr) mpProgram->setUniformValue("normalMapTexture", TEXTURE_UNIT + 4);
}

void GeometryEngine::GeometryMaterial::AlphaNormalMapMultiTextureMaterial::bindTextures()
{
	GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::bindTextures();
	if (mpNormalMapTexture != nullptr) mpNormalMapTexture->Texture->bind(TEXTURE_UNIT + 4);
}

void GeometryEngine::GeometryMaterial::AlphaNormalMapMultiTextureMaterial::copy(const AlphaNormalMapMultiTextureMaterial & mat)
{
	GeometryEngine::GeometryMaterial::AlphaMultiTextureMaterial::copy(mat);
	mpNormalMapTexture = new TextureParameters((*mat.mpNormalMapTexture));
}


