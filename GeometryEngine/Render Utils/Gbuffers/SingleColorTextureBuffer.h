#pragma once
#ifndef GEOMETRYSINGLECOLORTEXTBUFFER_H
#define GEOMETRYSINGLECOLORTEXTBUFFER_H

#include "../GBuffer.h"

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		/// Specialization of GBuffer that only includes the smallest functional amount of textures (diffuse color, positions, normal vectors, and tmp)
		class SingleColorTextBuffer : public GBuffer
		{
		public:
			/// Constructor
			SingleColorTextBuffer();
			/// Copy constructor
			/// \param ref Const reference to SingleColorTextBuffer to be copied
			SingleColorTextBuffer(const SingleColorTextBuffer& ref);
			/// Destructor
			virtual ~SingleColorTextBuffer() {};
			/// Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual GBuffer* Clone() const override { return new SingleColorTextBuffer(*this); };
		protected:
			/// Copies the data from a GBuffer into this object
			/// \param ref GBuffer to be copied
			virtual void copy(const GBuffer& ref) override;
		};
	}
}

#endif