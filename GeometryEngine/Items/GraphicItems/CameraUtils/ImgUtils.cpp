#include <qimage.h>
#include "..\Render Utils\IBuffer.h"
#include "ImgUtils.h"


uchar* GeometryEngine::CameraUtils::ImgUtils::GetRenderedImage(int width, int height)
{
	uchar* tmp = new uchar[3 * width * height];
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, tmp);
	return tmp;
}

uchar* GeometryEngine::CameraUtils::ImgUtils::GetRenderedImage(int width, int height, GeometryBuffer::IBuffer * buffer, int idBufferTexture)
{
	buffer->BindTextureRead(idBufferTexture);
	return GetRenderedImage(width, height);
}

void GeometryEngine::CameraUtils::ImgUtils::ExportRenderedImage(const std::string & path, const std::string& filename, int width, int height)
{
	uchar* img = GetRenderedImage(width, height);
	ExportImage(img, path, filename, width, height, true);
	delete[] img;
}

void GeometryEngine::CameraUtils::ImgUtils::ExportRenderedImage(const std::string & path, const std::string& filename, int width, int height, GeometryBuffer::IBuffer * buffer, int idBufferTexture)
{
	uchar* img = GetRenderedImage(width, height, buffer, idBufferTexture);
	ExportImage(img, path, filename, width, height, true);
	delete[] img;
}

void GeometryEngine::CameraUtils::ImgUtils::ExportImage(uchar * img, const std::string & path, const std::string& filename, int width, int height, bool mirrored)
{
	QImage qimg(img, width, height, QImage::Format::Format_RGB888);
	ExportImage(qimg, path, filename, mirrored);
}

void GeometryEngine::CameraUtils::ImgUtils::ExportImage(const QImage & img, const std::string & path, const std::string& filename, bool mirrored)
{
	QString file = QString(std::string(path).append(filename).append(".tiff").c_str());
	if(mirrored)img.mirrored().save(file);
	else img.save(file);
}
