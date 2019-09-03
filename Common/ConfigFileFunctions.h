#pragma once
#ifndef CONFIGFILEFUNCTIONS_H
#define CONFIGFILEFUNCTIONS_H

#include "StringFunctions.h"
#include <string>
#include <map>
#include <fstream>
///Namespace for utility classes for reading config files
namespace ConfigFileFunctions
{
	///Enum containing different posible return codes
	enum class ReadConfigErrorCodes
	{
		SUCCESS,
		CONFIG_FILE_NOT_FOUND,
		DUPLICATE_KEY
	};
	
	///Static singleton for reading config files
	class ConfigFileFunctions
	{
	public:
		///Reads a configuration file following the format "[key] [separator] [value]" returns all pairs of key value in a [string, string] map
		///param folderPath path of the folder that contains the conf file
		///param confFile name of the conf file
		///param separator used in the conf file
		///param desMap map where the key value pairs will be returned
		///return result code
		static ReadConfigErrorCodes ReadConfigFileIntoStringMap(const std::string& folderPath, const std::string& confFile, char separator, std::map<std::string, std::string>& desMap)
		{
			std::ifstream infile(std::string(folderPath).append(confFile));
			if (!infile.is_open())
			{
				return ReadConfigErrorCodes::CONFIG_FILE_NOT_FOUND;
			}

			std::string line;
			while (std::getline(infile, line))
			{
				std::vector<std::string> tokenConfig;
				StringFunctions::StringFunctions::splitString(StringFunctions::StringFunctions::trim(line), separator, tokenConfig);

				if (!(desMap.find(tokenConfig[0]) == desMap.end()))
				{
					return ReadConfigErrorCodes::DUPLICATE_KEY;
				}

				desMap.insert(std::pair<std::string, std::string>(tokenConfig[0], std::string(folderPath).append(tokenConfig[1])));
			}

			return ReadConfigErrorCodes::SUCCESS;
		}

	};
}

#endif