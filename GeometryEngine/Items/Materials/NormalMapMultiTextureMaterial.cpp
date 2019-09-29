#include "NormalMapMultiTextureMaterial.h"

GeometryEngine::GeometryMaterial::NormalMapMultiTextureMaterial::NormalMapMultiTextureMaterial(const std::string & ambientTexDir, const std::string & diffuseTexDir, 
	const std::string & specularTexDir, const std::string & emissiveTexDir, const std::string & normalMapTexDir, float shininess) :
	MultiTextureMaterial(ambientTexDir, diffuseTexDir, specularTexDir, emissiveTexDir, shininess)
{
	mpNormalMapTexture = new TextureParameters(normalMapTexDir, -1, true);
}

GeometryEngine::GeometryMaterial::NormalMapMultiTextureMaterial::NormalMapMultiTextureMaterial(const NormalMapMultiTextureMaterial & mat) : MultiTextureMaterial()
{
	copy(mat);
	initTextures();
}

GeometryEngine::GeometryMaterial::Material * GeometryEngine::GeometryMaterial::NormalMapMultiTextureMaterial::Clone() const
{
	return new NormalMapMultiTextureMaterial(*this);
}

GeometryEngine::GeometryMaterial::NormalMapMultiTextureMaterial::~NormalMapMultiTextureMaterial()
{
	if (mpNormalMapTexture != nullptr)
	{
		delete mpNormalMapTexture;
		mpNormalMapTexture = nullptr;
	}
}

void GeometryEngine::GeometryMaterial::NormalMapMultiTextureMaterial::initTextures()
{
	MultiTextureMaterial::initTextures();
	if (mpNormalMapTexture->Texture == nullptr)
		mpNormalMapTexture->Build();
}

void GeometryEngine::GeometryMaterial::NormalMapMultiTextureMaterial::initShaders()
{
	mVertexShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::NORMALMAP_TEXTURE_MATERIAL_VERTEX_SHADER;
	mFragmentShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::MULTI_TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryMaterial::NormalMapMultiTextureMaterial::setProgramParameters(const QMatrix4x4 & projection, const QMatrix4x4 & view, const GeometryWorldItem::GeometryItem::GeometryItem & parent)
{
	assert(mpProgram != nullptr && "NormalMap Multi Texture Material --> Shader Program Null");
	GeometryEngine::GeometryMaterial::MultiTextureMaterial::setProgramParameters(projection, view, parent);
	if (mpProgram != nullptr) mpProgram->setUniformValue("normalMapTexture", TEXTURE_UNIT + 4);
}

void GeometryEngine::GeometryMaterial::NormalMapMultiTextureMaterial::bindTextures()
{
	GeometryEngine::GeometryMaterial::MultiTextureMaterial::bindTextures();
	if (mpNormalMapTexture != nullptr) mpNormalMapTexture->Texture->bind(TEXTURE_UNIT + 4);
}

void GeometryEngine::GeometryMaterial::NormalMapMultiTextureMaterial::copy(const NormalMapMultiTextureMaterial & mat)
{
	MultiTextureMaterial::copy(mat);
	mpNormalMapTexture = new TextureParameters((*mat.mpNormalMapTexture));
}
