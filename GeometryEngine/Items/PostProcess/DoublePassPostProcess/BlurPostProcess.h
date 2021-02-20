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
				/// \param boundingGeometry The post process will be applied to every part of the screen where the bounding geometry is drawn. Usually it should be a rectangle directly in front of the camera.
				/// \param customShading Custom shading step manager.
				/// \param componentManager Object that manages and contains the custom components for this postprocess
				/// \param iterations Number of iterations that this posprocess should be executed
				BlurPostProcess( const GeometryWorldItem::GeometryItem::GeometryItem & boundingGeometry, const CustomShading::CustomPostProcessStepInterface* const componentManager = nullptr, 
					unsigned int iterations = 1);
				///Copy constructor
				/// \param ref Const reference to GreyScalePostProcess to be copied
				BlurPostProcess(const BlurPostProcess& ref);
				///Destructor
				virtual ~BlurPostProcess();

				/// Methods that applies the first render step of the post process
				/// \param gBuffTexInfo geometry buffer data
				virtual void ApplyPostProcess(const GBufferTextureInfo& gBuffTexInfo) override;

				/// Method that applies the second render step of the post process
				/// \param gBuffTexInfo geometry buffer data
				virtual void ApplyPostProcessSecondStep(const GBufferTextureInfo& gBuffTexInfo);

				bool GetVertical() const { return mIsVertical; }
				void SetVertical(bool IsVertical) { mIsVertical = IsVertical; }
				bool GetOrigin() const { return mSwapOriginTexture; }
				void SetOrigin(bool swapOrigin) { mSwapOriginTexture = swapOrigin; }
				///Factory method. Creates a copy of this object
				/// \return Pointer to a copy of this object
				virtual PostProcess* Clone() const override { return new BlurPostProcess(*this); }
			protected:
				///Stablishes shaders keys.
				virtual void initPostProcessShaders() override;
				///Sends parameters to the shaders.
				/// \param gBuffTexInfo geometry buffer data
				virtual void setProgramParameters(const GBufferTextureInfo& gBuffTexInfo) override;
				///Renders the filter
				virtual void applyFilter() override;
				///Sets class members to be used during the first render step.
				virtual void SetFirstStepParameters();
				///Sets class members to be used during the second render step.
				virtual void SetSecondStepParameters();
				/// Copies the data of a PostProcess object to the current object
				/// \param ref PostProcess to be copied
				virtual void copy(const BlurPostProcess& ref);
				bool mIsVertical;
				bool mSwapOriginTexture;
			};
		}
	}
}

#endif