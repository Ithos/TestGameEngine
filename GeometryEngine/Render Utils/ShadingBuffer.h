#pragma once

#ifndef GEOMETRYSHADINGBUFFER_H
#define GEOMETRYSHADINGBUFFER_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <qvector2d.h>

namespace GeometryEngine
{
	class ShadingBufferTextureInfo;
	
	namespace GeometryBuffer
	{
		class GFramebufferObject;
		/// Class that manages framebuffer object for shading and color maps
		class ShadingBuffer : protected QOpenGLExtraFunctions
		{
		public:
			/// Enum used to locate every texture used to store different information in the buffer
			enum SHADINGBUFFER_TEXTURE_TYPE
			{
				SHADINGBUFFER_TEXTURE_TYPE_DIFFUSE_MAP,
				SHADINGBUFFER_TEXTURE_TYPE_SPECULAR_MAP,
				SHADINGBUFFER_TEXTURE_TYPE_SHADOW_MAP,
				SHADINGBUFFER_TEXTURE_TYPE_TRANSLUCENT_DEPTH_MAP,
				SHADINGBUFFER_NUM_TEXTURES
			};

			/// Constructor
			ShadingBuffer();
			/// Copy constructor
			/// param ref Const reference to ShadingBuffer to be copied
			ShadingBuffer(const ShadingBuffer& ref);
			/// Destructor. Clears framebuffers and textures.
			virtual ~ShadingBuffer();

			/// Creates framebuffers and textures
			/// param MaxWindowWidth Max texture width
			/// param MaxWindowHeight Max texture height
			/// return true if ok false otherwise
			bool Init(unsigned int MaxWindowWidth, unsigned int MaxWindowHeight);
			/// Resizes textures
			/// param WindowWidth New texture width
			/// param WindowHeight New texture height
			/// return true if ok false otherwise
			bool Resize(unsigned int WindowWidth, unsigned int WindowHeight);
			/// Clears textures color
			void StartFrame();
			/// Binds color map buffers buffers
			void BindColorMaps();
			/// Unbind all texture locations
			void ResetBindings();
			/// Unbind all buffer textures
			void UnbindBuffer();
			/// Binds selected texture to its default texture unit
			/// param tex texture to bind
			void BindTexture(SHADINGBUFFER_TEXTURE_TYPE tex);
			/// Binds selected texture to the selected texture unit
			/// param tex texture to bind
			void BindTexture(SHADINGBUFFER_TEXTURE_TYPE tex, unsigned int textureUnit);
			/// Unbinds texture from its default texture unit
			/// param tex texture to unbind
			void UnbindTexture(SHADINGBUFFER_TEXTURE_TYPE tex);
			/// Unbinds texture from the selected texture unit
			/// param tex texture to unbind
			void UnbindTexture(SHADINGBUFFER_TEXTURE_TYPE tex, unsigned int textureUnit);
			/// Binds the buffer as Read/Write and the shadowMap texture for writing
			void BindShadowMapTextureWrite();
			/// Binds the buffer as Read/Write and the shadowMap texture for Reading
			void BindShadowMapTextureRead();
			/// Gets current texture size
			/// return Current texture size
			const QVector2D& GetTextureSize() const { return mTextureSize; }
			/// Gets the max texture size
			/// return Max texture siza
			const QVector2D& GetMaxTextureSize() const { return mMaxTextureSize; }
			/// Clears the color of one of the textures of the framebuffer
			/// param texture Texture to clear
			void ClearColorTexture(SHADINGBUFFER_TEXTURE_TYPE texture);
			/// Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual ShadingBuffer* Clone() const { return new ShadingBuffer(*this); };
			/// Sets the data of a TBufferTextureInfo
			/// param bufferInfo TBufferTextureInfo to be updated
			void FillShadingBufferInfo(ShadingBufferTextureInfo& bufferInfo);
		protected:
			QVector2D mTextureSize;
			QVector2D mMaxTextureSize;
			GFramebufferObject* mpFBO;
			static const int mTextureUnits[SHADINGBUFFER_NUM_TEXTURES];
			/// Copies a data ShadingBuffer to the current object
			/// param ref ShadingBuffer to be copied
			virtual void copy(const ShadingBuffer& ref);
		};
	}
}

#endif