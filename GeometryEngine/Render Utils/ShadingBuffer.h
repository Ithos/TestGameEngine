#pragma once

#ifndef GEOMETRYSHADINGBUFFER_H
#define GEOMETRYSHADINGBUFFER_H

#include <qvector2d.h>
#include "BufferUtils\GFramebufferObject.h"
#include "IBuffer.h"

namespace GeometryEngine
{
	class ShadingBufferTextureInfo;
	
	namespace GeometryBuffer
	{
		/// Class that manages framebuffer object for shading and color maps
		class ShadingBuffer : public IBuffer
		{
		public:
			/// Enum used to locate every texture used to store different information in the buffer
			enum SHADINGBUFFER_TEXTURE_TYPE
			{
				SHADINGBUFFER_TEXTURE_TYPE_DIFFUSE_MAP,
				SHADINGBUFFER_TEXTURE_TYPE_SPECULAR_MAP,
				SHADINGBUFFER_TEXTURE_TYPE_TRANSLUCENT_DEPTH_MAP,
				SHADINGBUFFER_TEXTURE_TYPE_SHADOW_MAP,
				SHADINGBUFFER_NUM_TEXTURES
			};

			/// Constructor
			ShadingBuffer();
			/// Copy constructor
			/// \param ref Const reference to ShadingBuffer to be copied
			ShadingBuffer(const ShadingBuffer& ref);
			/// Destructor. Clears framebuffers and textures.
			virtual ~ShadingBuffer();

			/// Creates framebuffers and textures
			/// \param MaxWindowWidth Max texture width
			/// \param MaxWindowHeight Max texture height
			/// \return true if ok false otherwise
			bool Init(unsigned int MaxWindowWidth, unsigned int MaxWindowHeight);
			/// Resizes textures
			/// \param WindowWidth New texture width
			/// \param WindowHeight New texture height
			/// \return true if ok false otherwise
			bool Resize(unsigned int WindowWidth, unsigned int WindowHeight);
			/// Clears textures color
			void StartFrame();
			/// Binds color map buffers buffers for drawing
			void BindColorMapsWrite();
			/// Binds color map buffers buffers for reading
			void BindColorMapsRead();
			/// Unbind all texture locations
			void ResetBindings();
			/// Unbind all buffer textures
			void UnbindBuffer();
			/// Binds the buffer in read mode and sets the selected texture as read
			/// \param texture index to be bound
			virtual void BindTextureRead(unsigned int index) override;
			/// Binds the buffer in draw mode and sets the selected texture as draw
			/// \param texture index to be bound
			virtual void BindTextureDraw(unsigned int index) override;
			/// Binds the buffer in draw-read mode and sets the selected texture as draw-read
			/// \param texture index to be bound
			virtual void BindTextureDrawRead(unsigned int index) override;
			/// Binds selected texture to its default texture unit
			/// \param index texture index to bind
			virtual void BindTexture(unsigned int index) override;
			/// Binds selected texture to the selected texture unit
			/// \param index texture index to bind
			/// \param textureUnit texture unit to bind the texture to
			virtual void BindTexture(unsigned int index, unsigned int textureUnit) override;
			/// Unbinds texture from its default texture unit
			/// \param index texture index to bind
			virtual void UnbindTexture(unsigned int index) override;
			/// Unbinds texture from the selected texture unit
			/// \param index texture index to bind
			/// \param textureUnit texture unit to bind the texture to
			virtual void UnbindTexture(unsigned int index, unsigned int textureUnit) override;
			/// Binds selected texture to its default texture unit
			/// \param tex texture to bind
			void BindTexture(SHADINGBUFFER_TEXTURE_TYPE tex);
			/// Binds selected texture to the selected texture unit
			/// \param tex texture to bind
			/// \param textureUnit texture unit to bind the texture to
			void BindTexture(SHADINGBUFFER_TEXTURE_TYPE tex, unsigned int textureUnit);
			/// Unbinds texture from its default texture unit
			/// \param tex texture to unbind
			void UnbindTexture(SHADINGBUFFER_TEXTURE_TYPE tex);
			/// Unbinds texture from the selected texture unit
			/// \param tex texture to unbind
			/// \param textureUnit texture unit to bind the texture to
			void UnbindTexture(SHADINGBUFFER_TEXTURE_TYPE tex, unsigned int textureUnit);
			/// Binds the buffer as Read/Write and the shadowMap texture for writing
			void BindShadowMapTextureWrite();
			/// Binds the buffer as Read/Write and the shadowMap texture for Reading
			void BindShadowMapTextureRead();
			/// Binds the buffer as Read/Write and the translucentDepthMAp texture for writing
			void BindTranslucentDepthMapWrite();
			/// Binds the buffer as Read/Write and the translucentDepthMAp texture for reading
			void BindTranslucentDepthMapRead();
			/// Gets current texture size
			/// \return Current texture size
			const QVector2D& GetTextureSize() const { return mTextureSize; }
			/// Gets the max texture size
			/// \return Max texture siza
			const QVector2D& GetMaxTextureSize() const { return mMaxTextureSize; }
			/// Clears the color of one of the textures of the framebuffer
			/// \param texture Texture to clear
			void ClearColorTexture(SHADINGBUFFER_TEXTURE_TYPE texture);
			/// Detaches the depth texture from the framebuffer and attaches the tmp depth buffer
			void DetachDepthTexture();
			/// Detaches the tmp depth buffer and ataches the depth texture to the framebuffer
			void AttachDepthTexture();
			/// Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual ShadingBuffer* Clone() const { return new ShadingBuffer(*this); };
			/// Sets the data of a TBufferTextureInfo
			/// \param bufferInfo TBufferTextureInfo to be updated
			void FillShadingBufferInfo(ShadingBufferTextureInfo& bufferInfo);
		protected:
			QVector2D mTextureSize;
			QVector2D mMaxTextureSize;
			GFramebufferObject* mpFBO;
			static const int mTextureUnits[SHADINGBUFFER_NUM_TEXTURES];
			/// Binds the buffer in the selected mode and sets the selected texture
			/// \param mode READ, WRITE or DRAW_READ
			/// \param tex index to be bound
			virtual void bindTextureMode(GeometryBuffer::G_FRAMEBUFFER_BINDS mode, unsigned int tex);
			/// Copies a data ShadingBuffer to the current object
			/// \param ref ShadingBuffer to be copied
			virtual void copy(const ShadingBuffer& ref);
		};
	}
}

#endif