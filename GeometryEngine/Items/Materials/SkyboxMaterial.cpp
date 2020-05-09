#include <Textures.h>
#include "TextureMaterialCommons.h"
#include "../GeometryItem.h"
#include "SkyboxMaterial.h"

GeometryEngine::GeometryMaterial::SkyboxMaterial::SkyboxMaterial(const std::string & textureDir) :
	Material(QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), 0.0), mpTexDirManager(nullptr), mpSkyboxTexture(nullptr)
{
	mpAmbientTexture = new TextureParameters(TextureConstant::TEST_BLACK_TEXTURE, -1, true);
	mpDiffuseTexture = new TextureParameters(TextureConstant::TEST_BLACK_TEXTURE, -1, true);;
	mpSpecularTexture = new TextureParameters(TextureConstant::TEST_BLACK_TEXTURE, -1, true);;
	mpSkyboxTexture = new TextureParameters(textureDir, -1, true);
	mIdentityMatrix.setToIdentity();
	initMaterial();
}

GeometryEngine::GeometryMaterial::SkyboxMaterial::~SkyboxMaterial()
{
	if(mpSkyboxTexture != nullptr)
	{
		delete mpSkyboxTexture;
	}

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
}

GeometryEngine::GeometryMaterial::SkyboxMaterial::SkyboxMaterial(const SkyboxMaterial & mat)
{
	copy(mat);
	initTextures();
}

void GeometryEngine::GeometryMaterial::SkyboxMaterial::SetTexture(const std::string & ambientTexDir)
{
	if (mpSkyboxTexture != nullptr)
	{
		delete mpSkyboxTexture;
	}

	mpSkyboxTexture = new TextureParameters(ambientTexDir, -1);
}

void GeometryEngine::GeometryMaterial::SkyboxMaterial::initMaterial()
{
	Material::initMaterial();
	mpTexDirManager = TexturesFiles::Textures::InitInstance(mpConfInstance->getTexturesFolder(), mpConfInstance->getTexturesConfig());
}

void GeometryEngine::GeometryMaterial::SkyboxMaterial::initShaders()
{
	mVertexShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::TEXTURE_MATERIAL_VERTEX_SHADER;
	mFragmentShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::MULTI_TEXTURE_MATERIAL_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryMaterial::SkyboxMaterial::initTextures()
{
	if (mpAmbientTexture->Texture == nullptr)
		mpAmbientTexture->Build();
	if (mpDiffuseTexture->Texture == nullptr)
		mpDiffuseTexture->Build();
	if (mpSpecularTexture->Texture == nullptr)
		mpSpecularTexture->Build();
	if (mpSkyboxTexture->Texture == nullptr)
		mpSkyboxTexture->Build();
}

void GeometryEngine::GeometryMaterial::SkyboxMaterial::setProgramParameters(const QMatrix4x4 & projection, const QMatrix4x4 & view, const GeometryWorldItem::GeometryItem::GeometryItem & parent)
{
	assert(mpProgram != nullptr && "Skybox Material --> Shader Program Null");
	if (mpProgram != nullptr)
	{
		// Extract the rotation from the view matrix
		QMatrix4x4 rot(view); rot.setColumn(3, QVector4D(0,0,0,1) );

		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", projection * rot); // Nomodel matrix, the skybox does not move or rotate
		mpProgram->setUniformValue("modelViewMatrix", rot);
		mpProgram->setUniformValue("modelMatrix", mIdentityMatrix);

		mpProgram->setUniformValue("textureAmbient", TEXTURE_UNIT);
		mpProgram->setUniformValue("textureDiffuse", TEXTURE_UNIT + 1);
		mpProgram->setUniformValue("textureReflective", TEXTURE_UNIT + 2);
		mpProgram->setUniformValue("textureEmissive", TEXTURE_UNIT + 3);
	}
}

void GeometryEngine::GeometryMaterial::SkyboxMaterial::drawMaterial(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber)
{
	assert(mpProgram != nullptr && "Skybox Material --> Shader Program Null");
	if (mpProgram != nullptr)
	{
		glDisable(GL_CULL_FACE);

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

		glEnable(GL_CULL_FACE);

	}
}

void GeometryEngine::GeometryMaterial::SkyboxMaterial::bindTextures()
{
	if (mpAmbientTexture->Texture != nullptr)
		mpAmbientTexture->Texture->bind(TEXTURE_UNIT);
	if (mpDiffuseTexture->Texture != nullptr)
		mpDiffuseTexture->Texture->bind(TEXTURE_UNIT + 1);
	if (mpSpecularTexture->Texture != nullptr)
		mpSpecularTexture->Texture->bind(TEXTURE_UNIT + 2);
	if (mpSkyboxTexture->Texture != nullptr)
		mpSkyboxTexture->Texture->bind(TEXTURE_UNIT + 3);
}

void GeometryEngine::GeometryMaterial::SkyboxMaterial::copy(const SkyboxMaterial & mat)
{
	mpAmbientTexture = new TextureParameters((*mat.mpAmbientTexture));
	mpDiffuseTexture = new TextureParameters((*mat.mpDiffuseTexture));
	mpSpecularTexture = new TextureParameters((*mat.mpSpecularTexture));
	mpSkyboxTexture = new TextureParameters((*mat.mpSkyboxTexture));
	initMaterial();
}
