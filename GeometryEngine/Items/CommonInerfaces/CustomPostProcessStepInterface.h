#pragma once

#ifndef GEOMETRYCUSTOMPOSTPROCESSSTEPINTERFACE_H
#define GEOMETRYCUSTOMPOSTPROCESSSTEPINTERFACE_H

#include <map>

#include <QOpenGLBuffer>

#include <ConfigurationManager.h>
#include <ShaderManager.h>

namespace GeometryEngine
{
	namespace GeometryPostProcess
	{
		class PostProcess;
	};

	class GBufferTextureInfo;

	namespace CustomShading
	{
		class CustomPostProcessStep;

		/// All posible post process steps to override
		enum CustomPostProcessSteps
		{
			SECOND_STEP
		};

		/// Interface and manager class for post processes that require multiple steps
		class CustomPostProcessStepInterface
		{

		public:
			/// Constructor
			CustomPostProcessStepInterface() : mpShaderManager(nullptr), mpConfInstance(nullptr) {}
			/// Copy constructor. Automatiaclly initializes the shading steps after copying them.
			/// \param ref Object to be copied.
			CustomPostProcessStepInterface(const CustomPostProcessStepInterface& ref);
			/// Destructor
			virtual ~CustomPostProcessStepInterface();
			/// Abstract method. Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual CustomPostProcessStepInterface* Clone() const { return new CustomPostProcessStepInterface((*this)); }
			/// Adds a new CustomShadingStep T on the selected step slot
			/// \param step Step key where the CustomShadingStep object will be added
			template <typename T > CustomPostProcessStep* AddNewPostProcessStep(const CustomPostProcessSteps& step) { return new T(this, step); }
			/// Adds a new CustomShadingStep on the selected function slot
			/// \param value CustomShadingStep to be added to the manager
			/// \param key Step key where the CustomShadingStep object will be added
			bool AddPostProcessStep(CustomPostProcessStep* value, const CustomPostProcessSteps& key);
			/// Removes custom step from the manager
			/// \param key Custom step key to be removed
			bool RemovePostProcessStep(const CustomPostProcessSteps& key);
			/// Checks if the manager contains a custom step key
			/// \param step Key to check
			bool ContainsStep(const CustomPostProcessSteps& step) { return mStepMap.find(step) != mStepMap.end(); }
			/// Sets the target for every shading step in this interface
			/// \param target PostProcess to be rendered
			void SetTargetPostProcess(GeometryPostProcess::PostProcess* target);
			/// Render step
			/// \param step Step to be rendered
			/// \param gBuffTexInfo geometry buffer data
			virtual bool RenderStep(const CustomPostProcessSteps& step, const GBufferTextureInfo& gBuffTexInfo);
			/// Calls init on all the shading steps in the map. This method should be called before trying to use any shading step.
			void InitCustomSteps();
			/// Calls init on the selected shading step. Steps should be initialited before trying to use them.
			bool InitCustomStep(const CustomPostProcessSteps& key);

		protected:
			ShaderFiles::ShaderManager* mpShaderManager;
			Configuration::ConfigurationManager* mpConfInstance;

			std::map<CustomPostProcessSteps, CustomPostProcessStep*> mStepMap;

			/// Initializes managers and shaders
			virtual void getManagers();
			/// Copies the data of a CustomPostProcessStepInterface object to the current object
			/// \param ref Material to be copied
			virtual void copy(const CustomPostProcessStepInterface& ref);

		};
	}
}

#endif // !GEOMETRYCUSTOMPOSTPROCESSSTEPINTERFACE_H
