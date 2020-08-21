#pragma once

#ifndef TRANSLUCENTBUFFER_H
#define TRENSLUCENTBUFFER_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <qvector2d.h>

namespace GeometryEngine
{
	class TBufferTextureInfo;
	///namespace for geometry buffers
	namespace GeometryBuffer
	{
		class GFramebufferObject;
		/// Class that manages the geometry buffer
		class TranslucentBuffer : protected QOpenGLExtraFunctions
		{
		public:
			/// Enum used to locate every texture used to store different information in the geometry buffer
			enum TBUFFER_TEXTURE_TYPE
			{
				TBUFFER_TEXTURE_TYPE_DIFFUSE_MAP,
				TBUFFER_TEXTURE_TYPE_SPECULAR_MAP,
				TBUFFER_TEXTURE_TYPE_SHADOW_MAP,
				TBUFFER_TEXTURE_TYPE_TRANSLUCENT_DEPTH_MAP,
				TBUFFER_NUM_TEXTURES
			};

			/// Constructor
			TranslucentBuffer();
			/// Copy constructor
			/// param ref Const reference to GBuffer to be copied
			TranslucentBuffer(const TranslucentBuffer& ref);
			/// Destructor. Clears framebuffers and textures.
			virtual ~TranslucentBuffer();

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
			void BindTexture(TBUFFER_TEXTURE_TYPE tex);
			/// Binds selected texture to the selected texture unit
			/// param tex texture to bind
			void BindTexture(TBUFFER_TEXTURE_TYPE tex, unsigned int textureUnit);
			/// Unbinds texture from its default texture unit
			/// param tex texture to unbind
			void UnbindTexture(TBUFFER_TEXTURE_TYPE tex);
			/// Unbinds texture from the selected texture unit
			/// param tex texture to unbind
			void UnbindTexture(TBUFFER_TEXTURE_TYPE tex, unsigned int textureUnit);
			/// Bindsthe buffer as Read/Write and the shadowMap texture for writing
			void BindShadowMapTextureWrite();
			/// Gets current texture size
			/// return Current texture size
			const QVector2D& GetTextureSize() const { return mTextureSize; }
			/// Gets the max texture size
			/// return Max texture siza
			const QVector2D& GetMaxTextureSize() const { return mMaxTextureSize; }
			/// Clears the color of one of the textures of the framebuffer
			/// param texture Texture to clear
			void ClearColorTexture(TBUFFER_TEXTURE_TYPE texture);
			/// Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual TranslucentBuffer* Clone() const { return new TranslucentBuffer(*this); };
			/// Sets the data of a TBufferTextureInfo
			/// param bufferInfo TBufferTextureInfo to be updated
			void FillGBufferInfo(TBufferTextureInfo& bufferInfo);
		protected:
			QVector2D mTextureSize;
			QVector2D mMaxTextureSize;
			GFramebufferObject* mpFBO;
			/// Copies a data GBuffer to the current object
			/// param ref GBuffer to be copied
			virtual void copy(const TranslucentBuffer& ref);
		};
	}
}

#endif