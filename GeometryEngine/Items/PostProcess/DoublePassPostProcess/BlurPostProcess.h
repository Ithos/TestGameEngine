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
			/// Adds gaussian blur to the image
			class BlurPostProcess : public DoublePassPostProcess
			{
			public:
				///Constructor
				///param boundingGeometry
				///param isVertical indicates if the vertical or horizontal blur pass should be applied
				BlurPostProcess( const GeometryWorldItem::GeometryItem::GeometryItem & boundingGeometry, bool isVertical = false);
				///Copy constructor
				///param ref Const reference to GreyScalePostProcess to be copied
				BlurPostProcess(const BlurPostProcess& ref);
				///Destructor
				virtual ~BlurPostProcess();

				bool GetVertical() const { return mIsVertical; }
				void SetVertical(bool IsVertical) { mIsVertical = IsVertical; }
				bool GetOrigin() const { return mSwapOriginTexture; }
				void SetOrigin(bool swapOrigin) { mSwapOriginTexture = swapOrigin; }
				///Factory method. Creates a copy of this object
				///return Pointer to a copy of this object
				virtual PostProcess* Clone() const override { return new BlurPostProcess(*this); }
			protected:
				///Stablishes shaders keys.
				virtual void initPostProcessShaders() override;
				///Sends parameters to the shaders.
				virtual void setProgramParameters(const GBufferTextureInfo& gBuffTexInfo) override;
				///Renders the filter
				virtual void applyFilter() override;
				///Sets class members to be used during the first render step.
				virtual void SetFirstStepParameters() override;
				///Sets class members to be used during the second render step.
				virtual void SetSecondStepParameters() override;
				/// Copies the data of a PostProcess object to the current object
				/// param ref PostProcess to be copied
				virtual void copy(const BlurPostProcess& ref);
				bool mIsVertical;
				bool mSwapOriginTexture;
			};
		}
	}
}

#endif