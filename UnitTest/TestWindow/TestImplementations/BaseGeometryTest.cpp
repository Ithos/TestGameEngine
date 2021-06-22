#include "stdafx.h"

#include <ctime>
#include <qimage.h>
#include <Items\GraphicItems\CameraUtils\ImgUtils.h>
#include "BaseGeometryTest.h"

void UnitTest::CBaseGeometryTest::copy(const CBaseGeometryTest & ref)
{
	this->mTestFinished = false; this->mTestSuccessful = false; this->mScenePainted = false;
	this->mSceneWidth = ref.mSceneWidth; this->mSceneHeigth = ref.mSceneHeigth;
};

bool UnitTest::CBaseGeometryTest::compareScreenImage(const std::string & testImagePath, const std::string & className)
{
	uchar* screen = GeometryEngine::CameraUtils::ImgUtils::GetRenderedImage(mSceneWidth, mSceneHeigth);
	QImage image(QString(testImagePath.c_str()));
	image = image.mirrored().convertToFormat(QImage::Format::Format_RGB888);
	uchar* bytes = image.bits();
	bool result = true;
	for (int i = 0; i < 3 * mSceneWidth * mSceneHeigth; ++i)
	{
		result &= (screen[i] == bytes[i]);
		if (!result)break;
	}

	if (!result)
	{
		std::time_t t = std::time(nullptr);
		GeometryEngine::CameraUtils::ImgUtils::ExportImage(screen, mpConfManager->GetTestImageFile(),
			std::string("debug_").append(className).append("_").append(std::to_string(t)), mSceneWidth, mSceneHeigth, true);
	}

	delete[] screen;
	return result;
}
