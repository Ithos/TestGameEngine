#pragma once

#ifndef STATSMANAGER_H
#define STATSMANAGER_H

namespace GeometryEngine
{
	namespace GeometryStatistics
	{
		class FPSMeasure;
	}

	/// Singleton that manages staticstics of the engine
	class StatsManager
	{
	public:
		/// Returns the pointer to the singleton. It builds the object if the singleton hasn't been built before.
		static StatsManager* GetInstance();
		/// Destroys the pointer to the singleton.
		static void Release();
		/// Method that updates the statistics that depend on the graphic tick
		/// \param tick Time since last frame in seconds
		void OnTick(double tick);
		/// Returns the last measured number of FPS
		double GetFPS();
		/// Read the last measured frame time in milliseconds
		double GetFrameTime();

	protected:
		/// Constructor
		StatsManager();
		/// Destructor
		virtual ~StatsManager();

		static StatsManager* mpInstance;
		GeometryStatistics::FPSMeasure* mpFPSMeasure;

	};
}

#endif

