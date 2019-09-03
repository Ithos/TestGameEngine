#include "DirectionalLight.h"

GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalLight::DirectionalLight(const QVector3D & direction, GeometryItem::GeometryItem* boundingBox, 
	const QVector3D & diffuse, const QVector3D & ambient,
	const QVector3D & specular, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent) :
	DeferredShadingLight(boundingBox, diffuse, ambient, specular, pos, rot, scale, parent), mDirection(direction)
{
	initLight();
}

GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalLight::~DirectionalLight()
{
}

void GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalLight::initLightShaders()
{
	mVertexShaderKey = LightShaderConstants::DEFERRED_SHADING_VERTEX_SHADER;
	mFragmentShaderKey = LightShaderConstants::DIRECTIONAL_LIGHT_FRAGMENT_SHADER; 
}

void GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalLight::setProgramParameters(const LightingTransformationData & transformData,
	const GBufferTextureInfo& gBuffTexInfo, const QVector3D & viewPos)
{
	assert( mpProgram != nullptr && "Shading program not found");
	{
		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", transformData.ModelMatrix);

		mpProgram->setUniformValue("mUseDiffuse", gBuffTexInfo.UseDiffuseTexture);
		mpProgram->setUniformValue("mUseAmbient", gBuffTexInfo.UseAmbientTexture);
		mpProgram->setUniformValue("mUseReflective", gBuffTexInfo.UseReflectiveTexture);
		mpProgram->setUniformValue("mUseEmissive", gBuffTexInfo.UseEmissiveTexture);

		mpProgram->setUniformValue("mPositionMap", gBuffTexInfo.PositionTexture);
		mpProgram->setUniformValue("mDiffuseColorMap", gBuffTexInfo.DiffuseTexture);
		mpProgram->setUniformValue("mAmbientColorMap", gBuffTexInfo.AmbientTexture);
		mpProgram->setUniformValue("mReflectiveColorMap", gBuffTexInfo.ReflectiveTexture);
		mpProgram->setUniformValue("mEmissiveColorMap", gBuffTexInfo.EmissiveTexture);
		mpProgram->setUniformValue("mNormalMap", gBuffTexInfo.NormalTexture);

		mpProgram->setUniformValue("mTextureSize", gBuffTexInfo.TextureSize);

		//Set light properties
		mpProgram->setUniformValue("mLight.ambient", this->mColorAmbient);
		mpProgram->setUniformValue("mLight.diffuse", this->mColorDiffuse);
		mpProgram->setUniformValue("mLight.specular", this->mColorSpecular);
		mpProgram->setUniformValue("mLight.direction", this->GetRotation() * this->mDirection);

		mpProgram->setUniformValue("mViewPos", viewPos);
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::DirectionalLight::calculateContribution(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum)
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
