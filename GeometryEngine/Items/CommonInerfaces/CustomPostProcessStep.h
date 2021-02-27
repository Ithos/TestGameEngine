#pragma once

#ifndef GEOMETRYCUSTOMPOSTPROCESSSTEP_H
#define GEOMETRYCUSTOMPOSTPROCESSSTEP_H

#include <QOpenGLShaderProgram>

#include "CustomPostProcessStepInterface.h"

namespace GeometryEngine
{
	namespace GeometryPostProcess
	{
		class PostProcess;
	};

	class GBufferTextureInfo;

	namespace CustomShading
	{
		/// Component class that executes a shading step for target post process T
		class CustomPostProcessStep
		{
		public:
			/// Constructor
			/// \param parent Pointer to the CustomShadingInterface that contains it 
			/// \param step Value that indicates at which render stepthis shading technique will be used
			CustomPostProcessStep(CustomPostProcessStepInterface* parent, CustomPostProcessSteps step);
			/// Copy constructor
			/// \param ref Object to be copied.
			CustomPostProcessStep(const CustomPostProcessStep& ref) { copy(ref); }
			/// Destructor
			virtual ~CustomPostProcessStep();

			/// Initializes the object
			/// \param shaderManager pointer to shader manager singleton
			virtual void initCustomShading(ShaderFiles::ShaderManager* shaderManager);

			/// Sets the target for this shading step
			/// \param target Process to be rendered
			virtual void SetTargetPostProcess(GeometryPostProcess::PostProcess* target) { mpTargetProcess = target; }

			/// Renders the post process step
			/// \param gBuffTexInfo Geometry buffer data
			virtual void ApplyPostProcessStep(const GBufferTextureInfo& gBuffTexInfo);

			/// Abstract method. Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual CustomPostProcessStep* Clone(CustomPostProcessStepInterface* parent, CustomPostProcessSteps step) const = 0;

		protected:
			ShaderFiles::ShaderManager* mpShaderManager;
			CustomPostProcessStepInterface* mpParentInterface;
			GeometryPostProcess::PostProcess* mpTargetProcess;

			QOpenGLShaderProgram* mpProgram; // Custom shader
			std::string mVertexShaderKey;
			std::string mFragmentShaderKey;

			/// Abstract method. Sets the shaders that should be loaded
			virtual void initShaders() = 0;
			/// Loads and compiles material shader program
			virtual void initProgram();
			/// Sets a new parent interface and adds the step to that interface
			/// \param parent Pointer to the CustomPostProcessStepInterface that contains it 
			/// \param step that indicates at which render stepthis shading technique will be used
			virtual void AddToInterface(CustomPostProcessStepInterface* parent, CustomPostProcessSteps step);
			/// Abstract method. This method should send parameters to the shaders.
			/// \param gBuffTexInfo geometry buffer data
			virtual void setProgramParameters(const GBufferTextureInfo& gBuffTexInfo) = 0;
			/// Abstract method. Renders the PostProcess step
			virtual void applyFilter() = 0;
			/// Copies the data of a CustomShadingStep object to the current object
			/// \param ref CustomPostProcessStep to be copied
			virtual void copy(const CustomPostProcessStep& ref);

		};
	}
}

#endif // !GEOMETRYCUSTOMPOSTPROCESSSTEP_H
