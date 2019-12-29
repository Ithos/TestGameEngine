#include "DoublePassPostProcess.h"

void GeometryEngine::GeometryPostProcess::DoublePassPostProcess::DoublePassPostProcess::ApplyPostProcess(const GBufferTextureInfo & gBuffTexInfo)
{
	SetFirstStepParameters();
	PostProcess::ApplyPostProcess(gBuffTexInfo);
}

bool GeometryEngine::GeometryPostProcess::DoublePassPostProcess::DoublePassPostProcess::ApplyPostProcessSecondStep(const GBufferTextureInfo & gBuffTexInfo)
{
	SetSecondStepParameters();
	PostProcess::ApplyPostProcess(gBuffTexInfo);
	return true;
}
