#include "../Items/CommonItemParameters.h"
#include "Items\GraphicItems\Camera.h"
#include "Items\GraphicItems\Light.h"
#include "Items\GeometryItem.h"
#include "../Items/PostProcess/DoublePassPostProcess.h"
#include"../GBuffer.h"
#include "PostProcessPass.h"

void GeometryEngine::GeometryRenderStep::PostProcessPass::Render(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items, std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* lights)
{
	assert(cam != nullptr && "DoublePostProcessPass -> Camera not found");

	applyPostProcess(cam->GetGBuffer(), cam->GetPostProcess());
}

void GeometryEngine::GeometryRenderStep::PostProcessPass::applyPostProcess(GeometryBuffer::GBuffer * buf, const std::list<GeometryPostProcess::PostProcess*> & postProcess)
{
	assert(buf != nullptr && "DoublePostProcessPass --> Geometry buffer not found");
	GBufferTextureInfo gbuff(buf->GetTextureSize());
	buf->FillGBufferInfo(gbuff);
	buf->BindTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	buf->ClearColorTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	buf->BindFinalTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL, gbuff);

	glDisable(GL_DEPTH);
	glDisable(GL_DEPTH_TEST);

	for (auto it = postProcess.begin(); it != postProcess.end(); ++it)
	{
		GeometryEngine::GeometryPostProcess::PostProcess* postProcess = (*it);
		initPostProcessPass(buf);
		postProcess->ApplyPostProcess(gbuff);
		secondPostProcessPass(buf);
		if(	!postProcess->ApplyPostProcessSecondStep(gbuff) )
		{ 
			finishPostProcesPass(buf);
		}
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH);

	buf->UnbindFinalTexture();
	buf->UnbindTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
}

void GeometryEngine::GeometryRenderStep::PostProcessPass::initPostProcessPass(GeometryBuffer::GBuffer * buf)
{
	assert(buf != nullptr && "DoublePostProcessPass --> Geometry buffer not found");
	buf->BindTexturesForPostProcess();
}

void GeometryEngine::GeometryRenderStep::PostProcessPass::secondPostProcessPass(GeometryBuffer::GBuffer * buf)
{
	assert(buf != nullptr && "DoublePostProcessPass --> Geometry buffer not found");
	buf->FinishPostProcessBinding();
}

void GeometryEngine::GeometryRenderStep::PostProcessPass::finishPostProcesPass(GeometryBuffer::GBuffer* buf)
{
	assert(buf != nullptr && "FinalPass --> Geometry buffer not found");
	QVector2D texSize = buf->GetTextureSize();
	glBlitFramebuffer(
		0, 0, texSize.x(), texSize.y(),
		0, 0, texSize.x(), texSize.y(),
		GL_COLOR_BUFFER_BIT, GL_LINEAR
	);

}
