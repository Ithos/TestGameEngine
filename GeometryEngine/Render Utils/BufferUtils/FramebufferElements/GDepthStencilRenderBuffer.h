#pragma once

#ifndef GDEPTHSTENCILRENDERBUFFER_H
#define GDEPTHSTENCILRENDERBUFFER_H

#include "GRenderTarget.h"
#include "GFramebufferCommmons.h"

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		class GDepthStencilRenderBuffer : public GRenderTarget
		{
		public:
			/// Constructor.
			/// \param textureSize Size of the texture to be created
			GDepthStencilRenderBuffer(const QVector2D& textureSize);
			/// Constructor.
			/// \param textureSize Size of the texture to be created
			/// \param attachement Color attachment to use 
			/// \param owner GFramebufferObject where this texture is contained
			GDepthStencilRenderBuffer(const QVector2D& textureSize, GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS attachment, const GFramebufferObject* owner);
			/// Copy constructor
			/// \param ref Object to be copied
			GDepthStencilRenderBuffer(const GDepthStencilRenderBuffer& ref);
			///Destructor
			virtual ~GDepthStencilRenderBuffer();
			/// Binds this texture to the OpenGl pipeline to perform operations on it
			virtual void Bind();
			/// Unbinds this texture from the OpenGl pipeline so no more operations are performed on it
			virtual void Unbind();
			/// Clear the target stored information
			virtual void Clear();
			/// Clear the target stored information with a given value or array of values
			virtual void Clear(GLuint * values);
			/// resizes the texture
			/// \param textureSize New size for the texture
			virtual void Resize(const QVector2D& textureSize);
			/// Sets the attachment for this texture
			/// \param colorAttachmentNumber Number of the color attachment to use
			/// \return true if the texture has an owner false otherwise
			virtual void SetAttachment(unsigned int attachmentIndex);
			/// Sets the attachment for this texture
			/// \param attachment Color attachment to use
			/// \return true if the texture has an owner false otherwise
			void SetAttachment(GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS attachment);
			/// Enable/Disable the depth buffer
			virtual bool Enable(bool enable);
			/// Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual GDepthStencilRenderBuffer* Clone() const { return new GDepthStencilRenderBuffer(*this); };
		protected:
			/// Copies a data GRenderBuffer to the current object
			/// \param ref GRenderBuffer to be copied
			virtual void copy(const GDepthStencilRenderBuffer& ref);
		};
	}
}

#endif