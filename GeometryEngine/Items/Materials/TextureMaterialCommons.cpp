#include "TextureMaterialCommons.h"

const std::string GeometryEngine::GeometryMaterial::TextureConstant::ERROR_TEXTURE = "ERROR_TEXTURE";
const std::string GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLUE_CHIP_TEXTURE = "TEST_BLUE_CHIP_TEXTURE";
const std::string GeometryEngine::GeometryMaterial::TextureConstant::TEST_UP_TEXTURE = "TEST_UP_TEXTURE";
const std::string GeometryEngine::GeometryMaterial::TextureConstant::TEST_DOWN_TEXTURE = "TEST_DOWN_TEXTURE";
const std::string GeometryEngine::GeometryMaterial::TextureConstant::TEST_FORWARD_TEXTURE = "TEST_FORWARD_TEXTURE";
const std::string GeometryEngine::GeometryMaterial::TextureConstant::TEST_BACK_TEXTURE = "TEST_BACK_TEXTURE";
const std::string GeometryEngine::GeometryMaterial::TextureConstant::TEST_LEFT_TEXTURE = "TEST_LEFT_TEXTURE";
const std::string GeometryEngine::GeometryMaterial::TextureConstant::TEST_RIGHT_TEXTURE = "TEST_RIGHT_TEXTURE";
const std::string GeometryEngine::GeometryMaterial::TextureConstant::TEST_UNIFIED_CUBE = "TEST_UNIFIED_CUBE";
const std::string GeometryEngine::GeometryMaterial::TextureConstant::TEST_TEXTURE = "TEST_TEXTURE";
const std::string GeometryEngine::GeometryMaterial::TextureConstant::TEST_BLACK_TEXTURE = "TEST_BLACK_TEXTURE";
const std::string GeometryEngine::GeometryMaterial::TextureConstant::NORMALMAP_TEST_BLUE_CHIP = "NORMALMAP_TEST_BLUE_CHIP";

void GeometryEngine::GeometryMaterial::TextureParameters::Build()
{
	QImage img = QImage(QString(TextureDir.c_str()));

	if (img.isNull())
	{
		img = QImage(QString(mpTexDirManager->GetTextureDir(TextureConstant::ERROR_TEXTURE).c_str()));
	}

	Texture = new QOpenGLTexture(img.mirrored());
}
