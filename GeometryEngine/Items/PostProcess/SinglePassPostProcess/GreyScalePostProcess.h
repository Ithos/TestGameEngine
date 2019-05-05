#pragma once

#ifndef GREYSCALEPOSTPROCESS_H
#define GREYSCALEPOSTPROCESS_H

#include "..\SinglePassPostProcess.h"

namespace GeometryEngine
{
	namespace GeometryPostProcess
	{
		namespace SinglePassPostProcess
		{
			class GreyScalePostProcess : public SinglePassPostProcess
			{
			public:
				GreyScalePostProcess(const GeometryWorldItem::GeometryItem::GeometryItem& boundingGeometry);
				GreyScalePostProcess(const GreyScalePostProcess& ref) : SinglePassPostProcess(ref) { copy(ref); }
				virtual ~GreyScalePostProcess();

				virtual PostProcess* Clone() const override { return new GreyScalePostProcess(*this); }
			protected:
				virtual void initPostProcessShaders() override;
				virtual void setProgramParameters(const GBufferTextureInfo& gBuffTexInfo) override;
				virtual void applyFilter() override;
				virtual void copy(const GreyScalePostProcess& ref);
			};
		}
	}
}


#endif // !GREYSCALEPOSTPROCESS_H
