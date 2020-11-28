#pragma once
#ifndef GLSETUP_H
#define GLSETUP_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

namespace GeometryEngine
{
	///namespace for OpenGl setups
	namespace GeometryGlSetup
	{
		/// Abstract class that acts as an interface for all setups.
		/// Setup classes execute a set of OpenGl operations in order to configure the pipeline in a certain way.
		class GlSetup : protected QOpenGLExtraFunctions
		{
		public:
			/// Constructor
			GlSetup();
			/// Copy constructor
			/// \param ref Constant reference to GlSetup to be copied
			GlSetup(const GlSetup& ref) { copy(ref); }
			/// Destructor
			virtual ~GlSetup() {}

			/// Configure pipeline
			virtual void StartSetup() = 0;
			/// \return the pipeline to its initial state
			virtual void FinishSetup() = 0;
			/// Factory method that creates a copy of the object
			/// \return Pointer to a copy of the object
			virtual GlSetup* Clone() const = 0;

			/// Sets the GlSetup color parameter (If there is one)
			virtual void SetColor(const QVector4D& color) {}
		protected:
			/// Copies the values of a given GlSetup into this object
			/// \param ref GlSetup to be copied
			virtual void copy(const GlSetup& ref) {}
		};
	}
}

#endif
