#pragma once
#ifndef COMPLETECOLORPOSTPROCESSBUFFER_H
#define COMPLETECOLORPOSTPROCESSBUFFER_H

#include "../GBuffer.h"

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		/// Specialization of GBuffer that only includes specific textures, in this case all color textures (diffuse, ambient, reflective, emmissive), normal, position and tmp.
		/// Functionally identical to Complete color buffer, but intended to be used with the post process scene.
		class CompleteColorPostProcessBuffer : public GBuffer
		{
		public:
			/// Constructor
			CompleteColorPostProcessBuffer();
			/// Copy constructor
			/// \param ref Const reference to CompleteColorPostProcessBuffer to be copied
			CompleteColorPostProcessBuffer(const CompleteColorPostProcessBuffer& ref);
			/// Destructor
			virtual ~CompleteColorPostProcessBuffer() {};
			/// Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual GBuffer* Clone() const override { return new CompleteColorPostProcessBuffer(*this); };
		protected:
			/// Copies the data from a GBuffer into this object
			/// \param ref GBuffer to be copied
			virtual void copy(const GBuffer& ref) override;
		};
	}
}

#endif