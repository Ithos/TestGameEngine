#pragma once

#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QElapsedTimer>
#include <QBasicTimer>
#include <QVector2D>

#include "TestImplementations/BaseGeometryTest.h"

namespace UnitTest
{
	/// Class that creates a window that contains the test OpenGL environment
	class CTestWindow : public QOpenGLWidget, protected QOpenGLFunctions
	{
	public:
		/// Constructor
		/// \param testImplementation Test class to be executed
		/// \param parent Pointer to the parent widget, nullptr by default
		explicit CTestWindow( const CBaseGeometryTest& testImplementation,  QWidget *parent = nullptr);
		/// Destructor
		virtual ~CTestWindow();

		/// Method to be executed every timer interval
		/// \param e Event sent by the timer
		void timerEvent(QTimerEvent *e) override;

		/// Callback that is executed after the OpenGl environment is initilized
		void initializeGL() override;
		/// Callback that is executed after resizing the window
		/// \param w New window width
		/// \param h New window height
		void resizeGL(int w, int h) override;

		/// Callback that is called when the window updates, here the openGl drawing should happen
		void paintGL() override;

		/// Gets the result of the test evaluation
		/// \return The result of the test
		bool GetTestResult() { return mpImplementation->IsTestSuccessful(); }

	protected:
		CBaseGeometryTest* mpImplementation;
		QElapsedTimer mElapsedTimer;
		QBasicTimer mTimer;
		GeometryEngine::GeometryEngine *mpGeomInstance;
		QVector2D mWindowSize;
	};
}

#endif // !TESTWINDOW_H
