#pragma once

#ifndef GEOMETRYGBUFFER_H
#define GEOMETRYGBUFFER_H

#include <qvector2d.h>
#include <unordered_set>
#include "BufferUtils\GFramebufferObject.h"
#include "IBuffer.h"

namespace GeometryEngine
{
	class GBufferTextureInfo;
	///namespace for buffer clases
	namespace GeometryBuffer
	{
		/// Class that manages a framebuffer object for geometry, lighting and post process effects
		class GBuffer : public IBuffer
		{
		public:
			/// Enum used to locate every texture used to store different information in the geometry buffer
			enum GBUFFER_TEXTURE_TYPE
			{
				GBUFFER_TEXTURE_TYPE_DIFFUSE,
				GBUFFER_TEXTURE_TYPE_AMBIENT,
				GBUFFER_TEXTURE_TYPE_REFLECTIVE,
				GBUFFER_TEXTURE_TYPE_EMMISSIVE,
				GBUFFER_TEXTURE_TYPE_POSITION,
				GBUFFER_TEXTURE_TYPE_NORMAL,
				GBUFFER_TEXTURE_TYPE_TEXCOORD,
				GBUFFER_NUM_TEXTURES
			};

			/// Constructor
			GBuffer();
			/// Copy constructor
			/// \param ref Const reference to GBuffer to be copied
			GBuffer(const GBuffer& ref);
			/// Destructor. Clears framebuffers and textures.
			virtual ~GBuffer();

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
			/// Binds framebuffers and draw buffers
			void BindForGeomPass();
			/// Disables draw buffers
			void BindForStencilPass();
			/// Binds the final texture for writing and all others for reading
			void BindForLightPass();
			/// Binds final texture and the framebuffer for reading and the window for writing
			void BindForFinalPass();
			/// Binds final texture for reading and tmpTexture for writing
			void BindTexturesForPostProcess();
			/// Binds tmpTexture texture for reading and final for writing
			void FinishPostProcessBinding();
			/// Binds final texture for reading and tmpTexture for writing
			void BindTmpTextureWrite();
			/// Binds all textures
			void BindBuffer();
			/// Binds the final texture to the selected texture location
			/// \param texture location to be bound
			void BindFinalTexture(GBUFFER_TEXTURE_TYPE location);
			/// Binds the final texture to the selected texture location and updates bufferInfo final texture
			/// \param texture location to be bound
			/// \param GBufferTextureInfo reference to be updated 
			void BindFinalTexture(GBUFFER_TEXTURE_TYPE location, GBufferTextureInfo& bufferInfo);
			/// Unbind all texture locations
			void ResetBindings();
			/// Unbind all buffer textures
			void UnbindBuffer();
			/// Unbinds the final texture from its location
			void UnbindFinalTexture();
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
			void BindTexture(GBUFFER_TEXTURE_TYPE tex);
			/// Binds selected texture to the selected texture unit
			/// \param tex texture to bind
			/// \param textureUnit texture unit to bind the texture to
			void BindTexture(GBUFFER_TEXTURE_TYPE tex, unsigned int textureUnit);
			/// Unbinds texture from its default texture unit
			/// \param tex texture to unbind
			void UnbindTexture(GBUFFER_TEXTURE_TYPE tex);
			/// Unbinds texture from the selected texture unit
			/// \param tex texture to unbind
			/// \param textureUnit texture unit to bind the texture to
			void UnbindTexture(GBUFFER_TEXTURE_TYPE tex, unsigned int textureUnit);
			/// Gets current texture size
			/// \return Current texture size
			const QVector2D& GetTextureSize() const { return mTextureSize; }
			/// Gets the max texture size
			/// \return Max texture siza
			const QVector2D& GetMaxTextureSize() const { return mMaxTextureSize; }
			/// Returns true if the texture is active
			/// \param texture Texture to check
			/// \return true if active false otherwise
			bool IsTextureActive(GBUFFER_TEXTURE_TYPE texture) const { return mActiveTextures.find(texture) != mActiveTextures.end(); }
			/// Clears the color of one of the textures of the framebuffer
			/// \param texture Texture to clear
			void ClearColorTexture(GBUFFER_TEXTURE_TYPE texture);
			/// Detaches the depth texture from the framebuffer and attaches the tmp depth buffer
			void DetachDepthTexture();
			/// Detaches the tmp depth buffer and ataches the depth texture to the framebuffer
			void AttachDepthTexture();
			/// Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual GBuffer* Clone() const { return new GBuffer(*this); };
			/// Sets the data of a GBufferTextureInfo
			/// \param bufferInfo GBufferTextureInfo to be updated
			void FillGBufferInfo(GBufferTextureInfo& bufferInfo);

		protected:
			unsigned int mFinalTextureLocation;
			QVector2D mTextureSize;
			QVector2D mMaxTextureSize;
			std::unordered_set<GBUFFER_TEXTURE_TYPE> mActiveTextures;
			GFramebufferObject* mpFBO;
			/// Binds the buffer in the selected mode and sets the selected texture
			/// \param mode READ, WRITE or DRAW_READ
			/// \param tex index to be bound
			virtual void bindTextureMode(GeometryBuffer::G_FRAMEBUFFER_BINDS mode, unsigned int tex);
			/// Copies a data GBuffer to the current object
			/// \param ref GBuffer to be copied
			virtual void copy(const GBuffer& ref);
			static const int mTextureUnits[GBUFFER_NUM_TEXTURES + 1];
		};
	}
}

#endif