#pragma once

#ifndef SINGLEPASSPOSTPROCESS_H
#define SINGLEPASSPOSTPROCESS_H

#include "PostProcess.h"

namespace GeometryEngine
{
	namespace GeometryPostProcess
	{
		///namespace for all post processes that need only one render step to be applied
		namespace SinglePassPostProcess
		{
			/// Base class for all post processes that need only one render step to be applied
			class SinglePassPostProcess : public PostProcess
			{
			public:
				/// Constructor
				/// param boundingGeometry The post process will be applied to every part of the screen where the bounding geometry is drawn. Usually it should be a rectangle directly in front of the camera.
				SinglePassPostProcess(const GeometryWorldItem::GeometryItem::GeometryItem& boundingGeometry) : PostProcess(boundingGeometry) {}
				/// Copy constructor
				/// param ref Const reference to PostProcess to be copied
				SinglePassPostProcess(const SinglePassPostProcess& ref) : PostProcess(ref) {}
				/// Destructor
				virtual ~SinglePassPostProcess() {}
			protected:
				/// Copies the data of a PostProcess object to the current object
				/// param ref PostProcess to be copied
				virtual void copy(const PostProcess& ref) override { PostProcess::copy(ref); }
			};
		}
	}
}

#endif