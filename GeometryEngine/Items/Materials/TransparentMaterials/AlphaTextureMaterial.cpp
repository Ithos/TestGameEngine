#include <Textures.h>
#include "../TextureMaterialCommons.h"
#include "../../GeometryItem.h"
#include "AlphaTextureMaterial.h"

GeometryEngine::GeometryMaterial::AlphaTextureMaterial::AlphaTextureMaterial(const std::string & texDir, float thresholdValue, float globalAlphaValue, float shininess):
	TransparentMaterial(thresholdValue, globalAlphaValue, shininess), mpTexture(nullptr)
{
	mpTexture = new TextureParameters(texDir, -1, true);
	initMaterial();
}

GeometryEngine::GeometryMaterial::AlphaTextureMaterial::AlphaTextureMaterial(const AlphaTextureMaterial & ref)
{
	copy(ref);
	initTextures();
	initMaterial();
}

GeometryEngine::GeometryMaterial::AlphaTextureMaterial::~AlphaTextureMaterial()
{
	if (mpTexture != nullptr)
	{
		delete mpTexture;
	}
}

void GeometryEngine::GeometryMaterial::AlphaTextureMaterial::SetMaterialTexture(const std::string & texDir)
{
	if (mpTexture != nullptr)
	{
		delete mpTexture;
	}
	mpTexture = new TextureParameters(texDir, -1);
}

void GeometryEngine::GeometryMaterial::AlphaTextureMaterial::initMaterial()
{
	TransparentMaterial::initMaterial();
	mpTexDirManager = TexturesFiles::Textures::InitInstance(mpConfInstance->getTexturesFolder(), mpConfInstance->getTexturesConfig());
}

void GeometryEngine::GeometryMaterial::AlphaTextureMaterial::initShaders()
{
	mVertexShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_TEXTURE_MATERIAL_VERTEX_SHADER;
	mFragmentShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_TEXTURE_MATERIAL_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryMaterial::AlphaTextureMaterial::initTextures()
{
	if (mpTexture->Texture == nullptr) mpTexture->Build();
}

void GeometryEngine::GeometryMaterial::AlphaTextureMaterial::setProgramParameters(const QMatrix4x4 & projection, const QMatrix4x4 & view, 
	const GeometryWorldItem::GeometryItem::GeometryItem & parent)
{
	assert(mpProgram != nullptr && "Alpha Texture Material --> Shader Program Null");
	if (mpProgram != nullptr)
	{
		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", projection * view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelViewMatrix", view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelMatrix", parent.GetModelMatrix());

		mpProgram->setUniformValue("texture", TEXTURE_UNIT);

		mpProgram->setUniformValue("mGlobalAlphaValue", mGlobalAlphaValue);
		mpProgram->setUniformValue("mThresholdAlphaValue", mThresholdValue);
		mpProgram->setUniformValue("mShininess", mShininess);
	}
}

void GeometryEngine::GeometryMaterial::AlphaTextureMaterial::drawMaterial(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber)
{
	assert(mpProgram != nullptr && "Alpha Texture Material --> Shader Program Null");
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
	}
}

void GeometryEngine::GeometryMaterial::AlphaTextureMaterial::bindTextures()
{
	if (mpTexture->Texture != nullptr) mpTexture->Texture->bind(TEXTURE_UNIT);
}

void GeometryEngine::GeometryMaterial::AlphaTextureMaterial::copy(const AlphaTextureMaterial & mat)
{
	TransparentMaterial::copy(mat);
	mpTexture = new TextureParameters((*mat.mpTexture));
}
