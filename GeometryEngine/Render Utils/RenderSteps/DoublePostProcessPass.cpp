#include "DoublePostProcessPass.h"

void GeometryEngine::GeometryRenderStep::DoublePostProcessPass::Render(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items, std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* lights)
{
	assert(cam != nullptr && "DoublePostProcessPass -> Camera not found");

	applyPostProcess(cam->GetGBuffer(), cam->GetPostProcess());
}

void GeometryEngine::GeometryRenderStep::DoublePostProcessPass::applyPostProcess(GeometryBuffer::GBuffer * buf, const std::list<GeometryPostProcess::PostProcess*> & postProcess)
{
	assert(buf != nullptr && "DoublePostProcessPass --> Geometry buffer not found");
	GBufferTextureInfo gbuff(buf->GetTextureSize());
	buf->FillGBufferInfo(gbuff);
	buf->BindTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	buf->BindFinalTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL, gbuff);

	glDisable(GL_DEPTH);
	glDisable(GL_DEPTH_TEST);

	for (auto it = postProcess.begin(); it != postProcess.end(); ++it)
	{
		GeometryEngine::GeometryPostProcess::DoublePassPostProcess::DoublePassPostProcess* doublePassProcess = 
			dynamic_cast<GeometryEngine::GeometryPostProcess::DoublePassPostProcess::DoublePassPostProcess*>((*it));

		if (doublePassProcess != nullptr)
		{
			initPostProcessPass(buf);
			doublePassProcess->ApplyFirstStep(gbuff);
			secondPostProcessPass(buf);
			doublePassProcess->ApplySecondStep(gbuff);
		}
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH);

	buf->UnbindFinalTexture();
	buf->UnbindTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
}

void GeometryEngine::GeometryRenderStep::DoublePostProcessPass::initPostProcessPass(GeometryBuffer::GBuffer * buf)
{
	assert(buf != nullptr && "DoublePostProcessPass --> Geometry buffer not found");
	buf->BindForPostProcess();
}

void GeometryEngine::GeometryRenderStep::DoublePostProcessPass::secondPostProcessPass(GeometryBuffer::GBuffer * buf)
{
	assert(buf != nullptr && "DoublePostProcessPass --> Geometry buffer not found");
	buf->BindForFinishPostProcess();
}
