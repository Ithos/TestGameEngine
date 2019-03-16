#include "ColorMaterial.h"

GeometryEngine::GeometryMaterial::ColorMaterial::ColorMaterial(const QVector3D & emissive, const QVector3D & ambient, const QVector3D & diffuse,
	const QVector3D & specular, float shininess, bool isLit, bool customLight) : Material(ambient, diffuse, specular, shininess, isLit, customLight), mEmissive(emissive)
{
	initMaterial();
}

GeometryEngine::GeometryMaterial::ColorMaterial::ColorMaterial(const ColorMaterial & mat)
{
	copy(mat);
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
	if (mpProgram != nullptr)
	{
		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", projection * view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelViewMatrix", view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelMatrix", parent.GetModelMatrix());
	}
}

void GeometryEngine::GeometryMaterial::ColorMaterial::drawMaterial(QOpenGLBuffer * arrayBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber)
{
	// Tell OpenGL which VBOs to use
	arrayBuf->bind();
	indexBuf->bind();

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = mpProgram->attributeLocation("posAttr");
	mpProgram->enableAttributeArray(vertexLocation);
	mpProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

	// Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
	int color = mpProgram->attributeLocation("colAttr");
	mpProgram->enableAttributeArray(color);
	mpProgram->setAttributeBuffer(color, GL_FLOAT, VertexData::COLOR_OFFSET, 3, sizeof(VertexData));

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
}

void GeometryEngine::GeometryMaterial::ColorMaterial::copy(const ColorMaterial & mat)
{
	Material::copy(mat);
	this->mEmissive = mat.mEmissive;
	initMaterial();
}
