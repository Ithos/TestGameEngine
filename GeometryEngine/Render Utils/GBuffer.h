#pragma once

#ifndef GEOMETRYGBUFFER_H
#define GEOMETRYGBUFFER_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <qvector2d.h>

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		class GBuffer : protected QOpenGLExtraFunctions
		{
		public:

			enum GBUFFER_TEXTURE_TYPE
			{
				GBUFFER_TEXTURE_TYPE_DIFFUSE,
				GBUFFER_TEXTURE_TYPE_POSITION,
				GBUFFER_TEXTURE_TYPE_NORMAL,
				GBUFFER_TEXTURE_TYPE_TEXCOORD,
				GBUFFER_NUM_TEXTURES
			};

			GBuffer();
			virtual ~GBuffer();

			bool Init(unsigned int MaxWindowWidth, unsigned int MaxWindowHeight);
			bool Resize(unsigned int WindowWidth, unsigned int WindowHeight);
			void StartFrame();
			void BindForGeomPass();
			void BindForStencilPass();
			void BindForLightPass();
			void BindForFinalPass();
			const QVector2D& GetTextureSize() const { return mTextureSize; }
			const QVector2D& GetMaxTextureSize() const { return mMaxTextureSize; }
			unsigned int GetTexture(GBUFFER_TEXTURE_TYPE texture) const { return mTextures[texture]; }

		protected:
			unsigned int mFbo;
			unsigned int mTextures[GBUFFER_NUM_TEXTURES];
			unsigned int mDepthTexture;
			unsigned int mFinalTexture;
			QVector2D mTextureSize;
			QVector2D mMaxTextureSize;
		};
	}
}

#endif