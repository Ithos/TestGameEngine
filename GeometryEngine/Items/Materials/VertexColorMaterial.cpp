#include "../GeometryItem.h"
#include "VertexColorMaterial.h"

GeometryEngine::GeometryMaterial::VertexColorMaterial::VertexColorMaterial(float shininess) : Material(shininess)
{
	initMaterial();
}

GeometryEngine::GeometryMaterial::VertexColorMaterial::VertexColorMaterial(const VertexColorMaterial & mat)
{
	copy(mat);
}

GeometryEngine::GeometryMaterial::VertexColorMaterial::~VertexColorMaterial()
{
}

void GeometryEngine::GeometryMaterial::VertexColorMaterial::initShaders()
{
	mVertexShaderKey = GeometryMaterial::MaterialConstants::VERTEX_COLOR_MATERIAL_VERTEX_SHADER;
	mFragmentShaderKey = GeometryMaterial::MaterialConstants::VERTEX_COLOR_MATERIAL_VERTEX_SHADER;
}

void GeometryEngine::GeometryMaterial::VertexColorMaterial::setProgramParameters(const QMatrix4x4 & projection, const QMatrix4x4 & view, const GeometryWorldItem::GeometryItem::GeometryItem & parent)
{
	assert(mpProgram != nullptr && "Vertex Color Material --> Shader Program Null");
	if (mpProgram != nullptr)
	{
		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", projection * view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelViewMatrix", view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelMatrix", parent.GetModelMatrix());
		mpProgram->setUniformValue("mShininess", mShininess);
	}
}

void GeometryEngine::GeometryMaterial::VertexColorMaterial::drawMaterial(QOpenGLBuffer * arrayBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber)
{
	assert(mpProgram != nullptr && "Vertex Color Material --> Shader Program Null");
	if (mpProgram != nullptr)
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

		arrayBuf->release();
		indexBuf->release();
	}
}

void GeometryEngine::GeometryMaterial::VertexColorMaterial::copy(const VertexColorMaterial & mat)
{
	Material::copy(mat);
	initMaterial();
}
