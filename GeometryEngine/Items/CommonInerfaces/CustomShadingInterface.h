#pragma once

#ifndef GEOMETRYCUSTOMSHADINGINTERFACE_H
#define GEOMETRYCUSTOMSHADINGINTERFACE_H

#include <map>

#include <QOpenGLBuffer>

#include <ConfigurationManager.h>
#include <ShaderManager.h>

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		class Material;
	}

	namespace CustomShading
	{
		/// All posible shading steps to override
		enum CustomShadingSteps
		{
			CUSTOM_SHADOWMAP,
			CUSTOM_COLORMAP
		};

		class CustomShadingStep;

		/// String constants that reference shader keys 
		struct CustomShadingConstants
		{
			static const std::string POSITION_TEX_COORD_VERTEX_SHADER;
			static const std::string ALPHA_COLOR_SHADOWMAP;
			static const std::string ALPHA_TEXTURE_SHADOWMAP;
			static const std::string ALPHA_MULTI_TEXTURE_SHADOWMAP;
			static const std::string DIRECT_COLOR_MAP_VERTEX_SHADER;
			static const std::string DIRECT_COLOR_MAP_FRAGMENT_SHADER;
			static const std::string TEXTURE_COLOR_MAP_FRAGMENT_SHADER;
			static const std::string MULTI_TEXTURE_COLOR_MAP_FRAGMENT_SHADER;
		};

		/// Interface for objects that implement custom shadow map calculations
		class CustomShadingInterface
		{
		public:
			/// Constructor
			CustomShadingInterface(): mpShaderManager(nullptr), mpConfInstance(nullptr){}
			/// Copy constructor. Automatiaclly initializes the shading steps after copying them.
			/// \param ref Object to be copied.
			CustomShadingInterface(const CustomShadingInterface& ref);
			/// Destructor
			virtual ~CustomShadingInterface();
			/// Abstract method. Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual CustomShadingInterface* Clone() const { return new CustomShadingInterface((*this)); }

			/// Adds a new CustomShadingStep T on the selected step slot
			/// \param step Step key where the CustomShadingStep object will be added
			template <typename T > CustomShadingStep* AddNewShadingStep(CustomShadingSteps step) { return new T(this, step); }
			/// Adds a new CustomShadingStep on the selected function slot
			/// \param value CustomShadingStep to be added to the manager
			/// \param key Step key where the CustomShadingStep object will be added
			bool AddShadingStep(CustomShadingStep* value, CustomShadingSteps key);
			/// Removes custom step from the manager
			/// \param key Custom step key to be removed
			bool RemoveShadingStep(CustomShadingSteps key);
			/// Checks if the manager contains a custom step key
			/// \param step Key to check
			bool ContainsStep(CustomShadingSteps step) { return mStepMap.find(step) != mStepMap.end(); }

			/// Sets the target for every shading step in this interface
			/// \param target Material to be rendered
			void SetTargetMaterial(GeometryMaterial::Material* target);

			/// Render step
			/// \param step Step to be rendered
			/// \param vertexBuf Pointer to the vertex buffer
			/// \param indexBuffer Pointer to the index buffer
			/// \param modelMatrix model matrix of the item to be added to the shadow map
			/// \param totalVertexNum Total amount of vertices
			/// \param totalIndexNum Total amount of indices
			virtual bool RenderStep(CustomShadingSteps step, QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& modelViewProjection, 
				unsigned int totalVertexNum, unsigned int totalIndexNum);
			/// Calls init on all the shading steps in the map. This method should be called before trying to use any shading step.
			void InitCustomSteps();
			/// Calls init on the selected shading step. Steps should be initialited before trying to use them.
			bool InitCustomStep(CustomShadingSteps key);

		protected:
			ShaderFiles::ShaderManager* mpShaderManager;
			Configuration::ConfigurationManager* mpConfInstance;

			std::map<CustomShadingSteps, CustomShadingStep*> mStepMap;

			/// Initializes managers and shaders
			virtual void getManagers();
			/// Copies the data of a Material object to the current object
			/// \param ref Material to be copied
			virtual void copy(const CustomShadingInterface& ref);
		};
	}
}
#endif // !GEOMETRYCUSTOMSHADINGINTERFACE_H
