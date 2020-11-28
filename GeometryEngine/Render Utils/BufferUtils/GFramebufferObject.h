#pragma once

#ifndef GFRAMEBUFFEROBJECT_H
#define GFRAMEBUFFEROBJECT_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "FramebufferElements/GFramebufferCommmons.h"

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		class GFramebufferManager;
		class GRenderTarget;
		class GDepthStencilTexture;
		class GDepthStencilRenderBuffer;

		enum G_FRAMEBUFFER_BINDS
		{
			DETACH = GL_NONE,
			DRAW = GL_DRAW_FRAMEBUFFER,
			READ = GL_READ_FRAMEBUFFER,
			DRAW_READ = GL_FRAMEBUFFER
		};

		class GFramebufferObject : protected QOpenGLExtraFunctions
		{
		public:
			///Default constructor. Generates a framebuffer index and the default bind state is DETACH
			GFramebufferObject(GFramebufferManager* owner);
			///Destructor
			~GFramebufferObject();
			/// Adds a null texture attached to an OpenGl color attachment if the Framebuffer is attached
			/// \parameter attachment Attachment where the texture will be attached
			/// \return true if the FBO is bound false if it isn't
			bool AddTexture(GFramebufferCommons::G_COLOR_ATTACHMENTS attachment);
			/// Adds a color texture attached to an OpenGl color attachment if the Framebuffer is attached
			/// \parameter textureSize Size of the texture
			/// \parameter attachment Attachment where the texture will be attached
			/// \return true if the FBO is bound false if it isn't
			bool AddTexture(const QVector2D& textureSize, GFramebufferCommons::G_COLOR_ATTACHMENTS attachment);
			/// Adds a color texture attached to an OpenGl color attachment if the Framebuffer is attached
			/// \parameter textureSize Size of the texture
			/// \parameter borderColor Border color of the texture
			/// \parameter attachment Attachment where the texture will be attached
			/// \return true if the FBO is bound false if it isn't
			bool AddTexture(const QVector2D& textureSize, const QVector3D& borderColor, GFramebufferCommons::G_COLOR_ATTACHMENTS attachment);
			/// Adds a color buffer attached to an OpenGl color attachment if the Framebuffer is attached
			/// \parameter bufferSize Size of the buffer
			/// \parameter attachment Attachment where the texture will be attached
			/// \return true if the FBO is bound false if it isn't
			bool AddBuffer(const QVector2D& bufferSize, GFramebufferCommons::G_COLOR_ATTACHMENTS attachment);
			/// Creates a new depth texture if the FBO is attached. If a depth texture already exists it's destroyed and replaced by a new one
			/// \parameter textureSize Size of the texture
			/// \parameter attachment Attachment where the texture will be attached
			/// \return true if the FBO is bound false if it isn't
			bool SetStencilDepthTexture(const QVector2D& textureSize, GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS attachment);
			/// Creates a new depth buffer if the FBO is attached. If a depth buffer already exists it's destroyed and replaced by a new one
			/// \parameter bufferSize Size of the buffer
			/// \parameter attachment Attachment where the texture will be attached
			/// \return true if the FBO is bound false if it isn't
			bool SetStencilDepthBuffer(const QVector2D& bufferSize, GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS);
			/// Returs a pointer to a render target in the framebuffer
			/// \param index Index of the render target in the list
			/// \return pointer to a render target in the framebuffer or null if the index is outside of the inner list
			GRenderTarget* GetColorTarget(unsigned int index);
			/// Returs a pointer to the stencil/depth texture in the framebuffer or null if it doesn`t have one
			GRenderTarget* GetStencilDepthTexture() { return (GRenderTarget*)mpDepthTexture; }
			/// Returs a pointer to the stencil/depth buffer in the framebuffer or null if it doesn`t have one
			GRenderTarget* GetStencilDepthBuffer() { return (GRenderTarget*)mpDepthBuffer; }
			/// Binds the FBO
			/// \param bind type of binding to be used
			void Bind(G_FRAMEBUFFER_BINDS bind);
			/// Clears the OpenGl framebuffer bind
			void Unbind();
			/// Returns true if the FBO is bound
			bool IsBound() const { return mFramebufferState != G_FRAMEBUFFER_BINDS::DETACH; }
			/// Returns whether the binding state of the framebuffer allows reading
			bool IsReadState() { return (mFramebufferState == G_FRAMEBUFFER_BINDS::READ || mFramebufferState == G_FRAMEBUFFER_BINDS::DRAW_READ); }
			/// Returns whether the binding state of the framebuffer allows drawing
			bool IsDrawState() { return (mFramebufferState == G_FRAMEBUFFER_BINDS::DRAW || mFramebufferState == G_FRAMEBUFFER_BINDS::DRAW_READ); };
			/// Specify which color buffers are to be drawn into
			/// \param index Index of the render target in the list
			/// \return true if the index exists and the framebuffer is bound for drawing false othewise
			bool Draw(unsigned int index);
			/// Specify which color buffers are to be drawn into
			/// \param indices Array of indices of the render targets in the list
			/// \param lenght Lenght of the array
			/// \return true if all the indices exist and the framebuffer is bound for drawing false othewise
			bool Draw(unsigned int* indices, unsigned int lenght);
			/// Remove current draw targets
			/// \return true if the framebuffer is bound for drawing false otherwise
			bool DisableDraw();
			/// Specify which color buffers are to be read from
			/// \param index Index of the render target in the list
			/// \return true if the index exists false and the framebuffer is bound for reading othewise
			bool Read(unsigned int index);
			/// Remove current read targets
			/// \return true if the framebuffer is bound for reading false otherwise
			bool DisableRead();
			/// Returns the OpenGl index for the FBO
			unsigned int GetIndex() { return mFramebufferIndex; }
		private:
			unsigned int mFramebufferIndex;
			G_FRAMEBUFFER_BINDS mFramebufferState;
			std::list<GRenderTarget*> mColorAttachements;
			GDepthStencilTexture* mpDepthTexture;
			GDepthStencilRenderBuffer* mpDepthBuffer;
			GFramebufferManager* mpManager;
		};
	}
}

#endif