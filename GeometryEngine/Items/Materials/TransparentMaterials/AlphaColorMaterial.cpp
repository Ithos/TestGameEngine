#include "../../GeometryItem.h"
#include "AlphaColorMaterial.h"

GeometryEngine::GeometryMaterial::AlphaColorMaterial::AlphaColorMaterial(const CustomShading::CustomShadingInterface* const customShading, const QVector3D & ambient, const QVector3D & diffuse, 
	const QVector3D & specular, const QVector3D & emissive, float thresholdValue, float globalAlphaValue, float shininess, bool translucent) : 
	TransparentMaterial(customShading, thresholdValue, globalAlphaValue, shininess, translucent), mAmbient(ambient), mDiffuse(diffuse), mSpecular(specular), mEmissive(emissive)
{
	initMaterial();
}

GeometryEngine::GeometryMaterial::AlphaColorMaterial::AlphaColorMaterial(const AlphaColorMaterial & ref)
{
	copy(ref);
	initMaterial();
}

GeometryEngine::GeometryMaterial::AlphaColorMaterial::~AlphaColorMaterial()
{
}

void GeometryEngine::GeometryMaterial::AlphaColorMaterial::initShaders()
{
	mVertexShaderKey = GeometryMaterial::MaterialConstants::ALPHA_COLOR_MATERIAL_VERTEX_SHADER;
	mFragmentShaderKey = GeometryMaterial::MaterialConstants::ALPHA_COLOR_MATERIAL_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryMaterial::AlphaColorMaterial::initShadowMapShaders()
{
	mShadowMapVertexShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::POSITION_VERTEX_SHADER;
	mShadowMapFragmentShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_COLOR_SHADOWMAP;
}

void GeometryEngine::GeometryMaterial::AlphaColorMaterial::setProgramParameters(const QMatrix4x4 & projection, const QMatrix4x4 & view, const GeometryWorldItem::GeometryItem::GeometryItem & parent)
{
	assert(mpProgram != nullptr && "Alpha Color Material --> Shader Program Null");
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
		mpProgram->setUniformValue("mGlobalAlphaValue", mGlobalAlphaValue);
		mpProgram->setUniformValue("mThresholdAlphaValue", mThresholdValue);
		mpProgram->setUniformValue("mShininess", mShininess);
	}
}

void GeometryEngine::GeometryMaterial::AlphaColorMaterial::drawMaterial(QOpenGLBuffer * arrayBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber)
{
	assert(mpProgram != nullptr && "Alpha Color Material --> Shader Program Null");
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
	}
}

void GeometryEngine::GeometryMaterial::AlphaColorMaterial::setShadowProgramParameters(const QMatrix4x4 & modelViewProjectionMatrix)
{
	assert(mpShadowMapProgram != nullptr && "Material color shadow map program not found");
	{
		mpShadowMapProgram->setUniformValue("mModelViewProjectionMatrix", modelViewProjectionMatrix);
		mpShadowMapProgram->setUniformValue("mThresholdAlphaValue", mThresholdValue);
		mpShadowMapProgram->setUniformValue("mGlobalAlphaValue", mGlobalAlphaValue);
	}
}

void GeometryEngine::GeometryMaterial::AlphaColorMaterial::renderShadowMap(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	// Tell OpenGL which VBOs to use
	vertexBuf->bind();
	indexBuf->bind();

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = mpShadowMapProgram->attributeLocation("posAttr");
	mpShadowMapProgram->enableAttributeArray(vertexLocation);
	mpShadowMapProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

	// Draw light
	glDrawElements(GL_TRIANGLE_STRIP, totalIndexNum, GL_UNSIGNED_SHORT, 0);
}

void GeometryEngine::GeometryMaterial::AlphaColorMaterial::copy(const AlphaColorMaterial & mat)
{
	TransparentMaterial::copy(mat);
	this->mAmbient = mat.mAmbient;
	this->mDiffuse = mat.mDiffuse;
	this->mSpecular = mat.mSpecular;
	this->mEmissive = mat.mEmissive;
}
