#pragma once
#ifndef COMPLETECOLORPOSTPROCESSBUFFER_H
#define COMPLETECOLORPOSTPROCESSBUFFER_H

#include "../GBuffer.h"

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		class CompleteColorPostProcessBuffer : public GBuffer
		{
		public:
			CompleteColorPostProcessBuffer();
			CompleteColorPostProcessBuffer(const CompleteColorPostProcessBuffer& ref);
			virtual ~CompleteColorPostProcessBuffer() {};
			virtual GBuffer* Clone() const override { return new CompleteColorPostProcessBuffer(*this); };
		protected:
			virtual void copy(const GBuffer& ref) override;
		};
	}
}

#endif