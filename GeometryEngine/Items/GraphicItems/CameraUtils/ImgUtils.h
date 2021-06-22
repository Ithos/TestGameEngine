#pragma once

#ifndef GEOMETRYIMGUTILS_H

#include <qopengl.h>

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		class IBuffer;
	}

	/// Namespace for aditional camera related utilities
	namespace CameraUtils
	{

		/// Static singleton that contains utilities for exporting rendered images
		class ImgUtils
		{
		public:
			/// Gets the currently rendered image as a uchar array. The returned array must be destryed by the caller.
			/// \param width Image pixel width
			/// \param height Image pixel height
			/// \return Image pixels
			static uchar* GetRenderedImage(int width, int height);
			/// Gets the currently rendered image at the selected texture from the selected buffer as a uchar array. The returned array must be destryed by the caller.
			/// \param img Output parameter where the image will be returned
			/// \param width Image pixel width
			/// \param height Image pixel height
			/// \param buffer Buffer to be read
			/// \param idBufferTexture Buffer texture to be read
			/// \return Image pixels
			static uchar* GetRenderedImage(int width, int height, GeometryBuffer::IBuffer* buffer, int idBufferTexture);
			/// Exports the currently rendered image to the selected path as a png file
			/// \param path Path to save the image to
			/// \param filename Name of the image file
			/// \param width Image pixel width
			/// \param height Image pixel height
			static void ExportRenderedImage(const std::string& path, const std::string& filename, int width, int height);
			/// Exports the currently rendered image at the selected texture from the selected buffer to the selected path as a png file
			/// \param path Path to save the image to
			/// \param filename Name of the image file
			/// \param width Image pixel width
			/// \param height Image pixel height
			/// \param buffer Buffer to be read
			/// \param idBufferTexture Buffer texture to be read
			static void ExportRenderedImage(const std::string& path, const std::string& filename, int width, int height, GeometryBuffer::IBuffer* buffer, int idBufferTexture);
			/// Exports a image to the selected path as a png file
			/// \param img Image to be exported
			/// \param path Path to save the image to
			/// \param filename Name of the image file
			/// \param width Image pixel width
			/// \param height Image pixel height
			/// \param mirrored Invert the image
			static void ExportImage(uchar* img, const std::string& path, const std::string& filename, int width, int height, bool mirrored);
			/// Exports a image to the selected path as a png file
			/// \param img Image to be exported
			/// \param path Path to save the image to
			/// \param filename Name of the image file
			/// \param mirrored Invert the image
			static void ExportImage(const QImage& img, const std::string& path, const std::string& filename, bool mirrored);
		};
	}
}

#endif // !GEOMETRYIMGUTILS_H
