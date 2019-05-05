#pragma once

#ifndef BLURPOSTPROCESS_H
#define BLURPOSTPROCESS_H

#include "..\DoublePassPostProcess.h"

namespace GeometryEngine
{
	namespace GeometryPostProcess
	{
		namespace DoublePassPostProcess
		{
			class BlurPostProcess : public DoublePassPostProcess
			{
			public:
				BlurPostProcess( const GeometryWorldItem::GeometryItem::GeometryItem & boundingGeometry, bool isVertical = false);
				BlurPostProcess(const BlurPostProcess& ref);
				virtual ~BlurPostProcess();

				bool GetVertical() const { return mIsVertical; }
				void SetVertical(bool IsVertical) { mIsVertical = IsVertical; }
				bool GetOrigin() const { return mSwapOriginTexture; }
				void SetOrigin(bool swapOrigin) { mSwapOriginTexture = swapOrigin; }
				virtual PostProcess* Clone() const override { return new BlurPostProcess(*this); }
			protected:
				virtual void initPostProcessShaders() override;
				virtual void setProgramParameters(const GBufferTextureInfo& gBuffTexInfo) override;
				virtual void applyFilter() override;
				virtual void SetFirstStepParameters() override;
				virtual void SetSecondStepParameters() override;
				virtual void copy(const BlurPostProcess& ref);
				bool mIsVertical;
				bool mSwapOriginTexture;
			};
		}
	}
}

#endif