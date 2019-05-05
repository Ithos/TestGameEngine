#include "DoublePassPostProcess.h"

void GeometryEngine::GeometryPostProcess::DoublePassPostProcess::DoublePassPostProcess::ApplyFirstStep(const GBufferTextureInfo & gBuffTexInfo)
{
	SetFirstStepParameters();
	ApplyPostProcess(gBuffTexInfo);
}

void GeometryEngine::GeometryPostProcess::DoublePassPostProcess::DoublePassPostProcess::ApplySecondStep(const GBufferTextureInfo & gBuffTexInfo)
{
	SetSecondStepParameters();
	ApplyPostProcess(gBuffTexInfo);
}
