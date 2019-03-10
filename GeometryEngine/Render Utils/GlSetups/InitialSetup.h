#pragma once
#ifndef INITIALSETUP_H
#define INITIALSETUP_H

#include <qvector4d.h>

#include "../GlSetup.h"

namespace GeometryEngine
{
	class InitialSetup : public GlSetup
	{
	public:
		InitialSetup() : GlSetup() {}
		virtual ~InitialSetup() {}

		virtual void StartSetup() override;
		virtual void FinishSetup() override;
		virtual void SetColor(const QVector4D& color) override { mClearColor.setX(color.x()); mClearColor.setY(color.y()); mClearColor.setZ(color.z()); mClearColor.setW(color.w()); }
	protected:
		QVector4D mClearColor;
	};
}

#endif
