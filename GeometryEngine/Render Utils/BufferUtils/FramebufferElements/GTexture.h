#pragma once

#ifndef GTEXTURE_H
#define GTEXTURE_H

#include <qvector3d.h>
#include "GRenderTarget.h"
#include "GFramebufferCommmons.h"

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		/// Class that represents a OpenGl color texture
		class GTexture : public GRenderTarget
		{
		public:
			/// Constructor
			/// param textureSize Size of the texture to be created
			/// param attachment Color attachment to use
			/// param borderColor Border color of the texture
			/// param owner GFramebufferObject where this texture is contained
			GTexture(const QVector2D& textureSize, const QVector3D& borderColor, GFramebufferCommons::G_COLOR_ATTACHMENTS attachment, const GFramebufferObject* owner);
			/// Constructor
			/// param textureSize Size of the texture to be created
			/// param borderColor Border color of the texture
			GTexture(const QVector2D& textureSize, const QVector3D& borderColor);
			/// Constructor. Creates a texture with white border color.
			/// param textureSize Size of the texture to be created
			GTexture(const QVector2D& textureSize);
			/// Null texture constructor
			/// Builds a null texture to be passed to shaders that need a texture to be passed
			/// param attachment Color attachment to use
			/// param owner GFramebufferObject where this texture is contained
			GTexture(GFramebufferCommons::G_COLOR_ATTACHMENTS attachment, const GFramebufferObject* owner);
			/// Default constructor
			/// Builds a null texture to be passed to shaders that need a texture to be passed
			GTexture();
			/// Copy constructor
			/// param ref Object to be copied
			GTexture(const GTexture& ref);
			/// Destructor
			virtual ~GTexture();
			/// Binds this texture to the OpenGl pipeline to perform operations on it
			virtual void Bind();
			/// Unbinds this texture from the OpenGl pipeline so no more operations are performed on it
			virtual void Unbind();
			/// Clear the target stored information
			virtual void Clear();
			/// Clear the target stored information with a given value or array of values
			virtual void Clear(GLuint * values);
			/// Enable/Disable the depth buffer
			/// Returns true if the target is owned by an FBO false otherwise
			virtual bool Enable(bool enable);
			/// resizes the texture
			/// param textureSize New size for the texture
			virtual void Resize(const QVector2D& textureSize);
			/// Sets the attachment for this texture. The actual attachment is updated when Enable is called.
			/// param colorAttachmentNumber Number of the color attachment to use
			/// return true if the texture has an owner false otherwise
			virtual void SetAttachment(unsigned int attachmentIndex);
			/// Sets the attachment for this texture.The actual attachment is updated when Enable is called.
			/// param attachment Color attachment to use
			/// return true if the texture has an owner false otherwise
			void SetAttachment(GFramebufferCommons::G_COLOR_ATTACHMENTS attachment);
			///Sets the texture border color
			/// param borderColor Border color of the texture
			void SetBorderColor(const QVector3D& borderColor);
			/// Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual GTexture* Clone() const { return new GTexture(*this); };
		protected:
			/// Copies the OpenGl data from a GTexture to the current object, creating another object that manages the same OpenGl texture
			/// param ref GTexture to be copied
			virtual void copy(const GTexture& ref);
		private:
			QVector3D mBorderColor;
		};
	}
}

#endif