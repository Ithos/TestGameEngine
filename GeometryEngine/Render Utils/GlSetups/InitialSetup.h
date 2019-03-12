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
		InitialSetup(const InitialSetup& ref) { copy(ref); }
		virtual ~InitialSetup() {}

		virtual void StartSetup() override;
		virtual void FinishSetup() override;
		virtual void SetColor(const QVector4D& color) override { mClearColor.setX(color.x()); mClearColor.setY(color.y()); mClearColor.setZ(color.z()); mClearColor.setW(color.w()); }
		virtual GlSetup* Clone() const { return new InitialSetup(*this); }
	protected:
		QVector4D mClearColor;

		virtual void copy(const InitialSetup& ref);
	};
}

#endif
