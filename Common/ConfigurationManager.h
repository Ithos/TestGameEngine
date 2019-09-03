#pragma once

#ifndef __CONFIGURATION_CONFMANAGER_H
#define __CONFIGURATION_CONFMANAGER_H

#include "StringFunctions.h"
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>
#include <map>
///namespace for reading configuration files and storing  that information
namespace Configuration
{
	///Struct that contains constants with the initial values needed to read the main config file
	struct ConfigurationManagerConstants
	{
		static const std::string CONFIG_FILE;
		static const std::string VERTEX_SHADER_FOLDER;
		static const std::string FRAGMENT_SHADER_FOLDER;
		static const std::string CONFIG_FRAGMENT_SHADER;
		static const std::string CONFIG_VERTEX_SHADER;
		static const std::string TEXTURES_FOLDER;
		static const std::string CONFIG_TEXTURES;
		static const char SEPARATOR;
	};

	///Singleton for reading the main configuration file 
	class ConfigurationManager
	{
	public:
		///Initializes the singleton instance if it has not been initialized and returns the singleton instance
		///return singleton instance
		static ConfigurationManager* GetInstance();
		///Destroys the singleton instance if it has been initialized
		static void DestroyInstance();

		const std::string& getVertexShaderFolder() { return _mVertexShaderFolder; }
		const std::string& getFragmentShaderFolder() { return _mFragmentShaderFolder; }
		const std::string& getVertexShaderConfig() { return _mVertexShaderConfig; }
		const std::string& getFragmentShaderConfig() { return _mFragmentShaderConfig; }
		const std::string& getTexturesFolder() { return _mTexturesFolder; }
		const std::string& getTexturesConfig() { return _mTexturesConfig; }

	private:
		///Singleton instance
		static ConfigurationManager* _mpInstance;
		std::string _mVertexShaderFolder;
		std::string _mFragmentShaderFolder;
		std::string _mVertexShaderConfig;
		std::string _mFragmentShaderConfig;
		std::string _mTexturesFolder;
		std::string _mTexturesConfig;
		///Path map
		std::map <std::string, std::string* > _mConfigMap;

		///Constructor.Calls init.
		ConfigurationManager();
		///Copy constructor. Declared private so it can't be used.
		ConfigurationManager(const ConfigurationManager& ref) {}
		///Destructor
		virtual ~ConfigurationManager();

		///Fills the path map and calls readConfigFile
		void init();
		///Reads the configuration file and inserts the values into the path map
		///param map path map where the config values will be returned
		void readConfigFile(std::map <std::string, std::string* > & map);

	};
}
#endif