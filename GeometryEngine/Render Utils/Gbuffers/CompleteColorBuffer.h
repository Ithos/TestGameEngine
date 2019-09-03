#pragma once
#ifndef COMPLETECOLORBUFFER_H
#define COMPLETECOLORBUFFER_H

#include "../GBuffer.h"

namespace GeometryEngine
{
	namespace GeometryBuffer
	{	
		/// Specialization of GBuffer that only includes specific textures, in this case all color textures (diffuse, ambient, reflective, emmissive), normal, position and tmp
		class CompleteColorBuffer : public GBuffer
		{
		public:
			/// Constructor
			CompleteColorBuffer();
			/// Copy constructor
			/// param ref Const reference to CompleteColorBuffer to be copied
			CompleteColorBuffer(const CompleteColorBuffer& ref);
			/// Destructor
			virtual ~CompleteColorBuffer() {};
			/// Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual GBuffer* Clone() const override { return new CompleteColorBuffer(*this); };
		protected:
			/// Copies the data from a GBuffer into this object
			/// param ref GBuffer to be copied
			virtual void copy(const GBuffer& ref) override;
		};
	}
}

#endif