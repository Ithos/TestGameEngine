#pragma once

#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include <string>

class QOpenGLShaderProgram;

namespace Configuration
{
	class ConfigurationManager;
}

namespace ShaderFiles
{
	class ShaderManager;
}

namespace GeometryEngine
{
	class GBufferTextureInfo;

	namespace GeometryWorldItem
	{
		namespace GeometryItem
		{
			class GeometryItem;
		}
	}

	namespace CustomShading
	{
		class CustomPostProcessStepInterface;
	}

	///namespace for all post processes
	namespace GeometryPostProcess
	{
		/// Struct that contains string constants with that represent keys for PostProcess shaders
		struct PostProcessShaderConstants
		{
			static const std::string POST_PROCESS_VERTEX_SHADER;
			static const std::string BLUR_POST_PROCESS_FRAGMENT_SHADER;
			static const std::string GREY_SCALE_PROCESS_FRAGMENT_SHADER;
		};

		/// Base class for all post processes
		class PostProcess
		{
		public:
			/// Construct
			/// \param boundingGeometry The post process will be applied to every part of the screen where the bounding geometry is drawn. Usually it should be a rectangle directly in front of the camera.
			/// \param customShading Custom shading step manager.
			/// \param componentManager Object that manages and contains the custom components for this postprocess
			/// \param iterations Number of iterations that this posprocess should be executed
			PostProcess(const GeometryWorldItem::GeometryItem::GeometryItem & boundingGeometry, const CustomShading::CustomPostProcessStepInterface* const componentManager = nullptr, 
				unsigned int iterations = 1);
			/// Copy constructor
			/// \param ref Const reference to PostProcess to be copied
			PostProcess(const PostProcess& ref);
			/// Destructor
			virtual ~PostProcess();
			/// Applies the default post process step
			/// \param gBuffTexInfo Geometry buffer data
			virtual void ApplyPostProcess(const GBufferTextureInfo& gBuffTexInfo); 
			/// Returns a pointer to the geometry in which the post process will be applied
			virtual GeometryWorldItem::GeometryItem::GeometryItem* const GetBoundingGeometry() { return mpBoundingGeometry; }
			/// Returns the number of iterations this posprocess should be executed
			unsigned int GetIterations() { return mIterations; }
			/// Stablishes the number of iterations this posprocess should be executed, 0 means that it won't be executed at all.
			void SetIterations(unsigned int iterations) { mIterations = iterations; }
			/// Returns the componant manager for the post process
			CustomShading::CustomPostProcessStepInterface* GetComponentManager() { return mpComponentManager; }
			/// Abstract method. Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual PostProcess* Clone() const = 0;
		protected:
			GeometryWorldItem::GeometryItem::GeometryItem* mpBoundingGeometry;
			QOpenGLShaderProgram* mpProgram;
			Configuration::ConfigurationManager* mpConfInstance;
			ShaderFiles::ShaderManager* mpShaderManager;
			CustomShading::CustomPostProcessStepInterface* mpComponentManager;
			std::string mVertexShaderKey;
			std::string mFragmentShaderKey;
			unsigned int mIterations;

			/// Gets singletons instances and call other inits
			void initPostProcess();
			/// Loads and compiles shaders
			virtual void initPostProcessProgram();
			/// Abstract method. This method should stablish the shaders keys.
			virtual void initPostProcessShaders() = 0;
			/// Abstract method. This method should send parameters to the shaders.
			virtual void setProgramParameters(const GBufferTextureInfo& gBuffTexInfo) = 0;
			/// Abstract method. Renders the PostPRocess
			virtual void applyFilter() = 0;
			/// Copies the data of a PostProcess object to the current object
			/// \param ref PostProcess to be copied
			virtual void copy(const PostProcess& ref);
		};
	}
}
#endif