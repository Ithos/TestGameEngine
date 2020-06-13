#pragma once

#ifndef RENDERBUFFERDATA_H
#define RENDERBUFFERDATA_H

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		class GBuffer;
		class TranslucentBuffer;
	}

	///Namespace for Render data classes
	namespace GeometryRenderData
	{
		///Class that stores the different render buffers to be used by the scene
		class RenderBuffersData
		{
		public:
			RenderBuffersData(const GeometryBuffer::GBuffer& geometryBuffer, const GeometryBuffer::TranslucentBuffer* const translucentBuffer = nullptr);
			GeometryBuffer::GBuffer* GetGeometryBuffer() { return mpGeometryBuffer; }
			GeometryBuffer::TranslucentBuffer* GetTranslucentBuffer() { return mpTranslucentBuffer; }
		protected:
			GeometryBuffer::GBuffer* mpGeometryBuffer;
			GeometryBuffer::TranslucentBuffer* mpTranslucentBuffer;
		};
	}
}

#endif