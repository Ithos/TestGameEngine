#pragma once

#ifndef TESTCONFIG_H

#include <string>
#include <map>


namespace UnitTest
{

	struct TestConfigurationConstants
	{
		static const std::string TEST_CONFIG_FILE;
		static const std::string TEST_IMAGE_FOLDER;
		static const std::string TEST_IMAGE_FILE;
		static const char SEPARATOR;
	};

	///Singleton for reading the test configuration file 
	class TestConfigurationManager
	{
	public:
		///Initializes the singleton instance if it has not been initialized and returns the singleton instance
		/// \return singleton instance
		static TestConfigurationManager* GetInstance();
		///Destroys the singleton instance if it has been initialized
		static void DestroyInstance();

		///Gets the configuration property that contains the relative path to the test image folder
		/// \return Relative path to the test image folder
		const std::string& GetTestImageFolder() { return _mTestImageFolder; }
		///Gets the configuration property that contains the name of the test image configuration file
		/// \return Name of the image configuration file
		const std::string& GetTestImageFile() { return _mTestImageConfigFile; }

		///Gets the path to a loaded texture. If the texture key has not been defined an assert will trigger
		/// \param key texture key
		/// \return path to the texture
		const std::string& GetTextureDir(const std::string& key);

	private:
		///Singleton instance
		static TestConfigurationManager* _mpInstance;
		std::string _mTestImageFolder;
		std::string _mTestImageConfigFile;

		///Path map
		std::map <std::string, std::string* > _mConfigMap;
		/// Images map 
		std::map<std::string, std::string> _mImagesMap;

		///Constructor.Calls init.
		TestConfigurationManager();
		///Copy constructor. Declared private so it can't be used.
		TestConfigurationManager(const TestConfigurationManager& ref) {}
		///Destructor
		virtual ~TestConfigurationManager();
		///Fills the path map and calls readConfigFile
		void init();
		///Reads the configuration file and inserts the values into the path map
		/// \param map path map where the config values will be returned
		void readConfigFile(std::map <std::string, std::string* > & map);
	};
}
#endif // !TESTCONFIG_H
