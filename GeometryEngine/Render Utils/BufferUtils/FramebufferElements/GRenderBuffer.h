#pragma once

#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include <qvector3d.h>
#include "GRenderTarget.h"
#include "GFramebufferCommmons.h"

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		/// Class that represents a OpenGl color render buffer
		class GRenderBuffer : public GRenderTarget
		{
		public:
			/// Constructor.
			/// param textureSize Size of the texture to be created
			GRenderBuffer(const QVector2D& textureSize);
			/// Constructor.
			/// param textureSize Size of the texture to be created
			/// param attachement Color attachment to use 
			GRenderBuffer(const QVector2D& textureSize, GFramebufferCommons::G_COLOR_ATTACHMENTS attachment, const GFramebufferObject* owner);
			/// Copy constructor
			/// param ref Object to be copied
			GRenderBuffer(const GRenderBuffer& ref);
			///Destructor
			virtual ~GRenderBuffer();
			/// Binds this texture to the OpenGl pipeline to perform operations on it
			virtual void Bind();
			/// Unbinds this texture from the OpenGl pipeline so no more operations are performed on it
			virtual void Unbind();
			/// Clear the target stored information
			virtual void Clear();
			/// Clear the target stored information with a given value or array of values
			virtual void Clear(GLuint * values);
			/// Attaches or detaches the target from its given attachment
			/// Returns true if the target is owned by an FBO false otherwise
			virtual bool Enable(bool enable);
			/// resizes the texture
			/// param textureSize New size for the texture
			virtual void Resize(const QVector2D& textureSize);
			/// Sets the attachment for this texture. The actual attachment is updated when Enable is called.
			/// param colorAttachmentNumber Number of the color attachment to use
			/// return true if the texture has an owner false otherwise
			virtual void SetAttachment(unsigned int attachmentIndex);
			/// Sets the attachment for this texture. The actual attachment is updated when Enable is called.
			/// param attachment Color attachment to use
			/// return true if the texture has an owner false otherwise
			void SetAttachment(GFramebufferCommons::G_COLOR_ATTACHMENTS attachment);
			/// Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual GRenderBuffer* Clone() const { return new GRenderBuffer(*this); };
		protected:
			/// Copies a data GRenderBuffer to the current object
			/// param ref GRenderBuffer to be copied
			virtual void copy(const GRenderBuffer& ref);
		};
	}
}

#endif