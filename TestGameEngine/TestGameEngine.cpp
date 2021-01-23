#ifndef QT_NO_OPENGL
#include "OpenGlQtApplication.h"
#endif

int main(int argc, char **argv)
{
	Application::COpenGlQtApplication app;

	app.QtOpenGlInit(argc, argv);

	return app.run();
}
