#include "BlurPostProcess.h"

GeometryEngine::GeometryPostProcess::DoublePassPostProcess::BlurPostProcess::BlurPostProcess( const GeometryWorldItem::GeometryItem::GeometryItem & boundingGeometry, bool isVertical) :
	DoublePassPostProcess(boundingGeometry), mIsVertical(isVertical)
{
	initPostProcess();
}

GeometryEngine::GeometryPostProcess::DoublePassPostProcess::BlurPostProcess::BlurPostProcess(const BlurPostProcess & ref) : DoublePassPostProcess(ref)
{
	copy(ref);
	initPostProcess();
}

GeometryEngine::GeometryPostProcess::DoublePassPostProcess::BlurPostProcess::~BlurPostProcess()
{
}

void GeometryEngine::GeometryPostProcess::DoublePassPostProcess::BlurPostProcess::initPostProcessShaders()
{
	mVertexShaderKey = GeometryEngine::GeometryPostProcess::PostProcessShaderConstants::POST_PROCESS_VERTEX_SHADER;
	mFragmentShaderKey = GeometryEngine::GeometryPostProcess::PostProcessShaderConstants::BLUR_POST_PROCESS_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryPostProcess::DoublePassPostProcess::BlurPostProcess::setProgramParameters(const GBufferTextureInfo & gBuffTexInfo)
{
	assert(mpProgram != nullptr && "Shading program not found");
	{
		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", mpBoundingGeometry->GetModelMatrix());
		unsigned int sourceTextureLocation = this->mSwapOriginTexture ? gBuffTexInfo.TmpTexture : gBuffTexInfo.FinalTexture;
		mpProgram->setUniformValue("mTexture", sourceTextureLocation);
		mpProgram->setUniformValue("mTextureSize", gBuffTexInfo.TextureSize);
		mpProgram->setUniformValue("isVertical", this->mIsVertical);
	}
}

void GeometryEngine::GeometryPostProcess::DoublePassPostProcess::BlurPostProcess::applyFilter()
{
	assert(mpProgram != nullptr && "Shading program not found");
	assert(mpBoundingGeometry != nullptr && "Bounding geometry not found");
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

		mpProgram->disableAttributeArray(vertexLocation);
	}
}

void GeometryEngine::GeometryPostProcess::DoublePassPostProcess::BlurPostProcess::SetFirstStepParameters()
{
	mIsVertical = false;
	mSwapOriginTexture = false;
}

void GeometryEngine::GeometryPostProcess::DoublePassPostProcess::BlurPostProcess::SetSecondStepParameters()
{
	mIsVertical = true;
	mSwapOriginTexture = true;
}

void GeometryEngine::GeometryPostProcess::DoublePassPostProcess::BlurPostProcess::copy(const BlurPostProcess & ref)
{
	DoublePassPostProcess::copy(ref);
	mIsVertical = ref.mIsVertical;
	mSwapOriginTexture = ref.mSwapOriginTexture;
}
