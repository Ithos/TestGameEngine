#include "stdafx.h"

#include <cassert>
#include <fstream>
#include "StringFunctions.h"
#include "ConfigFileFunctions.h"
#include "TestConfig.h"

namespace UnitTest
{
	TestConfigurationManager* TestConfigurationManager::_mpInstance = nullptr;

	const std::string TestConfigurationConstants::TEST_CONFIG_FILE = "../settings/configTest";
	const std::string TestConfigurationConstants::TEST_IMAGE_FOLDER = "TEST_IMAGES_FOLDER";
	const std::string TestConfigurationConstants::TEST_DEBUG_FOLDER = "TEST_DEBUG_FOLDER";
	const std::string TestConfigurationConstants::TEST_IMAGE_FILE = "TEST_IMAGE_FILE";
	const char TestConfigurationConstants::SEPARATOR = '=';

	TestConfigurationManager * UnitTest::TestConfigurationManager::GetInstance()
{
	if (_mpInstance == nullptr)
	{
		_mpInstance = new TestConfigurationManager();
	}

	return _mpInstance;
}

void TestConfigurationManager::DestroyInstance()
{
	if (_mpInstance != nullptr)
	{
		delete _mpInstance;
		_mpInstance = nullptr;
	}
}

const std::string & TestConfigurationManager::GetTextureDir(const std::string & key)
{
	assert(_mImagesMap.find(key) != _mImagesMap.end() && "Texture not found");
	return _mImagesMap[key];
}

TestConfigurationManager::TestConfigurationManager()
{
	init();
}

TestConfigurationManager::~TestConfigurationManager()
{
}

void TestConfigurationManager::init()
{
	_mConfigMap[TestConfigurationConstants::TEST_IMAGE_FOLDER] = &_mTestImageFolder;
	_mConfigMap[TestConfigurationConstants::TEST_DEBUG_FOLDER] = &_mDebugImageFolder;
	_mConfigMap[TestConfigurationConstants::TEST_IMAGE_FILE] = &_mTestImageConfigFile;
	readConfigFile(_mConfigMap);

	bool ok = ConfigFileFunctions::ConfigFileFunctions::ReadConfigFileIntoStringMap(_mTestImageFolder, _mTestImageConfigFile, TestConfigurationConstants::SEPARATOR, _mImagesMap) ==
		ConfigFileFunctions::ReadConfigErrorCodes::SUCCESS;

	assert(ok && "Error reading test image configuration file");
}

void TestConfigurationManager::readConfigFile(std::map<std::string, std::string*>& map)
{
	std::ifstream infile(TestConfigurationConstants::TEST_CONFIG_FILE);

	assert(infile.is_open() && "Config file not found");

	std::string line;
	while (std::getline(infile, line))
	{
		std::vector<std::string> tokenConfig;
		StringFunctions::StringFunctions::splitString(StringFunctions::StringFunctions::trim(line), TestConfigurationConstants::SEPARATOR, tokenConfig);

		auto it = map.find(tokenConfig[0]);

		if (it != map.end())
		{
			(*map[tokenConfig[0]]) = tokenConfig[1];
		}
	}
}

}