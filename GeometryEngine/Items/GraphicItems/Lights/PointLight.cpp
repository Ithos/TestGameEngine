#include "PointLight.h"

GeometryEngine::PointLight::PointLight(const QVector3D & attParam, GeometryItem* boundingBox, const QVector3D & diffuse, const QVector3D & ambient,
	const QVector3D & specular, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent) :
	StencilTestLight(boundingBox, diffuse, ambient, specular, pos, rot, scale, parent), mAttenuationParameters(attParam)
{
	initLight();
}

GeometryEngine::PointLight::~PointLight()
{
}

void GeometryEngine::PointLight::initLightShaders()
{
	mVertexShaderKey = LightShaderConstants::DEFERRED_SHADING_VERTEX_SHADER;
	mFragmentShaderKey = LightShaderConstants::POINT_LIGHT_FRAGMENT_SHADER_DS;
}

void GeometryEngine::PointLight::setProgramParameters(const LightingTransformationData & transformData, const MaterialLightingParameters & matParam, 
	const GBufferTextureInfo& gBuffTexInfo, const QVector3D & viewPos)
{
	assert(mpProgram != nullptr && "Shading program not found");
	{
		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", transformData.ProjectionMatrix * transformData.ViewMatrix * transformData.ModelMatrix);
		
		mpProgram->setUniformValue("mPositionMap", gBuffTexInfo.PositionTexture);
		mpProgram->setUniformValue("mColorMap", gBuffTexInfo.DiffuseTexture);
		mpProgram->setUniformValue("mNormalMap", gBuffTexInfo.NormalTexture);

		mpProgram->setUniformValue("mTextureSize", gBuffTexInfo.TextureSize);

		//Set light properties
		mpProgram->setUniformValue("mLight.ambient", this->mColorAmbient);
		mpProgram->setUniformValue("mLight.diffuse", this->mColorDiffuse);
		mpProgram->setUniformValue("mLight.specular", this->mColorSpecular);
		mpProgram->setUniformValue("mLight.position", this->mPosition);

		mpProgram->setUniformValue("mLight.attenuation.constant", this->mAttenuationParameters.x());
		mpProgram->setUniformValue("mLight.attenuation.linear", this->mAttenuationParameters.y());
		mpProgram->setUniformValue("mLight.attenuation.quadratic", this->mAttenuationParameters.z());

		mpProgram->setUniformValue("mViewPos", viewPos);
	}
}

void GeometryEngine::PointLight::calculateContribution(QOpenGLBuffer * arrayBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	// Tell OpenGL which VBOs to use
	arrayBuf->bind();
	indexBuf->bind();

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = mpProgram->attributeLocation("posAttr");
	mpProgram->enableAttributeArray(vertexLocation);
	mpProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

	// Draw light
	glDrawElements(GL_TRIANGLE_STRIP, totalIndexNum, GL_UNSIGNED_SHORT, 0);
}

void GeometryEngine::PointLight::initLight()
{
	Light::initLight();
	if (mpBoundingBox != nullptr)
	{
		ScaleBoundingBox(mAttenuationParameters);
		mpBoundingBox->SetPosition(QVector3D(0.0f, 0.0f, 0.0f));
		mpBoundingBox->SetParent(this);
	}
}