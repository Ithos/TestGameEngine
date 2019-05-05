#include "SinglePostProcessPass.h"

void GeometryEngine::GeometryRenderStep::SinglePostProcessPass::Render(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items, std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* lights)
{
	assert(cam != nullptr && "SinglePostProcessPass -> Camera not found");
	GeometryBuffer::GBuffer* buf = cam->GetGBuffer();
	applyPostProcess(buf, cam->GetPostProcess());
}

void GeometryEngine::GeometryRenderStep::SinglePostProcessPass::applyPostProcess(GeometryBuffer::GBuffer* buf, std::list< GeometryPostProcess::PostProcess*> postProcess)
{
	assert(buf != nullptr && "SinglePostProcessPass --> Geometry buffer not found");

	GBufferTextureInfo gbuff(buf->GetTextureSize());
	buf->FillGBufferInfo(gbuff);
	buf->BindFinalTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_POSITION, gbuff);
	glDisable(GL_DEPTH);
	glDisable(GL_DEPTH_TEST);

	for (auto it = postProcess.begin(); it != postProcess.end(); ++it)
	{
		GeometryEngine::GeometryPostProcess::SinglePassPostProcess::SinglePassPostProcess* singlePassProcess = 
			dynamic_cast<GeometryEngine::GeometryPostProcess::SinglePassPostProcess::SinglePassPostProcess*>((*it));
		if (singlePassProcess != nullptr)
		{
			initPostProcessPass(buf);
			singlePassProcess->ApplyPostProcess(gbuff);
			finishPostProcesPass(buf);
		}
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH);
	buf->UnbindFinalTexture();
}

void GeometryEngine::GeometryRenderStep::SinglePostProcessPass::initPostProcessPass(GeometryBuffer::GBuffer* buf)
{
	assert(buf != nullptr && "FinalPass --> Geometry buffer not found");
	buf->BindForPostProcess();
}

void GeometryEngine::GeometryRenderStep::SinglePostProcessPass::finishPostProcesPass(GeometryBuffer::GBuffer* buf)
{
	assert(buf != nullptr && "FinalPass --> Geometry buffer not found");

	buf->BindForFinishPostProcess();
	QVector2D texSize = buf->GetTextureSize();
	glBlitFramebuffer(
		0, 0, texSize.x(), texSize.y(),
		0, 0, texSize.x(), texSize.y(),
		GL_COLOR_BUFFER_BIT, GL_LINEAR
	);

}
