#pragma once

#ifndef __SHADERFILES_SHADER_H
#define __SHADERFILES_SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <cassert>

namespace ShaderFiles
{
	///Class used by ShaderManager to load shaders and  store the Shader raw code
	class Shader
	{
	public:
		///Constructor. Loads the sader.
		/// \param filepath path to the shader file
		Shader(const std::string& filepath);
		///Destructor
		virtual ~Shader();
		/// Returns the raw code of the shader
		/// \return Raw code of the shader
		std::string& getContent() { return (*_mContent); }

	private:
		std::string* _mContent;
		///Loads the shader file and stores the code
		/// \param filepath path to the shader file
		void LoadShader(const std::string& filepath);

	};
}
#endif