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
		/// Struct that contains keys for texture directions as constant strings
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
			static const std::string NORMALMAP_TEST_BLUE_CHIP;
		};

		/// Class that holds texture data
		class TextureParameters
		{
		public:
			/// Constructor
			/// param texDir A texture direction or a key for a texture direction
			/// param vertex Number of vertices that this texture will be applied to, its only used when many textures will be applied to the same object
			/// param getFromConf If true the texDir parameter will be treated as a key for the conf files, else it will be treated as a path for a texture. 
			TextureParameters(const std::string& texDir, int vertex = -1, bool getFromConf = true) : Texture(nullptr), mpConfInstance(nullptr), mpTexDirManager(nullptr)
			{
				initManagers();
				TextureDir = getFromConf ? mpTexDirManager->GetTextureDir(texDir) : texDir;
				VertexNumber = vertex;
			}

			/// Copy constructor
			/// param ori TextureParameters to be copied
			TextureParameters(const TextureParameters& ori)
			{
				initManagers();
				TextureDir = ori.TextureDir;
				VertexNumber = ori.VertexNumber;
				ori.Texture == nullptr ? this->Texture = nullptr : Build();
			}

			///Destructor
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
			friend class NormalMapTextureMaterial;
			friend class NormalMapMultiTextureMaterial;

			/// Get instances to configuration and texture managers
			void initManagers()
			{
				mpConfInstance = Configuration::ConfigurationManager::GetInstance();
				mpTexDirManager = TexturesFiles::Textures::InitInstance(mpConfInstance->getTexturesFolder(), mpConfInstance->getTexturesConfig());
			}

			/// Loads the texture as an image and builds the texture
			void Build();

		};
	}
}

#endif