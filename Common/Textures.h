#pragma once

#ifndef TEXTURESCONFIGURATION_H
#define TEXTURESCONFIGURATION_H

#include "ConfigFileFunctions.h"
#include <map>
#include <cassert>

///namespace for classes that manage texture files
namespace TexturesFiles
{
	///Struct that stores constats for the texture manager singleton
	struct TexturesFilesConstants
	{
		static const char SEPARATOR;
	};

	///Singleton that manages texture files and texture configuration files
	class Textures
	{
	public:
		/// Returns the singleton instance if null the singleton has not been initialized
		/// \return singleton instance
		static Textures* GetActiveInstance() { return _mpInstance; }
		///Initializes the singleton instance if it has not been initialized and returns the singleton instance
		/// \param texFolder path to the textures folder
		/// \param texConfFile name of the textures conf file
		/// \return singleton instance
		static Textures* InitInstance(const std::string& texFolder, const std::string& texConfFile);
		///Destroys the singleton instance if it has been initialized
		static void DestroyInstance();
		///Gets the path to a loaded texture. If the texture key has not been defined an assert will trigger
		/// \param key texture key
		/// \return path to the texture
		const std::string& GetTextureDir(const std::string& key);

	private:
		///Singleton Instance
		static Textures* _mpInstance;
		///Key path texture map
		std::map<std::string, std::string> mTextures;
		std::string mTexFolder;
		std::string mTexConf;

		///Default constructor. Defined as private so it can't be used ouside of this class
		Textures() {}
		///Copy constructor. Declared private so it can't be used.
		Textures(const Textures& ref){}
		///Constructor. Calls init.
		/// \param texFolder path to the textures folder
		/// \param texConfFile name of the textures conf file
		Textures(const std::string& texFolder, const std::string& texConfFile);
		///Reads config files and fills the textures key value map
		void init();
		///Destructor
		virtual ~Textures();
	};
}
#endif