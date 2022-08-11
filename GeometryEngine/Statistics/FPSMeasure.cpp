#include "../StatsManager.h"
#include <stdio.h>
#include "FPSMeasure.h"

GeometryEngine::GeometryStatistics::FPSMeasure::FPSMeasure(GeometryEngine::StatsManager * parent) : mpParent(nullptr), mNumFrames(0), mFrameTime(0.0), mTime(0.0)
{
	mpParent = parent;
}

void GeometryEngine::GeometryStatistics::FPSMeasure::Update(double tick)
{
	++mNumFrames;
	mTime += tick;
	if (mTime >= 1.0)
	{
		mFrameTime = mTime * 1000.0 / double(mNumFrames);
		mNumFrames = 0;
		mTime = 0.0;
#if defined( _DEBUG)
		printf("%f ms/frame\n", mFrameTime);
#endif // _DEBUG

	}
}
