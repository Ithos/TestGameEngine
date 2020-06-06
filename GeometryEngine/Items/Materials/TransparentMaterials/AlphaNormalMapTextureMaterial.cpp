#include "../TextureMaterialCommons.h"
#include "AlphaNormalMapTextureMaterial.h"

GeometryEngine::GeometryMaterial::AlphaNormalMapTextureMaterial::AlphaNormalMapTextureMaterial(const std::string & texDir, const std::string & normalMapTexDir, 
	float thresholdValue, float globalAlphaValue, float shininess): AlphaTextureMaterial(texDir, thresholdValue, globalAlphaValue, shininess), mpNormalMapTexture(nullptr)
{
	mpNormalMapTexture = new TextureParameters(normalMapTexDir, -1, true);
}

GeometryEngine::GeometryMaterial::AlphaNormalMapTextureMaterial::AlphaNormalMapTextureMaterial(const AlphaNormalMapTextureMaterial & mat) : AlphaTextureMaterial()
{
	copy(mat);
	initMaterial();
	initTextures();
}

GeometryEngine::GeometryMaterial::AlphaNormalMapTextureMaterial::~AlphaNormalMapTextureMaterial()
{
	if (mpNormalMapTexture != nullptr)
	{
		delete mpNormalMapTexture;
		mpNormalMapTexture = nullptr;
	}
}

void GeometryEngine::GeometryMaterial::AlphaNormalMapTextureMaterial::SetNormalMapTexture(const std::string & normalMapTexDir)
{
	if (mpNormalMapTexture != nullptr)
	{
		delete mpNormalMapTexture;
	}
	mpNormalMapTexture = new TextureParameters(normalMapTexDir, -1);
}

void GeometryEngine::GeometryMaterial::AlphaNormalMapTextureMaterial::initTextures()
{
	AlphaTextureMaterial::initTextures();
	if (mpNormalMapTexture->Texture == nullptr)mpNormalMapTexture->Build();
}

void GeometryEngine::GeometryMaterial::AlphaNormalMapTextureMaterial::initShaders()
{
	mVertexShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_NORMALMAP_TEXTURE_MATERIAL_VERTEX_SHADER;
	mFragmentShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryMaterial::AlphaNormalMapTextureMaterial::setProgramParameters(const QMatrix4x4 & projection, const QMatrix4x4 & view, 
	const GeometryWorldItem::GeometryItem::GeometryItem & parent)
{
	assert(mpProgram != nullptr && "Alpha NormalMap Texture Material --> Shader Program Null");
	GeometryEngine::GeometryMaterial::AlphaTextureMaterial::setProgramParameters(projection, view, parent);
	if (mpProgram != nullptr) mpProgram->setUniformValue("normalMapTexture", TEXTURE_UNIT + 1);
}

void GeometryEngine::GeometryMaterial::AlphaNormalMapTextureMaterial::bindTextures()
{
	GeometryEngine::GeometryMaterial::AlphaTextureMaterial::bindTextures();
	if (mpNormalMapTexture != nullptr) mpNormalMapTexture->Texture->bind(TEXTURE_UNIT + 1);
}

void GeometryEngine::GeometryMaterial::AlphaNormalMapTextureMaterial::copy(const AlphaNormalMapTextureMaterial & mat)
{
	GeometryEngine::GeometryMaterial::AlphaTextureMaterial::copy(mat);
	mpNormalMapTexture = new TextureParameters((*mat.mpNormalMapTexture));
}
