#pragma once

#ifndef FPSMEASURE_H
#define FPSMEASURE_H

#include <QTime>

namespace GeometryEngine
{
	class StatsManager;

	/// namespace that contains classes to calculate statistics in the engine
	namespace GeometryStatistics
	{
		/// Class that measures the FPS at which the engine is running
		class FPSMeasure
		{
		public:
			/// Constructor
			FPSMeasure(GeometryEngine::StatsManager* parent);
			/// Destructor
			virtual ~FPSMeasure();
			/// Update the FPS measure
			void Update();
			/// Read the last measured frame time in milliseconds
			double GetFrameTime() { return mFrameTime; }
			/// Calculates the FPS value from the last measured frame time
			double GetFPS() { return 1000 / mFrameTime; }

		protected:
			GeometryEngine::StatsManager* mpParent;
			int mNumFrames;
			QTime* mTimer;
			double mFrameTime;

		};
	}
}

#endif // !FPSMEASURE_H
