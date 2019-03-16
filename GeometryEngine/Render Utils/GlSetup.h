#pragma once
#ifndef GLSETUP_H
#define GLSETUP_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

namespace GeometryEngine
{
	namespace GeometryGlSetup
	{
		class GlSetup : protected QOpenGLExtraFunctions
		{
		public:
			GlSetup();
			GlSetup(const GlSetup& ref) { copy(ref); }
			virtual ~GlSetup() {}

			virtual void StartSetup() = 0;
			virtual void FinishSetup() = 0;
			virtual GlSetup* Clone() const = 0;

			virtual void SetColor(const QVector4D& color) {}
		protected:
			virtual void copy(const GlSetup& ref) {}
		};
	}
}

#endif
