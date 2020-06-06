#pragma once

#ifndef GEOMETRYGBUFFER_H
#define GEOMETRYGBUFFER_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <qvector2d.h>
#include <unordered_set>

namespace GeometryEngine
{
	class GBufferTextureInfo;
	///namespace for geometry buffers
	namespace GeometryBuffer
	{
		class GFramebufferObject;
		/// Class that manages the geometry buffer
		class GBuffer : protected QOpenGLExtraFunctions
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
			/// param ref Const reference to GBuffer to be copied
			GBuffer(const GBuffer& ref);
			/// Destructor. Clears framebuffers and textures.
			virtual ~GBuffer();

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
			/// param texture location to be bound
			void BindFinalTexture(GBUFFER_TEXTURE_TYPE location);
			/// Binds the final texture to the selected texture location and updates bufferInfo final texture
			/// param texture location to be bound
			/// param GBufferTextureInfo reference to be updated 
			void BindFinalTexture(GBUFFER_TEXTURE_TYPE location, GBufferTextureInfo& bufferInfo);
			/// Unbind all texture locations
			void ResetBindings();
			/// Unbind all buffer textures
			void UnbindBuffer();
			/// Unbinds the final texture from its location
			void UnbindFinalTexture();
			/// Binds selected texture to its location
			/// param tex texture to bind
			void BindTexture(GBUFFER_TEXTURE_TYPE tex);
			/// Unbinds texture
			/// param tex texture to unbind
			void UnbindTexture(GBUFFER_TEXTURE_TYPE tex);
			/// Gets current texture size
			/// return Current texture size
			const QVector2D& GetTextureSize() const { return mTextureSize; }
			/// Gets the max texture size
			/// return Max texture siza
			const QVector2D& GetMaxTextureSize() const { return mMaxTextureSize; }
			/// Returns true if the texture is active
			/// param texture Texture to check
			/// return true if active false otherwise
			bool IsTextureActive(GBUFFER_TEXTURE_TYPE texture) const { return mActiveTextures.find(texture) != mActiveTextures.end(); }
			/// Clears the color of one of the textures of the framebuffer
			/// param texture Texture to clear
			void ClearColorTexture(GBUFFER_TEXTURE_TYPE texture);
			/// Detaches the depth texture from the framebuffer and attaches the tmp depth buffer
			void DetachDepthBuffer();
			/// Detaches the tmp depth buffer and ataches the depth texture to the framebuffer
			void AttachDepthBuffer();
			/// Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual GBuffer* Clone() const { return new GBuffer(*this); };
			/// Sets the data of a GBufferTextureInfo
			/// param bufferInfo GBufferTextureInfo to be updated
			void FillGBufferInfo(GBufferTextureInfo& bufferInfo);

		protected:
			unsigned int mFinalTextureLocation;
			QVector2D mTextureSize;
			QVector2D mMaxTextureSize;
			std::unordered_set<GBUFFER_TEXTURE_TYPE> mActiveTextures;
			GFramebufferObject* mpFBO;
			/// Copies a data GBuffer to the current object
			/// param ref GBuffer to be copied
			virtual void copy(const GBuffer& ref);
		};
	}
}

#endif