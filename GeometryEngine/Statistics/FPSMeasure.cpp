#include "../StatsManager.h"
#include "FPSMeasure.h"

GeometryEngine::GeometryStatistics::FPSMeasure::FPSMeasure(GeometryEngine::StatsManager * parent) : mpParent(nullptr), mNumFrames(0), mTimer(nullptr), mFrameTime(0.0)
{
	mpParent = parent;
	mTimer = new QTime();
	mTimer->start();
}

GeometryEngine::GeometryStatistics::FPSMeasure::~FPSMeasure()
{
	delete mTimer;
}

void GeometryEngine::GeometryStatistics::FPSMeasure::Update()
{
	++mNumFrames;
	int millis = mTimer->elapsed();
	if (millis >= 1000)
	{
		mFrameTime = millis / double(mNumFrames);
		mNumFrames = 0;
		mTimer->restart();

#ifdef _DEBUG
		printf("%f ms/frame\n", mFrameTime);
#endif // _DEBUG

	}
}
