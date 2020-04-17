#pragma once

#ifndef GRENDERTARGET_H
#define GRENDERTARGET_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <qvector2d.h>

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		class GFramebufferObject;
		/// Class that represents an element that can be a render target
		class GRenderTarget : protected QOpenGLExtraFunctions
		{
		public:
			/// Default constructor
			GRenderTarget();
			/// Constructor. Creates a texture with white border color.
			/// param textureSize Size of the texture to be created
			GRenderTarget(const QVector2D& size);
			/// Copy constructor
			/// param ref Object to be copied
			GRenderTarget(const GRenderTarget& ref);
			/// Abstract method. Resizes the render target
			/// param size New size for the render target
			virtual void Resize(const QVector2D& size) = 0;
			/// Gets the OpenGl index for the element
			/// return OpenGl index of this element
			GLuint GetIndex() const { return mIndex; }
			/// Gets the attachment index for the element
			/// return attachment index of this element
			GLint GetAttachment() const { return mAttachment; }
			/// Sets the attachment for this element. The actual attachment is updated when Enable is called.
			/// prameter attachmentIndex integer that represents the attachment, the specific attachment represented must be decided by the child class 
			virtual void SetAttachment(unsigned int attachmentIndex) = 0;
			/// Binds this render target to the OpenGl pipeline to perform operations on it
			virtual void Bind() = 0;
			/// Unbinds this render target from the OpenGl pipeline so no more operations are performed on it
			virtual void Unbind() = 0;
			/// Clear the target stored information
			virtual void Clear() = 0;
			/// Clear the target stored information with a given value or array of values
			virtual void Clear(GLuint * values) = 0;
			/// Attaches or detaches the target from its given attachment
			/// Returns true if the target is owned by an FBO false otherwise
			virtual bool Enable(bool enable) = 0;
			/// Returns whether the target is enabled or not
			virtual bool GetEnabled() const { return mEnabled; }
			/// Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual GRenderTarget* Clone() const = 0;
		protected:
			QVector2D mSize;
			GLuint mIndex;
			GLenum mAttachment;
			bool mEnabled;
			const GFramebufferObject* mOwner;
			/// Copies a data GRenderTarget to the current object
			/// param ref GRenderTarget to be copied
			virtual void copy(const GRenderTarget& ref);
			/// If the render target has an owner returns whether it is bound
			bool GetOwnerBound() const;
		private:
			/// General initialization common to al children of this class
			void init();
		};
	}
}

#endif
