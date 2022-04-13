#include "Statistics\FPSMeasure.h"
#include "StatsManager.h"

GeometryEngine::StatsManager* GeometryEngine::StatsManager::mpInstance = nullptr;

GeometryEngine::StatsManager * GeometryEngine::StatsManager::GetInstance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new StatsManager();
	}

	return mpInstance;
}

void GeometryEngine::StatsManager::Release()
{
	if (mpInstance != nullptr)
	{
		delete mpInstance;
		mpInstance = nullptr;
	}
}

void GeometryEngine::StatsManager::OnTick()
{
	mpFPSMeasure->Update();
}

double GeometryEngine::StatsManager::GetFPS()
{
	return mpFPSMeasure->GetFPS();
}

double GeometryEngine::StatsManager::GetFrameTime()
{
	return mpFPSMeasure->GetFrameTime();
}

GeometryEngine::StatsManager::StatsManager() : mpFPSMeasure( new GeometryStatistics::FPSMeasure(this) )
{
}

GeometryEngine::StatsManager::~StatsManager()
{
	delete mpFPSMeasure;
}
