#pragma once

#ifndef FPSMEASURE_H
#define FPSMEASURE_H

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
			/// Update the FPS measure
			/// \param tick Time since last frame in seconds
			void Update(double tick);
			/// Read the last measured frame time in milliseconds
			double GetFrameTime() { return mFrameTime; }
			/// Calculates the FPS value from the last measured frame time
			double GetFPS() { return 1000 / mFrameTime; }

		protected:
			GeometryEngine::StatsManager* mpParent;
			int mNumFrames;
			double mTime;
			double mFrameTime;

		};
	}
}

#endif // !FPSMEASURE_H
