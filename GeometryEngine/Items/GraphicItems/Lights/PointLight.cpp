#include "../../CommonItemParameters.h"
#include "../../GeometryItem.h"
#include "PointLight.h"

GeometryEngine::GeometryWorldItem::GeometryLight::PointLight::PointLight(const QVector3D & attParam, GeometryItem::GeometryItem* boundingBox, const QVector3D & diffuse, const QVector3D & ambient,
	const QVector3D & specular, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, const LightUtils::LightFunctionalities* const manager, WorldItem * parent) :
	StencilTestLight(boundingBox, diffuse, ambient, specular, pos, rot, scale, manager, parent), mAttenuationParameters(attParam)
{
	initLight();
}

GeometryEngine::GeometryWorldItem::GeometryLight::PointLight::~PointLight()
{
}

void GeometryEngine::GeometryWorldItem::GeometryLight::PointLight::initLightShaders()
{
	mVertexShaderKey = LightShaderConstants::DEFERRED_SHADING_VERTEX_SHADER;
	mFragmentShaderKey = LightShaderConstants::POINT_LIGHT_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryWorldItem::GeometryLight::PointLight::setProgramParameters(const LightingTransformationData & transformData,
	const GBufferTextureInfo& gBuffTexInfo, const QVector3D & viewPos)
{
	assert(mpProgram != nullptr && "Shading program not found");
	{
		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", transformData.ProjectionMatrix * transformData.ViewMatrix * transformData.ModelMatrix);
		
		mpProgram->setUniformValue("mUseDiffuse", gBuffTexInfo.UseDiffuseTexture);
		mpProgram->setUniformValue("mUseAmbient", gBuffTexInfo.UseAmbientTexture);
		mpProgram->setUniformValue("mUseReflective", gBuffTexInfo.UseReflectiveTexture);

		mpProgram->setUniformValue("mPositionMap", gBuffTexInfo.PositionTexture);
		mpProgram->setUniformValue("mDiffuseColorMap", gBuffTexInfo.DiffuseTexture);
		mpProgram->setUniformValue("mAmbientColorMap", gBuffTexInfo.AmbientTexture);
		mpProgram->setUniformValue("mReflectiveColorMap", gBuffTexInfo.ReflectiveTexture);
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

void GeometryEngine::GeometryWorldItem::GeometryLight::PointLight::calculateContribution(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	// Tell OpenGL which VBOs to use
	vertexBuf->bind();
	indexBuf->bind();

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = mpProgram->attributeLocation("posAttr");
	mpProgram->enableAttributeArray(vertexLocation);
	mpProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

	// Draw light
	glDrawElements(GL_TRIANGLE_STRIP, totalIndexNum, GL_UNSIGNED_SHORT, 0);
}

void GeometryEngine::GeometryWorldItem::GeometryLight::PointLight::initLight()
{
	Light::initLight();
	if (mpBoundingBox != nullptr)
	{
		ScaleBoundingBox(mAttenuationParameters);
		mpBoundingBox->SetPosition(QVector3D(0.0f, 0.0f, 0.0f));
		mpBoundingBox->SetParent(this);
	}
}
