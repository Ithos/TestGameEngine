#pragma once

#ifndef SINGLEPASSPOSTPROCESS_H
#define SINGLEPASSPOSTPROCESS_H

#include "PostProcess.h"

namespace GeometryEngine
{
	namespace GeometryPostProcess
	{
		namespace SinglePassPostProcess
		{
			class SinglePassPostProcess : public PostProcess
			{
			public:
				SinglePassPostProcess(const GeometryWorldItem::GeometryItem::GeometryItem& boundingGeometry) : PostProcess(boundingGeometry) {}
				SinglePassPostProcess(const SinglePassPostProcess& ref) : PostProcess(ref) {}
				virtual ~SinglePassPostProcess() {}
			protected:
				virtual void copy(const PostProcess& ref) override { PostProcess::copy(ref); }
			};
		}
	}
}

#endif