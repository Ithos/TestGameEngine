#pragma once

#ifndef GREYSCALEPOSTPROCESS_H
#define GREYSCALEPOSTPROCESS_H

#include "..\PostProcess.h"

namespace GeometryEngine
{
	namespace GeometryPostProcess
	{
		namespace SinglePassPostProcess
		{
			/// Translates color intensity of the scene to a greyscale
			class GreyScalePostProcess : public PostProcess
			{
			public:
				///Constructor
				/// \param boundingGeometry The post process will be applied to every part of the screen where the bounding geometry is drawn. Usually it should be a rectangle directly in front of the camera.
				/// \param customShading Custom shading step manager.
				/// \param componentManager Object that manages and contains the custom components for this postprocess
				/// \param iterations Number of iterations that this posprocess should be executed
				GreyScalePostProcess(const GeometryWorldItem::GeometryItem::GeometryItem& boundingGeometry, const CustomShading::CustomPostProcessStepInterface* const componentManager = nullptr, 
					unsigned int iterations = 1);
				///Copy constructor
				/// \param ref Const reference to GreyScalePostProcess to be copied
				GreyScalePostProcess(const GreyScalePostProcess& ref) : PostProcess(ref) { copy(ref); }
				///Destructor
				virtual ~GreyScalePostProcess();
				///Factory method. Creates a copy of this object
				/// \return Pointer to a copy of this object
				virtual PostProcess* Clone() const override { return new GreyScalePostProcess(*this); }
			protected:
				///Stablishes shaders keys.
				virtual void initPostProcessShaders() override;
				///Sends parameters to the shaders.
				/// \param gBuffTexInfo geometry buffer data
				virtual void setProgramParameters(const GBufferTextureInfo& gBuffTexInfo) override;
				///Renders the filter
				virtual void applyFilter() override;
				/// Copies the data of a PostProcess object to the current object
				/// \param ref PostProcess to be copied
				virtual void copy(const GreyScalePostProcess& ref);
			};
		}
	}
}


#endif // !GREYSCALEPOSTPROCESS_H
