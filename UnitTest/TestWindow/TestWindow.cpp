#include "stdafx.h"
#include <qcoreevent.h>
#include <GeometryEngine.h>
#include <SceneManager.h>
#include <GeometryScene.h>
#include "TestWindow.h"


UnitTest::CTestWindow::CTestWindow(const CBaseGeometryTest & testImplementation, QWidget * parent) :
	QOpenGLWidget(parent),
	mpGeomInstance(nullptr),
	mpImplementation(nullptr)
{
	mpImplementation = testImplementation.Clone();
	mWindowSize = QVector2D(this->width(), this->height());
}

UnitTest::CTestWindow::~CTestWindow()
{
	// Make sure the context is current when deleting the texture
	// and the buffers.
	makeCurrent();
	GeometryEngine::GeometryEngine::Release();
	doneCurrent();

	if (mpImplementation != nullptr)
	{
		delete mpImplementation;
	}
}

void UnitTest::CTestWindow::timerEvent(QTimerEvent * e)
{
	qint64 tick = mElapsedTimer.elapsed();
	mElapsedTimer.restart();
	mpImplementation->UpdateGeometry(tick);
	update();
}

void UnitTest::CTestWindow::initializeGL()
{
	mpGeomInstance = GeometryEngine::GeometryEngine::GetInstance();
	mpImplementation->InitGeometry(mpGeomInstance, mWindowSize.x(), mWindowSize.y());
	mTimer.start(12, this);
	mElapsedTimer.start();
}

void UnitTest::CTestWindow::resizeGL(int w, int h)
{
	GeometryEngine::GeometryScene::GeometryScene * scene = mpGeomInstance->GetSceneManager()->GetActiveScene();

	if (scene != nullptr)
	{
		scene->ResizeScene(w, h, mWindowSize.x(), mWindowSize.y());
		mWindowSize.setX(w); mWindowSize.setY(h);
	}
}

void UnitTest::CTestWindow::paintGL()
{
	// Draw cube geometry
	GeometryEngine::GeometryScene::GeometryScene * scene = mpGeomInstance->GetSceneManager()->GetActiveScene();
	if (scene != nullptr)
	{
		scene->Draw();
		mpImplementation->SetScenePainted(this->isVisible());
	}
	// Close the window if the test has finished
	if(mpImplementation->IsTestFinished())this->close();
}
