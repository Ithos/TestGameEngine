#include "NormalMapTextureMaterial.h"

GeometryEngine::GeometryMaterial::NormalMapTextureMaterial::NormalMapTextureMaterial(const std::string & texDir, const std::string & normalMapDir, float shininess)
	: TextureMaterial(texDir, shininess), mpNormalMapTexture(nullptr)
{
	mpNormalMapTexture = new TextureParameters(normalMapDir, -1, true);
}

GeometryEngine::GeometryMaterial::NormalMapTextureMaterial::NormalMapTextureMaterial(const NormalMapTextureMaterial & mat) : TextureMaterial()
{
	copy(mat);
	buildTextures(this->mTexturesList);
}

GeometryEngine::GeometryMaterial::Material * GeometryEngine::GeometryMaterial::NormalMapTextureMaterial::Clone() const
{
	return new NormalMapTextureMaterial(*this);
}

GeometryEngine::GeometryMaterial::NormalMapTextureMaterial::~NormalMapTextureMaterial()
{

	if (mpNormalMapTexture != nullptr)
	{
		delete mpNormalMapTexture;
		mpNormalMapTexture = nullptr;
	}
}

void GeometryEngine::GeometryMaterial::NormalMapTextureMaterial::buildTextures(const std::list<TextureParameters*>& textureDirs)
{
	TextureMaterial::buildTextures(textureDirs);
	if (mpNormalMapTexture->Texture == nullptr)
		mpNormalMapTexture->Build();
}

void GeometryEngine::GeometryMaterial::NormalMapTextureMaterial::initShaders()
{
	mVertexShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::NORMALMAP_TEXTURE_MATERIAL_VERTEX_SHADER;
	mFragmentShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryMaterial::NormalMapTextureMaterial::setProgramParameters(const QMatrix4x4 & projection, const QMatrix4x4 & view, const GeometryWorldItem::GeometryItem::GeometryItem & parent)
{
	assert(mpProgram != nullptr && "NormlaMap Texture Material --> Shader Program Null");
	GeometryEngine::GeometryMaterial::TextureMaterial::setProgramParameters(projection, view, parent);
	if (mpProgram != nullptr) mpProgram->setUniformValue("normalMapTexture", TEXTURE_UNIT + 1);
}

void GeometryEngine::GeometryMaterial::NormalMapTextureMaterial::bindTextures()
{
	GeometryEngine::GeometryMaterial::TextureMaterial::bindTextures();
	if (mpNormalMapTexture != nullptr) mpNormalMapTexture->Texture->bind(TEXTURE_UNIT + 1);
}

void GeometryEngine::GeometryMaterial::NormalMapTextureMaterial::copy(const NormalMapTextureMaterial & mat)
{
	TextureMaterial::copy(mat);
	mpNormalMapTexture = new TextureParameters((*mat.mpNormalMapTexture));
}
