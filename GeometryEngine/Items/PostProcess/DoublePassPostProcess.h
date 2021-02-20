#pragma once
#ifndef DOUBLEPASSPOSTPROCESS_H
#define DOUBLEPASSPOSTPROCESS_H

#include "PostProcess.h"

namespace GeometryEngine
{
	namespace GeometryPostProcess
	{
		///namespace for all post processes that need two render steps to be applied
		namespace DoublePassPostProcess
		{
			/// Abstract class that defines an interface forall postProcesses that require 2 steps
			class DoublePassPostProcess : public PostProcess
			{
			public:
				///Constructor
				/// \param boundingGeometry The post process will be applied to every part of the screen where the bounding geometry is drawn. Usually it should be a rectangle directly in front of the camera.
				/// \param customShading Custom shading step manager.
				/// \param componentManager Object that manages and contains the custom components for this postprocess
				/// \param iterations Number of iterations that this posprocess should be executed
				DoublePassPostProcess(const GeometryWorldItem::GeometryItem::GeometryItem & boundingGeometry, const CustomShading::CustomPostProcessStepInterface* const componentManager = nullptr,
					unsigned int iterations = 1) : PostProcess(boundingGeometry, componentManager, iterations) {}
				/// Copy constructor
				/// \param ref Const reference to PostProcess to be copied
				DoublePassPostProcess(const DoublePassPostProcess& ref) : PostProcess(ref) {};
				/// Destructor
				virtual ~DoublePassPostProcess() {}
				/// Method that applies the second render step of the post process
				/// \param gBuffTexInfo geometry buffer data
				virtual void ApplyPostProcessSecondStep(const GBufferTextureInfo& gBuffTexInfo) = 0;

			protected:
				/// Copies the data of a PostProcess object to the current object
				/// \param ref PostProcess to be copied
				virtual void copy(const PostProcess& ref) { PostProcess::copy(ref); }
			};
		}
	}
}

#endif