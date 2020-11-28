#pragma once

#ifndef GDEPTHTEXTURE_H
#define GDEPTHTEXTURE_H

#include "GRenderTarget.h"
#include "GFramebufferCommmons.h"

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		/// Class that represents a texture that stores depth or stencil information
		class GDepthStencilTexture : public GRenderTarget
		{
		public:
			/// Constructor.
			/// \param textureSize Size of the texture to be created
			GDepthStencilTexture(const QVector2D& textureSize);
			/// Constructor
			/// \param textureSize Size of the texture to be created
			/// \param depthAttachment Number of the depth attachment to use, if it's greater than 0 the depth texture will be attached
			/// \param owner GFramebufferObject where this texture is contained
			GDepthStencilTexture(const QVector2D& textureSize, GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS attachment, const GFramebufferObject* owner);
			/// Copy constructor
			/// \param ref Object to be copied
			GDepthStencilTexture(const GDepthStencilTexture& ref);
			///Destructor
			virtual ~GDepthStencilTexture();
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
			virtual void SetAttachment(GFramebufferCommons::G_DEPTH_STENCIL_ATTACHMENTS attachment);
			/// Enable/Disable the depth texture
			virtual bool Enable(bool enable);
			/// Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual GDepthStencilTexture* Clone() const { return new GDepthStencilTexture(*this); };
		protected:
			/// Copies the OpenGl data from a GTexture to the current object, creating another object that manages the same OpenGl texture
			/// \param ref GTexture to be copied
			virtual void copy(const GDepthStencilTexture& ref);
		};
	}
}

#endif