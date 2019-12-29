#pragma once

#ifndef MAINOPENGLWINDOW
#define MAINOPENGLWINDOW


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <GeometryEngine.h>
#include <qmouseevent.h>
#include <qkeyevent.h>

namespace GeometryEngine {
	class GeometryEngine;

	namespace GeometryWorldItem
	{
		namespace GeometryItem
		{
			class Cube;
			class Sphere;
		}

		namespace GeometryCamera
		{
			class PerspectiveCamera;
		}

		namespace GeometryLight
		{
			class Light;
		}
	}
}

namespace Application
{

	class  CWindowApplication : public QOpenGLWidget, protected QOpenGLFunctions
	{
		Q_OBJECT
	public:
		explicit CWindowApplication(QWidget *parent = 0);
		~CWindowApplication();
	protected:
		void mousePressEvent(QMouseEvent *e) override;
		void mouseReleaseEvent(QMouseEvent *e) override;
		void keyPressEvent(QKeyEvent *e) override;
		void keyReleaseEvent(QKeyEvent *e) override;
		void timerEvent(QTimerEvent *e) override;

		void initializeGL() override;
		void resizeGL(int w, int h) override;
		void paintGL() override;

	private:

		void initGeometry(GeometryEngine::GeometryEngine* engine);

		QBasicTimer timer;
		GeometryEngine::GeometryEngine *mpGeomInstance;
		QVector2D mWindowSize;

		/// Items shouldnt be created here its just for testing purposes
		GeometryEngine::GeometryWorldItem::GeometryItem::Cube* testCube;
		GeometryEngine::GeometryWorldItem::GeometryItem::Sphere* testCube2;
		GeometryEngine::GeometryWorldItem::GeometryCamera::Camera* cam;
		GeometryEngine::GeometryWorldItem::GeometryLight::Light* mainLight;
		GeometryEngine::GeometryWorldItem::GeometryLight::Light* secondLight;
		GeometryEngine::GeometryWorldItem::GeometryItem::Cube* lightCube2;

		bool* mpMovementArray;

	};
}

#endif // MAINOPENGLWINDOW