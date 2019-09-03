#pragma once
#include <string>
#include <vector>
#include <sstream>

namespace StringFunctions
{
	///Static singleton that contains string utility methods
	class StringFunctions
	{
	public:
		///Splits a string by a separator and returs the resulting strings in a list
		///param source string to be split
		///param separator char used as separator in the string
		///param oVec string list where the result is returned
		static void splitString(const std::string & source, char separator, std::vector<std::string>& oVec)
		{
			std::string token;
			std::istringstream f(source);

			while (std::getline(f, token, separator))
			{
				oVec.push_back(trim(token));
			}
		}

		///Eliminates empty spaces from the beginning and end of a string
		///param str string to be trimmed
		///return the result of the operation is returned in a different string
		static std::string trim(const std::string& str)
		{
			size_t first = str.find_first_not_of(' ');
			if (std::string::npos == first)
			{
				return str;
			}
			size_t last = str.find_last_not_of(' ');
			return str.substr(first, (last - first + 1));
		}
	};
}
