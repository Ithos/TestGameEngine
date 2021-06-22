#pragma once

#ifndef GEOMETRY_BUFFER_BASE_H
#define GEOMETRY_BUFFER_BASE_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		/// Interface class for all buffer classes
		class IBuffer : protected QOpenGLExtraFunctions
		{
		public:
			/// Creates framebuffers and textures
			/// \param MaxWindowWidth Max texture width
			/// \param MaxWindowHeight Max texture height
			/// \return true if ok false otherwise
			virtual bool Init(unsigned int MaxWindowWidth, unsigned int MaxWindowHeight) = 0;
			/// Resizes textures
			/// \param WindowWidth New texture width
			/// \param WindowHeight New texture height
			/// \return true if ok false otherwise
			virtual bool Resize(unsigned int WindowWidth, unsigned int WindowHeight) = 0;
			/// Clears textures color
			virtual void StartFrame() = 0;
			/// Binds the buffer in read mode and sets the selected texture as read
			/// \param texture index to be bound
			virtual void BindTextureRead(unsigned int index) = 0;
			/// Binds the buffer in draw mode and sets the selected texture as draw
			/// \param texture index to be bound
			virtual void BindTextureDraw(unsigned int index) = 0;
			/// Binds the buffer in draw-read mode and sets the selected texture as draw-read
			/// \param texture index to be bound
			virtual void BindTextureDrawRead(unsigned int index) = 0;
			/// Binds selected texture to its default texture unit
			/// \param index texture index to bind
			virtual void BindTexture(unsigned int index) = 0;
			/// Binds selected texture to the selected texture unit
			/// \param index texture index to bind
			/// \param textureUnit texture unit to bind the texture to
			virtual void BindTexture(unsigned int index, unsigned int textureUnit) = 0;
			/// Unbinds texture from its default texture unit
			/// \param index texture index to bind
			virtual void UnbindTexture(unsigned int index) = 0;
			/// Unbinds texture from the selected texture unit
			/// \param index texture index to bind
			/// \param textureUnit texture unit to bind the texture to
			virtual void UnbindTexture(unsigned int index, unsigned int textureUnit) = 0;
			/// Unbind all texture locations
			virtual void ResetBindings() = 0;
			/// Unbind all buffer textures
			virtual void UnbindBuffer() = 0;
			/// Gets current texture size
			/// \return Current texture size
			virtual const QVector2D& GetTextureSize() const = 0;
			/// Gets the max texture size
			/// \return Max texture siza
			virtual const QVector2D& GetMaxTextureSize() const = 0;
			/// Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual IBuffer* Clone() const = 0;
		};
	}
}

#endif // !GEOMETRY_BUFFER_BASE_H
