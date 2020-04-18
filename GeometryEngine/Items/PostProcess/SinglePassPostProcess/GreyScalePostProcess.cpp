#include "../../CommonItemParameters.h"
#include "../../GeometryItem.h"
#include "GreyScalePostProcess.h"

GeometryEngine::GeometryPostProcess::SinglePassPostProcess::GreyScalePostProcess::GreyScalePostProcess(const GeometryWorldItem::GeometryItem::GeometryItem & boundingGeometry) :
	SinglePassPostProcess(boundingGeometry)
{
	initPostProcess();
}

GeometryEngine::GeometryPostProcess::SinglePassPostProcess::GreyScalePostProcess::~GreyScalePostProcess()
{
}

void GeometryEngine::GeometryPostProcess::SinglePassPostProcess::GreyScalePostProcess::initPostProcessShaders()
{
	mVertexShaderKey = GeometryEngine::GeometryPostProcess::PostProcessShaderConstants::POST_PROCESS_VERTEX_SHADER;
	mFragmentShaderKey = GeometryEngine::GeometryPostProcess::PostProcessShaderConstants::GREY_SCALE_PROCESS_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryPostProcess::SinglePassPostProcess::GreyScalePostProcess::setProgramParameters(const GBufferTextureInfo & gBuffTexInfo)
{
	assert(mpProgram != nullptr && "Shading program not found");
	{
		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", mpBoundingGeometry->GetModelMatrix());
		mpProgram->setUniformValue("mTexture", gBuffTexInfo.FinalTexture);
		mpProgram->setUniformValue("mTextureSize", gBuffTexInfo.TextureSize);
	}
}

void GeometryEngine::GeometryPostProcess::SinglePassPostProcess::GreyScalePostProcess::applyFilter()
{
	// Tell OpenGL which VBOs to use
	mpBoundingGeometry->GetArrayBuffer()->bind();
	mpBoundingGeometry->GetIndexBuffer()->bind();

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = mpProgram->attributeLocation("posAttr");
	mpProgram->enableAttributeArray(vertexLocation);
	mpProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

	// Draw cube geometry using indices from VBO 1
	glDrawElements(GL_TRIANGLE_STRIP, mpBoundingGeometry->GetIndexNumber(), GL_UNSIGNED_SHORT, 0);
}

void GeometryEngine::GeometryPostProcess::SinglePassPostProcess::GreyScalePostProcess::copy(const GreyScalePostProcess & ref)
{
	SinglePassPostProcess::copy(ref);
	initPostProcess();
}
