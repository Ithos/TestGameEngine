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
			/// Base class for all post processes that need two render steps to be applied
			class DoublePassPostProcess : public PostProcess
			{
			public:
				/// Constructor
				/// \param boundingGeometry The post process will be applied to every part of the screen where the bounding geometry is drawn. Usually it should be a rectangle directly in front of the camera.
				DoublePassPostProcess( const GeometryWorldItem::GeometryItem::GeometryItem & boundingGeometry) : PostProcess(boundingGeometry) {}
				/// Copy constructor
				/// \param ref Const reference to PostProcess to be copied
				DoublePassPostProcess(const DoublePassPostProcess& ref) : PostProcess(ref) {};
				/// Destructor
				virtual ~DoublePassPostProcess() {}
				/// Methods that applies the first render step of the post process
				/// \param gBuffTexInfo geometry buffer data
				virtual void ApplyPostProcess(const GBufferTextureInfo& gBuffTexInfo) override;
				/// Methods that applies the second render step of the post process
				/// \param gBuffTexInfo geometry buffer data
				virtual bool ApplyPostProcessSecondStep(const GBufferTextureInfo& gBuffTexInfo) override;

			protected:
				/// Abstract method. Sets class members to be used during the first render step.
				virtual void SetFirstStepParameters() = 0;
				/// Abstract method. Sets class members to be used during the second render step.
				virtual void SetSecondStepParameters() = 0;
				/// Copies the data of a PostProcess object to the current object
				/// \param ref PostProcess to be copied
				virtual void copy(const PostProcess& ref) { PostProcess::copy(ref); }
			};
		}
	}
}

#endif