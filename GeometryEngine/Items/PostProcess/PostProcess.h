#pragma once

#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include "../GeometryItem.h"
#include "../CommonItemParameters.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryItem
		{
			class GeometryItem;
		}
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
			/// param boundingGeometry The post process will be applied to every part of the screen where the bounding geometry is drawn. Usually it should be a rectangle directly in front of the camera.
			PostProcess(const GeometryWorldItem::GeometryItem::GeometryItem & boundingGeometry);
			/// Copy constructor
			/// param ref Const reference to PostProcess to be copied
			PostProcess(const PostProcess& ref);
			/// Destructor
			virtual ~PostProcess();
			/// Applies the post process
			/// param gBuffTexInfo Geometry buffer data
			void ApplyPostProcess(const GBufferTextureInfo& gBuffTexInfo);
			virtual GeometryEngine::GeometryWorldItem::WorldItem* const GetBoundingGeometry() { return mpBoundingGeometry; }
			/// Abstract method. Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual PostProcess* Clone() const = 0;
		protected:
			GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem* mpBoundingGeometry;
			QOpenGLShaderProgram* mpProgram;
			Configuration::ConfigurationManager* mpConfInstance;
			ShaderFiles::ShaderManager* mpShaderManager;
			std::string mVertexShaderKey;
			std::string mFragmentShaderKey;

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
			/// param ref PostProcess to be copied
			virtual void copy(const PostProcess& ref);
		};
	}
}
#endif