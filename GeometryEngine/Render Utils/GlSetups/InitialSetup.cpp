#include "InitialSetup.h"

void GeometryEngine::InitialSetup::StartSetup()
{
	glClearColor(mClearColor.x(), mClearColor.y(), mClearColor.z(), mClearColor.w());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	// Enable back face culling
	glEnable(GL_CULL_FACE);
}

void GeometryEngine::InitialSetup::FinishSetup()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}
