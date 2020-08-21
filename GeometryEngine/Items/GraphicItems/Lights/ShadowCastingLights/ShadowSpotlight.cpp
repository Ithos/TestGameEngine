#include "../../../CommonItemParameters.h"
#include "../../../Item Utils/Viewport.h"
#include "../../../GeometryItem.h"
#include "ShadowSpotlight.h"

GeometryEngine::GeometryWorldItem::GeometryLight::ShadowSpotlight::ShadowSpotlight(float maxLightAngle, const QVector3D & attParams, 
	const GeometryItemUtils::Viewport & viewport, const QVector3D & direction, GeometryItem::GeometryItem * boundingBox, const QVector3D & diffuse, 
	const QVector3D & ambient, const QVector3D & specular, const QVector3D & pos, const QVector3D & rot, float maxShadowBias, const QVector3D & scale, 
	const LightUtils::LightFunctionalities* const manager, WorldItem * parent) :
	mAttenuationParameters(attParams), mMaxLightAngle(maxLightAngle), ShadowMapLight(viewport, direction, boundingBox, diffuse, ambient, specular, pos, rot, maxShadowBias, scale, manager, parent)
{
	initLight();
}

GeometryEngine::GeometryWorldItem::GeometryLight::ShadowSpotlight::~ShadowSpotlight()
{
}

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowSpotlight::initLightShaders()
{
	mVertexShaderKey = ShadowMapConstants::SPOTLIGHT_SHADOW_LIGHT_VERTEX_SHADER;
	mFragmentShaderKey = ShadowMapConstants::SPOTLIGHT_SHADOW_LIGHT_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowSpotlight::setProgramParameters(const LightingTransformationData & transformData, const GBufferTextureInfo & gBuffTexInfo, const QVector3D & viewPos)
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
		mpProgram->setUniformValue("mShadowMap", gBuffTexInfo.TmpTexture);

		mpProgram->setUniformValue("mTextureSize", gBuffTexInfo.TextureSize);

		mpProgram->setUniformValue("mLightSpaceMatrix", mpViewport->GetViewProjectionMatrix());

		//Set light properties
		mpProgram->setUniformValue("mLight.ambient", this->mColorAmbient);
		mpProgram->setUniformValue("mLight.diffuse", this->mColorDiffuse);
		mpProgram->setUniformValue("mLight.specular", this->mColorSpecular);
		mpProgram->setUniformValue("mLight.position", this->mPosition);
		mpProgram->setUniformValue("mLight.direction", this->GetRotation() * this->mDirection);
		mpProgram->setUniformValue("mLight.maxAngle", this->mMaxLightAngle);

		mpProgram->setUniformValue("mLight.attenuation.constant", this->mAttenuationParameters.x());
		mpProgram->setUniformValue("mLight.attenuation.linear", this->mAttenuationParameters.y());
		mpProgram->setUniformValue("mLight.attenuation.quadratic", this->mAttenuationParameters.z());

		mpProgram->setUniformValue("mViewPos", viewPos);
		mpProgram->setUniformValue("mMaxBias", mMaxShadowBias);
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowSpotlight::calculateContribution(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum)
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

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowSpotlight::initLight()
{
	GeometryWorldItem::GeometryLight::ShadowMapLight::initLight();
	if (mpBoundingBox != nullptr)
	{
		ScaleBoundingBox(mAttenuationParameters);
		mpBoundingBox->SetPosition(QVector3D(0.0f, 0.0f, 0.0f));
		mpBoundingBox->SetParent(this);
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::ShadowSpotlight::copy(const ShadowSpotlight & ref)
{
	ShadowMapLight::copy(ref);
	this->mAttenuationParameters = ref.mAttenuationParameters;
	this->mMaxLightAngle = ref.mMaxLightAngle;
}
