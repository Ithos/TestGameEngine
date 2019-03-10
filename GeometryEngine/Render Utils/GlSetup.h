#pragma once
#ifndef GLSETUP_H
#define GLSETUP_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

namespace GeometryEngine
{
	class GlSetup : protected QOpenGLExtraFunctions
	{
	public:
		GlSetup();
		virtual ~GlSetup() {}

		virtual void StartSetup() = 0;
		virtual void FinishSetup() = 0;

		virtual void SetColor(const QVector4D& color) {}
	protected:
	};
}

#endif
