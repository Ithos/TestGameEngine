#pragma once

#ifndef RENDERBUFFERDATA_H
#define RENDERBUFFERDATA_H

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		class GBuffer;
		class ShadingBuffer;
	}

	///Namespace for Render data classes
	namespace GeometryRenderData
	{
		///Class that stores the different render buffers to be used by the scene
		class RenderBuffersData
		{
		public:
			RenderBuffersData(const GeometryBuffer::GBuffer& geometryBuffer, const GeometryBuffer::ShadingBuffer* const shadingBuffer = nullptr);

			RenderBuffersData(const RenderBuffersData& ref) { copy(ref); }
			virtual ~RenderBuffersData();
			void Init(unsigned int MaxWindowWidth, unsigned int MaxWindowHeight);
			void Resize(unsigned int WindowWidth, unsigned int WindowHeight);

			GeometryBuffer::GBuffer* GetGeometryBuffer() { return mpGeometryBuffer; }
			GeometryBuffer::ShadingBuffer* GetShadingBuffer() { return mpShadingBuffer; }

			RenderBuffersData* Clone() const { return new RenderBuffersData( (*this) ); }
		protected:
			GeometryBuffer::GBuffer* mpGeometryBuffer;
			GeometryBuffer::ShadingBuffer* mpShadingBuffer;

			void copy(const RenderBuffersData& ref);
		};
	}
}

#endif