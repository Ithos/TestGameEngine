#include "InitialSetup.h"

void GeometryEngine::GeometryGlSetup::InitialSetup::StartSetup()
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

void GeometryEngine::GeometryGlSetup::InitialSetup::FinishSetup()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void GeometryEngine::GeometryGlSetup::InitialSetup::copy(const InitialSetup & ref)
{
	GeometryGlSetup::GlSetup::copy(ref);
	this->mClearColor.setX(ref.mClearColor.x()); 
	this->mClearColor.setY(ref.mClearColor.y()); 
	this->mClearColor.setZ(ref.mClearColor.z()); 
	this->mClearColor.setW(ref.mClearColor.w());
}
