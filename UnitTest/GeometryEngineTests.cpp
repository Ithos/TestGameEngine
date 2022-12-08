#include "stdafx.h"
#include "CppUnitTest.h"

#include <QtWidgets/QApplication>

#include "TestWindow/TestWindow.h"

#include "TestWindow/TestImplementations/BasicSceneTest.h"
#include "TestWindow\TestImplementations\PostProcessTest.h"
#include "TestWindow\TestImplementations\ShadowSceneTest.h"
#include "TestWindow\TestImplementations\TranslucentShadowsTest.h"
#include "TestWindow\TestImplementations\TranslucentShadowingTest.h"
#include "TestWindow\TestImplementations\MultiViewportSceneTest.h"
#include "TestWindow\TestImplementations\GeometryPostProcessTest.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/// Namespace for tests and test classes
namespace UnitTest
{	
	/// Class that executes tests for the geometry engine
	TEST_CLASS(GeometryEngineTests)
	{
	public:
		
		/// Test constructor
		GeometryEngineTests() : mpApp(nullptr), mpSurface(nullptr)
		{
			int argc = 0;

			mpApp = new QApplication(argc, nullptr);
			mpApp->setApplicationName("GeometryEngineTests");
			mpApp->setApplicationVersion("0.1");

			mpSurface = new QSurfaceFormat();
			mpSurface->setDepthBufferSize(24);
			QSurfaceFormat::setDefaultFormat(*mpSurface);
		}

		/// Test destructor
		virtual ~GeometryEngineTests()
		{
			if (mpApp != nullptr) delete mpApp;
			if (mpSurface != nullptr) delete mpSurface;
		}

		/// Test for a basic illumination scene
		TEST_METHOD(BasicSceneTest)
		{
			executeSceneTest<CBasicSceneTest>();
		}

		/// Test for a scene with post processing
		TEST_METHOD(PostProcessSceneTest)
		{
			executeSceneTest<CPostProcessTest>();
		}

		/// Test for a scene with dynamic shadows
		TEST_METHOD(ShadowSceneTest)
		{
			executeSceneTest<CShadowSceneTest>();
		}

		/// Test for a scene with dynamic shadows
		TEST_METHOD(TranslucentShadowsSceneTest)
		{
			executeSceneTest<CTranslucentShadowsSceneTest>();
		}

		/// Test for a scene with dynamic translucent illumination and transparencies
		TEST_METHOD(TranslucentSceneTest)
		{
			executeSceneTest<CTranslucentShadowingTest>();
		}

		/// Test for a scene with multiple window viewports
		TEST_METHOD(MultiViewportSceneTest)
		{
			executeSceneTest<CMultiViewportSceneTest>();
		}

		/// Test for a scene that implements item post processes
		TEST_METHOD(GeometryPostProcessTest)
		{
			executeSceneTest<CGeometryPostProcessTest>();
		}

	protected:
		QApplication* mpApp;
		QSurfaceFormat* mpSurface;

		template<class T> void executeSceneTest()
		{
			CTestWindow* win = new CTestWindow(T());
			win->show();
			mpApp->exec();
			Assert::IsTrue(win->GetTestResult());
		}
	};
}