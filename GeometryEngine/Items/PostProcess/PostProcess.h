#pragma once

#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include "../GeometryItem.h"
#include "../CommonItemParameters.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryItem
		{
			class GeometryItem;
		}
	}

	namespace GeometryPostProcess
	{
		struct PostProcessShaderConstants
		{
			static const std::string POST_PROCESS_VERTEX_SHADER;
			static const std::string BLUR_POST_PROCESS_FRAGMENT_SHADER;
			static const std::string GREY_SCALE_PROCESS_FRAGMENT_SHADER;
		};

		class PostProcess
		{
		public:
			PostProcess(const GeometryWorldItem::GeometryItem::GeometryItem & boundingGeometry);
			PostProcess(const PostProcess& ref);
			virtual ~PostProcess();
			void ApplyPostProcess(const GBufferTextureInfo& gBuffTexInfo);
			virtual GeometryEngine::GeometryWorldItem::WorldItem* const GetBoundingGeometry() { return mpBoundingGeometry; }
			virtual PostProcess* Clone() const = 0;
		protected:
			GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem* mpBoundingGeometry;
			QOpenGLShaderProgram* mpProgram;
			Configuration::ConfigurationManager* mpConfInstance;
			ShaderFiles::ShaderManager* mpShaderManager;
			std::string mVertexShaderKey;
			std::string mFragmentShaderKey;

			void initPostProcess();
			virtual void initPostProcessProgram();
			virtual void initPostProcessShaders() = 0;
			virtual void setProgramParameters(const GBufferTextureInfo& gBuffTexInfo) = 0;
			virtual void applyFilter() = 0;
			virtual void copy(const PostProcess& ref);
		};
	}
}
#endif