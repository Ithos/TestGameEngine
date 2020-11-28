#pragma once
#ifndef INITIALSETUP_H
#define INITIALSETUP_H

#include <qvector4d.h>

#include "../GlSetup.h"

namespace GeometryEngine
{
	namespace GeometryGlSetup
	{
		/// Class that stablishes the initial state of the OpenGl pipeline
		class InitialSetup : public GlSetup
		{
		public:
			/// Constructor
			InitialSetup() : GlSetup() {}
			/// Copy Constructor
			/// \param ref Const reference to InitialSetup to be copied
			InitialSetup(const InitialSetup& ref) { copy(ref); }
			/// Destructor
			virtual ~InitialSetup() {}

			/// Initialize the OpenGl pipeline
			virtual void StartSetup() override;
			/// \return the OpenGl pipeline to its initial state
			virtual void FinishSetup() override;
			/// Set the OpenGl clear color the will be used
			/// \param color Qvector4D color+alpha in the form of a 4D vector
			virtual void SetColor(const QVector4D& color) override { mClearColor.setX(color.x()); mClearColor.setY(color.y()); mClearColor.setZ(color.z()); mClearColor.setW(color.w()); }
			/// Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual GlSetup* Clone() const { return new InitialSetup(*this); }
		protected:
			QVector4D mClearColor;
			/// Copies the data of an InitialSetup to the current object
			/// \param ref InitialSetup to be copied
			virtual void copy(const InitialSetup& ref);
		};
	}
}

#endif
