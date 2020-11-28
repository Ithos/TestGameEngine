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
			/// Translates color intensity of the scene to a greyscale
			class GreyScalePostProcess : public SinglePassPostProcess
			{
			public:
				///Constructor
				/// \param boundingGeometry
				GreyScalePostProcess(const GeometryWorldItem::GeometryItem::GeometryItem& boundingGeometry);
				///Copy constructor
				/// \param ref Const reference to GreyScalePostProcess to be copied
				GreyScalePostProcess(const GreyScalePostProcess& ref) : SinglePassPostProcess(ref) { copy(ref); }
				///Destructor
				virtual ~GreyScalePostProcess();
				///Factory method. Creates a copy of this object
				/// \return Pointer to a copy of this object
				virtual PostProcess* Clone() const override { return new GreyScalePostProcess(*this); }
			protected:
				///Stablishes shaders keys.
				virtual void initPostProcessShaders() override;
				///Sends parameters to the shaders.
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
