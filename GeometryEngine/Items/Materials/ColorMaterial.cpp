#include "../GeometryItem.h"
#include "ColorMaterial.h"

GeometryEngine::GeometryMaterial::ColorMaterial::ColorMaterial(const QVector3D & ambient, const QVector3D & diffuse,
	const QVector3D & specular, const QVector3D & emissive, float shininess) : Material(shininess), mAmbient(ambient), mDiffuse(diffuse), mSpecular(specular), mEmissive(emissive)
{
	initMaterial();
}

GeometryEngine::GeometryMaterial::ColorMaterial::ColorMaterial(const ColorMaterial & mat)
{
	copy(mat);
	initMaterial();
}

GeometryEngine::GeometryMaterial::ColorMaterial::~ColorMaterial()
{
}

GeometryEngine::GeometryMaterial::Material * GeometryEngine::GeometryMaterial::ColorMaterial::Clone() const
{
	return new ColorMaterial( *this );
}

void GeometryEngine::GeometryMaterial::ColorMaterial::initShaders()
{
	mVertexShaderKey = GeometryMaterial::MaterialConstants::COLOR_MATERIAL_VERTEX_SHADER;
	mFragmentShaderKey = GeometryMaterial::MaterialConstants::COLOR_MATERIAL_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryMaterial::ColorMaterial::setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent)
{
	assert(mpProgram != nullptr && "Color Material --> Shader Program Null");
	if (mpProgram != nullptr)
	{
		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", projection * view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelViewMatrix", view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelMatrix", parent.GetModelMatrix());
		mpProgram->setUniformValue("ambientColor", mAmbient);
		mpProgram->setUniformValue("diffuseColor", mDiffuse);
		mpProgram->setUniformValue("reflectiveColor", mSpecular);
		mpProgram->setUniformValue("emissiveColor", mEmissive);
		mpProgram->setUniformValue("mShininess", mShininess);
	}
}

void GeometryEngine::GeometryMaterial::ColorMaterial::drawMaterial(QOpenGLBuffer * arrayBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber)
{
	assert(mpProgram != nullptr && "Color Material --> Shader Program Null");
	if (mpProgram != nullptr)
	{
		// Tell OpenGL which VBOs to use
		arrayBuf->bind();
		indexBuf->bind();

		// Tell OpenGL programmable pipeline how to locate vertex position data
		int vertexLocation = mpProgram->attributeLocation("posAttr");
		mpProgram->enableAttributeArray(vertexLocation);
		mpProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

		// Tell OpenGL programmable pipeline how to locate texture coordinates
		int textureCoordinate = mpProgram->attributeLocation("TexCoord");
		mpProgram->enableAttributeArray(textureCoordinate);
		mpProgram->setAttributeBuffer(textureCoordinate, GL_FLOAT, VertexData::TEXTURE_COORDINATES_OFFSET, 3, sizeof(VertexData));

		// Tell OpenGL programmable pipeline how to locate normals
		int normalVector = mpProgram->attributeLocation("aNormal");
		mpProgram->enableAttributeArray(normalVector);
		mpProgram->setAttributeBuffer(normalVector, GL_FLOAT, VertexData::NORMALS_OFFSET, 3, sizeof(VertexData));

		// Draw cube geometry using indices from VBO 1
		glDrawElements(GL_TRIANGLE_STRIP, totalIndexNumber, GL_UNSIGNED_SHORT, 0);

		arrayBuf->release();
		indexBuf->release();
	}
}

void GeometryEngine::GeometryMaterial::ColorMaterial::copy(const ColorMaterial & mat)
{
	Material::copy(mat);
	this->mAmbient = mat.mAmbient;
	this->mDiffuse = mat.mDiffuse;
	this->mSpecular = mat.mSpecular;
	this->mShininess = mat.mShininess;
	this->mEmissive = mat.mEmissive;
}
