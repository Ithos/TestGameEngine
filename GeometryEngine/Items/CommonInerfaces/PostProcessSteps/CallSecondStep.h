#pragma once

#ifndef GEOMETRYCALLSECONDSTEP_H
#define GEOMETRYCALLSECONDSTEP_H

#include "../CustomPostProcessStep.h"

namespace GeometryEngine
{
	namespace CustomShading
	{
		/// Calls the second step from the owner post process. The owner 
		template<class T>
		class CallSecondStep : public CustomPostProcessStep
		{
		public:
			/// Constructor
			/// \param parent Pointer to the CustomShadingInterface that contains it 
			/// \param Value that indicates at which render stepthis shading technique will be used
			CallSecondStep(CustomPostProcessStepInterface* parent, CustomPostProcessSteps step) : CustomPostProcessStep(parent, step) {};

			/// Copy constructor
			/// \param ref Object to be copied.
			CallSecondStep(const CustomPostProcessStep& ref) { copy(ref); }

			/// Destructor
			virtual ~CallSecondStep() {};

			/// Abstract method. Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual CallSecondStep* Clone(CustomPostProcessStepInterface* parent, CustomPostProcessSteps step) const {
				CallSecondStep* cloned = new CallSecondStep((*this));
				cloned->AddToInterface(parent, step);
				return cloned;
			}

		protected:
			/// Loads and compiles material shader program. In this component this does nothing.
			virtual void initProgram() override {}

			/// Renders the post process step
			/// \param gBuffTexInfo Geometry buffer data
			virtual void ApplyPostProcessStep(const GBufferTextureInfo& gBuffTexInfo)
			{
				setProgramParameters(gBuffTexInfo);
			}

			/// Sets the shaders that should be loaded. No shaders are loaded in this component so keys .
			virtual void initShaders() override
			{
				mVertexShaderKey = "";
				mFragmentShaderKey = "";
			}

			/// Sends parameters to the shaders. This component calls the ApplyPostProcessSecondStep method in the owner class that sets parameters and implements the rendering.
			/// \param gBuffTexInfo geometry buffer data
			virtual void setProgramParameters(const GBufferTextureInfo& gBuffTexInfo) override
			{
				assert(mpTargetProcess != nullptr && "Target process not found");
				{
					((T*)mpTargetProcess)->ApplyPostProcessSecondStep(gBuffTexInfo);
				}
			}

			/// Renders the PostProcess step. This step doesn't apply the filter, the implementation is in the owner class.
			virtual void applyFilter() override {}

		};

	}
}

#endif // !GEOMETRYCALLSECONDSTEP_H
