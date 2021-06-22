#pragma once

#ifndef BASEGEOMETRYTEST_H
#define BASEGEOMETRYTEST_H

#include "../TestConfiguration/TestConfig.h"

namespace GeometryEngine
{
	class GeometryEngine;

	namespace GeometryWorldItem
	{
		namespace GeometryItem
		{
			class GeometryItem;
		}

		namespace GeometryCamera
		{
			class Camera;
		}

		namespace GeometryLight
		{
			class Light;
		}
	}
}

namespace UnitTest
{
	/// Base abstract class for geometry engine tests
	class CBaseGeometryTest
	{
	public:
		/// Constructor
		CBaseGeometryTest() : mTestFinished(false), mTestSuccessful(false), mScenePainted(false), mSceneWidth(0), mSceneHeigth(0), mpConfManager(nullptr)
		{
			mpConfManager = TestConfigurationManager::GetInstance();
		};
		/// Copy constructor
		CBaseGeometryTest(const CBaseGeometryTest& ref) { copy(ref); }
		/// Destructor
		virtual ~CBaseGeometryTest() {};
		/// Init graphic geometry
		/// \param engine Geometry engine to be tested
		/// \param sceneWidth viewport width
		/// \param sceneHeight viewport height
		virtual void InitGeometry(GeometryEngine::GeometryEngine* engine, int sceneWidth, int sceneHeight) = 0;
		/// Update graphic geometry
		/// \param tick Elapsed time since last update in milliseconds
		virtual void UpdateGeometry(qint64 tick) = 0;
		/// Factory method. Creates a copy of this object. The copy is not initialized.
		virtual CBaseGeometryTest* Clone() const = 0;
		/// Returns if the test has finished
		/// \return Test finished
		virtual bool IsTestFinished() { return mTestFinished; }
		/// Returs if the test was successful
		/// \return Test successful
		virtual bool IsTestSuccessful() { return mTestSuccessful; }
		/// Set the state of the scene as painted. Most tests won't start until the scene has been painted to the window.
		/// \return painted 
		virtual void SetScenePainted(bool painted) { mScenePainted = painted; }

		/// Executes and evaluates the result of the test
		virtual void evaluate() = 0;

	protected:
		bool mTestFinished, mTestSuccessful, mScenePainted;
		int mSceneWidth;
		int mSceneHeigth;
		TestConfigurationManager* mpConfManager;

		/// Copies the state of other object into this. The copy is not initialized.
		virtual void copy(const CBaseGeometryTest& ref);
		

		virtual bool compareScreenImage(const std::string& testImagePath, const std::string& className);
	};
}

#endif // !BASEGEOMETRYTEST_H
