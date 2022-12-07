#include <array>
#include "Items\GraphicItems\Camera.h"
#include "Items\GraphicItems\Light.h"
#include "Items\GeometryItem.h"
#include "../Items/Item Utils/Viewport.h"
#include "../Items/CommonItemParameters.h"
#include "../Items/PostProcess/DoublePassPostProcess.h"
#include "../Items/CommonInerfaces/MaterialPostProcessInterface.h"
#include "../Items/CommonInerfaces/MultiShadingInterface.h"
#include "Items\Materials\Material.h"
#include "GeometryPostProcessPass.h"

void GeometryEngine::GeometryRenderStep::GeometryPostProcessPass::Render(GeometryWorldItem::GeometryCamera::Camera * cam, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * orderedItems, 
	std::unordered_set<GeometryWorldItem::GeometryLight::Light*>* lights)
{
	assert(cam != nullptr && "GeometryPass --> No camera found");
	assert(orderedItems != nullptr && "GeometryPass --> No items list found");
	initStep();
	renderGeometry(cam, orderedItems);
	endStep();
}

void GeometryEngine::GeometryRenderStep::GeometryPostProcessPass::initStep()
{
	glDisable(GL_DEPTH);
}

void GeometryEngine::GeometryRenderStep::GeometryPostProcessPass::endStep()
{
	glEnable(GL_DEPTH);
}

void GeometryEngine::GeometryRenderStep::GeometryPostProcessPass::renderGeometry(GeometryWorldItem::GeometryCamera::Camera * cam, std::map<float, GeometryWorldItem::GeometryItem::GeometryItem*> * items)
{
	GeometryBuffer::GBuffer * buf = cam->GetGBuffer();
	assert(buf != nullptr && "GeometryPostProcessPass --> Geometry buffer not found");
	GBufferTextureInfo gbuff(buf->GetTextureSize());
	buf->FillGBufferInfo(gbuff);

	std::array<GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE, 4> locationArray = {
		GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_REFLECTIVE,
		GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_AMBIENT,
		GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_EMMISSIVE 
	};

	QVector4D viewport = cam->GetViewportSize();
	if (viewport.z() > 0 && viewport.w())
	{
		cam->GetViewport()->CalculateProjectionMatrix();

		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);

		for (auto itt = locationArray.begin(); itt != locationArray.end(); ++itt)
		{
			buf->BindTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
			buf->ClearColorTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
			GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE location = (*itt);

			buf->SetOriginTexture(gbuff, location);

			buf->UnbindTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
			buf->BindTexture(location);

			for (auto it = items->rbegin(); it != items->rend(); ++it)
			{
				GeometryWorldItem::GeometryItem::GeometryItem* item = (*it).second;
				if (!item->GetMaterialPtr()->IsTranslucent() && item->IsVisible() && item->GetMaterialPtr()->GetShadingInterface() != nullptr && checkRenderGroups(cam, item))
					drawItem(cam, buf, gbuff, item, location);
			}
			
			buf->UnbindTexture(location);
		}

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}

	
}

void GeometryEngine::GeometryRenderStep::GeometryPostProcessPass::drawItem(GeometryWorldItem::GeometryCamera::Camera * cam, GeometryBuffer::GBuffer* buf, const GBufferTextureInfo& gbuff, 
	GeometryWorldItem::GeometryItem::GeometryItem * item, const GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE & texture)
{
	CustomShading::MultiShadingInterface* sdInterf = item->GetMaterialPtr()->GetShadingInterface();
	CustomShading::MaterialPostProcessInterface* customSd = (sdInterf != nullptr && sdInterf->ContainsList(CustomShading::ShadingLists::POST_PROCESS_STEP_LIST)) ?
		sdInterf->GetList(CustomShading::ShadingLists::POST_PROCESS_STEP_LIST) : nullptr;

	CustomShading::MaterialPostProcessInterface* secondCustomSd = (sdInterf != nullptr && sdInterf->ContainsList(CustomShading::ShadingLists::SECOND_POST_PROCESS_STEP_LIST)) ?
		sdInterf->GetList(CustomShading::ShadingLists::SECOND_POST_PROCESS_STEP_LIST) : nullptr;

	if (customSd != nullptr)
	{
		for (auto it = customSd->GetSteps().begin(); it != customSd->GetSteps().end(); ++it)
		{
			for (int i = 0; i < customSd->GetStepIterations(it->first); ++i)
			{
				//Color informations is copied to an intermediate texture to apply the post process on it
				initPostProcessPass(buf, texture);
				copyBuffer(buf);

				customSd->RenderStep(it->first, item->GetArrayBuffer(), item->GetIndexBuffer(),
					cam->GetProjectionMatrix() * cam->GetViewMatrix() * item->GetModelMatrix(), item->GetVertexNumber(), item->GetIndexNumber(), gbuff);

				secondPostProcessPass(buf, texture);

				// The final result is always taken to the final texture
				if (secondCustomSd != nullptr && secondCustomSd->ContainsStep(it->first))
				{
					buf->UnbindTexture(texture);
					buf->BindTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
					secondCustomSd->RenderStep(it->first, item->GetArrayBuffer(), item->GetIndexBuffer(),
						cam->GetProjectionMatrix() * cam->GetViewMatrix() * item->GetModelMatrix(), item->GetVertexNumber(), item->GetIndexNumber(), gbuff);
					buf->UnbindTexture(GeometryEngine::GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
					buf->BindTexture(texture);
				}
				else
					copyBuffer(buf);
			}
		}
	}
}

void GeometryEngine::GeometryRenderStep::GeometryPostProcessPass::initPostProcessPass(GeometryBuffer::GBuffer * buf, const GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE & texture)
{
	assert(buf != nullptr && "initPostProcessPass --> Geometry buffer not found");
	buf->BindTextureForMaterialPostProcess(texture);
}

void GeometryEngine::GeometryRenderStep::GeometryPostProcessPass::secondPostProcessPass(GeometryBuffer::GBuffer * buf, const GeometryBuffer::GBuffer::GBUFFER_TEXTURE_TYPE & texture)
{
	assert(buf != nullptr && "DoublePostProcessPass --> Geometry buffer not found");
	buf->FinishMaterialPostProcess(texture);
}

void GeometryEngine::GeometryRenderStep::GeometryPostProcessPass::copyBuffer(GeometryBuffer::GBuffer * buf)
{
	assert(buf != nullptr && "copyBuffer --> Geometry buffer not found");
	QVector2D texSize = buf->GetTextureSize();
	glBlitFramebuffer(
		0, 0, texSize.x(), texSize.y(),
		0, 0, texSize.x(), texSize.y(),
		GL_COLOR_BUFFER_BIT, GL_LINEAR
	);
}
