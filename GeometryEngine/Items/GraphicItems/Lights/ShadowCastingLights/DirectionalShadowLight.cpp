#include "../../../CommonItemParameters.h"
#include "../../../Item Utils/Viewport.h"
#include "../../../GeometryItem.h"
#include "../../LightUtils/LightComponentManager.h"
#include "DirectionalShadowLight.h"

GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalShadowLight::DirectionalShadowLight(const GeometryItemUtils::Viewport& viewport, const QVector3D & direction, 
	GeometryItem::GeometryItem * boundingBox, const QVector3D & diffuse, const QVector3D & ambient, const QVector3D & specular, const QVector3D & pos, 
	const QVector3D & rot, float maxShadowBias, const QVector3D & scale, const LightUtils::LightComponentManager* const manager, WorldItem * parent) :
	ShadowMapLight(viewport, direction, boundingBox, diffuse, ambient, specular, pos, rot, maxShadowBias, scale, manager, parent)
{
	initLight();
}

GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalShadowLight::~DirectionalShadowLight()
{
}

void GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalShadowLight::checkStencylTestFunctionality()
{
	if (mpFunctionalitiesManager != nullptr && mpFunctionalitiesManager->ContainsLightShadingComponent(LightUtils::STENCIL_TESTING))
	{
		mpFunctionalitiesManager->RemoveLightShadingComponent(LightUtils::STENCIL_TESTING);
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalShadowLight::initLightShaders()
{
	mVertexShaderKey = ShadowMapConstants::DIRECTIONAL_SHADOW_LIGHT_VERTEX_SHADER;
	mFragmentShaderKey = ShadowMapConstants::DIRECTIONAL_SHADOW_LIGHT_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalShadowLight::setProgramParameters(const LightingTransformationData & transformData, 
	const BuffersInfo& buffInfo, const QVector3D & viewPos)
{
	assert(mpProgram != nullptr && "Shading program not found");
	{
		GBufferTextureInfo gBuffTexInfo = (*buffInfo.GeometryBufferInfo);
		ShadingBufferTextureInfo tBufferTexInfo = (*buffInfo.ShadingBufferInfo);

		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", transformData.ModelMatrix);

		mpProgram->setUniformValue("mUseDiffuse", gBuffTexInfo.UseDiffuseTexture);
		mpProgram->setUniformValue("mUseAmbient", gBuffTexInfo.UseAmbientTexture);
		mpProgram->setUniformValue("mUseReflective", gBuffTexInfo.UseReflectiveTexture);

		mpProgram->setUniformValue("mPositionMap", gBuffTexInfo.PositionTexture);
		mpProgram->setUniformValue("mDiffuseColorMap", gBuffTexInfo.DiffuseTexture);
		mpProgram->setUniformValue("mAmbientColorMap", gBuffTexInfo.AmbientTexture);
		mpProgram->setUniformValue("mReflectiveColorMap", gBuffTexInfo.ReflectiveTexture);
		mpProgram->setUniformValue("mNormalMap", gBuffTexInfo.NormalTexture);
		mpProgram->setUniformValue("mShadowMap", tBufferTexInfo.ShadowMapTexture);

		mpProgram->setUniformValue("mTextureSize", gBuffTexInfo.TextureSize);

		//Set light properties
		mpProgram->setUniformValue("mLight.ambient", this->mColorAmbient);
		mpProgram->setUniformValue("mLight.diffuse", this->mColorDiffuse);
		mpProgram->setUniformValue("mLight.specular", this->mColorSpecular);
		mpProgram->setUniformValue("mLight.direction", this->GetRotation() * this->mDirection);

		mpProgram->setUniformValue("mLightSpaceMatrix", mpViewport->GetViewProjectionMatrix());

		mpProgram->setUniformValue("mViewPos", viewPos);
		mpProgram->setUniformValue("mMaxBias", mMaxShadowBias);
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalShadowLight::calculateContribution(QOpenGLBuffer * vertexBuf, 
	QOpenGLBuffer * indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum)
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
