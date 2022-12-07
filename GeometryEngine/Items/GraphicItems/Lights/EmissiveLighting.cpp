#include "../../CommonItemParameters.h"
#include "../../GeometryItem.h"
#include "EmissiveLighting.h"

GeometryEngine::GeometryWorldItem::GeometryLight::EmissiveLighting::EmissiveLighting(GeometryItem::GeometryItem * boundingBox, const LightUtils::LightComponentManager* const manager, WorldItem * parent) :
	DeferredShadingLight(boundingBox, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f),
		QVector3D(0.0f, 0.0f, 0.0f), manager, parent)
{
	initLight();
}

GeometryEngine::GeometryWorldItem::GeometryLight::EmissiveLighting::~EmissiveLighting()
{
}

void GeometryEngine::GeometryWorldItem::GeometryLight::EmissiveLighting::initLightShaders()
{
	mVertexShaderKey = LightShaderConstants::DEFERRED_SHADING_VERTEX_SHADER;
	mFragmentShaderKey = LightShaderConstants::EMISSIVE_LIGHTING_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryWorldItem::GeometryLight::EmissiveLighting::setProgramParameters(const LightingTransformationData & transformData, const BuffersInfo& buffInfo, const QVector3D & viewPos)
{
	assert(mpProgram != nullptr && "Shading program not found");
	{
		GBufferTextureInfo gBuffTexInfo = (*buffInfo.GeometryBufferInfo);

		mpProgram->setUniformValue("modelViewProjectionMatrix", transformData.ModelMatrix);
		mpProgram->setUniformValue("mEmissiveColorMap", gBuffTexInfo.EmissiveTexture);
		mpProgram->setUniformValue("mTextureSize", gBuffTexInfo.TextureSize);
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::EmissiveLighting::calculateContribution(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum)
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

	mpProgram->disableAttributeArray(vertexLocation);

	vertexBuf->release();
	indexBuf->release();
}
