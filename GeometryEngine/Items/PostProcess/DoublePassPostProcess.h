#pragma once
#ifndef DOUBLEPASSPOSTPROCESS_H
#define DOUBLEPASSPOSTPROCESS_H

#include "PostProcess.h"

namespace GeometryEngine
{
	namespace GeometryPostProcess
	{
		namespace DoublePassPostProcess
		{
			class DoublePassPostProcess : public PostProcess
			{
			public:
				DoublePassPostProcess( const GeometryWorldItem::GeometryItem::GeometryItem & boundingGeometry) : PostProcess(boundingGeometry) {}
				DoublePassPostProcess(const DoublePassPostProcess& ref) : PostProcess(ref) {};
				virtual ~DoublePassPostProcess() {}
				virtual void ApplyFirstStep(const GBufferTextureInfo& gBuffTexInfo);
				virtual void ApplySecondStep(const GBufferTextureInfo& gBuffTexInfo);

			protected:
				virtual void SetFirstStepParameters() = 0;
				virtual void SetSecondStepParameters() = 0;
				virtual void copy(const PostProcess& ref) { PostProcess::copy(ref); }
			};
		}
	}
}

#endif