#pragma once
#ifndef COMPLETECOLORBUFFER_H
#define COMPLETECOLORBUFFER_H

#include "../GBuffer.h"

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		class CompleteColorBuffer : public GBuffer
		{
		public:
			CompleteColorBuffer();
			CompleteColorBuffer(const CompleteColorBuffer& ref);
			virtual ~CompleteColorBuffer() {};
			virtual GBuffer* Clone() const override { return new CompleteColorBuffer(*this); };
		protected:
			virtual void copy(const GBuffer& ref) override;
		};
	}
}

#endif