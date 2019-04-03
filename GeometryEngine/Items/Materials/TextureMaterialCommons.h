#pragma once

#ifndef TEXTUREMATERIALCOMMONS_H
#define TEXTUREMATERIALCOMMONS_H

#include <string>

#include <qopengltexture.h>

#include <Textures.h>
#include <ConfigurationManager.h>

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		struct TextureConstant
		{
			static const std::string ERROR_TEXTURE;
			static const std::string TEST_BLUE_CHIP_TEXTURE;
			static const std::string TEST_DOWN_TEXTURE;
			static const std::string TEST_UP_TEXTURE;
			static const std::string TEST_FORWARD_TEXTURE;
			static const std::string TEST_BACK_TEXTURE;
			static const std::string TEST_LEFT_TEXTURE;
			static const std::string TEST_RIGHT_TEXTURE;
			static const std::string TEST_UNIFIED_CUBE;
			static const std::string TEST_TEXTURE;
			static const std::string TEST_BLACK_TEXTURE;
		};

		class TextureParameters
		{
		public:
			TextureParameters(const std::string& texDir, int vertex, bool getFromConf = false) : Texture(nullptr), mpConfInstance(nullptr), mpTexDirManager(nullptr)
			{
				initManagers();
				TextureDir = getFromConf ? mpTexDirManager->GetTextureDir(texDir) : texDir;
				VertexNumber = vertex;
			}

			TextureParameters(const TextureParameters& ori)
			{
				initManagers();
				TextureDir = ori.TextureDir;
				VertexNumber = ori.VertexNumber;
				ori.Texture == nullptr ? this->Texture = nullptr : Build();
			}

			virtual ~TextureParameters()
			{
				delete Texture;
			}

		private:
			std::string TextureDir;
			int VertexNumber;
			QOpenGLTexture* Texture;
			Configuration::ConfigurationManager* mpConfInstance;
			TexturesFiles::Textures * mpTexDirManager;

			friend class TextureMaterial;
			friend class MultiTextureMaterial;

			void initManagers()
			{
				mpConfInstance = Configuration::ConfigurationManager::GetInstance();
				mpTexDirManager = TexturesFiles::Textures::InitInstance(mpConfInstance->getTexturesFolder(), mpConfInstance->getTexturesConfig());
			}

			void Build();

		};
	}
}

#endif