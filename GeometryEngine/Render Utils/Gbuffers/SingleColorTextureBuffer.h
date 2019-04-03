#pragma once
#ifndef GEOMETRYSINGLECOLORTEXTBUFFER_H
#define GEOMETRYSINGLECOLORTEXTBUFFER_H

#include "../GBuffer.h"

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		class SingleColorTextBuffer : public GBuffer
		{
		public:
			SingleColorTextBuffer();
			SingleColorTextBuffer(const SingleColorTextBuffer& ref);
			virtual ~SingleColorTextBuffer() {};
			virtual GBuffer* Clone() const override { return new SingleColorTextBuffer(*this); };
		protected:
			virtual void copy(const GBuffer& ref) override;
		};
	}
}

#endif