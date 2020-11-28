#pragma once

#ifndef __SHADERFILES_SHADERMANAGER_H
#define __SHADERFILES_SHADERMANAGER_H

#include "ConfigFileFunctions.h"
#include "Shader.h"
#include <iostream>
#include <cassert>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
///namespace for classes that manage shader files
namespace ShaderFiles
{
	///Struct that contains default shader keys and the key-value separator for the conf files
	struct ShaderManagerConstants
	{
		static const std::string TEST_VERTEX_SHADER;
		static const std::string TEST_FRAGMENT_SHADER;
		static const char SEPARATOR;
	};

	///Singleon that manages shader files
	class ShaderManager
	{
	public:
		/// Returns the singleton instance if null the singleton has not been initialized
		/// \return singleton instance
		static ShaderManager* GetActiveInstance() { return _mpInstance; }
		///Initializes the singleton instance if it has not been initialized and returns the singleton instance
		/// \param vertexFolder path of the folder that contains vertex shader files
		/// \param fragmentFolder path of the folder that contains fragment shader files
		/// \param vertexConf name of the vertex shader conf file in which the keys for the vertex shader files are defined
		/// \param fragmentConf name of the fragment shader conf file in which the keys for the fragment shader files are defined
		/// \return singleton instance
		static ShaderManager* GetInstance(const std::string& vertexFolder, const std::string& fragmentFolder, const std::string& vertexConf, const std::string& fragmentConf);
		///Destroys the singleton instance if it has been initialized
		static void DestroyInstance();

		///Loads a shader from a shader key into the singleton map. 
		///An assert will be triggered if the shader has been added already or if the shader key has not been defined
		/// \param shader shader key
		void LoadShader(const std::string& shader);
		///Removes a shader from the singleton map. 
		///an assert will be triggered if the shader has not been loaded previously.
		/// \param shader shader key
		void UnloadShader(const std::string& shader);
		///Unloads all shaders from the singleton map
		void UnloadAll();
		///Checks if a key has been loaded into the singleton map. It doesn't check if the key has beed defined or not.
		/// \param shader shader key
		/// \return true if the key has beed loaded into the map
		bool IsLoaded(const std::string& shader);

		///Gets the code of a shader already present in the singleton map. An assert will be triggered if the shader key has not been loaded.
		/// \param shader shader key
		/// \return shader code
		const std::string& GetLoadedShaderContent(const std::string& shader);
		///Gets a shader loaded into the singleton map. An assert will be triggered if the shader key has not been loaded.
		/// \param shader shader key
		/// \return shader object
		Shader& GetLoadedShader(const std::string& shader);

	private:
		///Singleton instance
		static ShaderManager* _mpInstance;
		///Shader map
		std::map<std::string, Shader*> _mLoadedShaders;
		///Shader kay value map
		std::map<std::string, std::string> _mShadersDirs;
		std::string _mVertexFolder;
		std::string _mFragmentFolder;
		std::string _mVertexConf;
		std::string _mFragmentConf;

		///Default constructor. Defined as private so it can't be used ouside of this class
		ShaderManager() {};
		///Copy constructor.Declared private so it can't be used
		ShaderManager(const ShaderManager& ref) {}
		///Constructor. Calls init.
		/// \param vertexFolder path of the folder that contains vertex shader files
		/// \param fragmentFolder path of the folder that contains fragment shader files
		/// \param vertexConf name of the vertex shader conf file in which the keys for the vertex shader files are defined
		/// \param fragmentConf name of the fragment shader conf file in which the keys for the fragment shader files are defined
		ShaderManager(const std::string& vertexFolder, const std::string& fragmentFolder, const std::string& vertexConf, const std::string& fragmentConf);
		///Destructor. Unloads all shaders.
		virtual ~ShaderManager();
		///Reads config files and fills the shader key value map
		void init();
	};
}
#endif
