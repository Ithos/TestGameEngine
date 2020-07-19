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
		enum CustomShadingSteps
		{
			CUSTOM_SHADOWMAP,
			CUSTOM_COLORMAP
		};

		class CustomShadingStep;

		struct CustomShadingConstants
		{
			static const std::string POSITION_TEX_COORD_VERTEX_SHADER;
			static const std::string ALPHA_COLOR_SHADOWMAP;
			static const std::string ALPHA_TEXTURE_SHADOWMAP;
			static const std::string ALPHA_MULTI_TEXTURE_SHADOWMAP;
		};

		/// Interface for objects that implement custom shadow map calculations
		class CustomShadingInterface
		{
		public:
			/// Constructor
			CustomShadingInterface(): mpShaderManager(nullptr), mpConfInstance(nullptr){}
			/// Copy constructor. Automatiaclly initializes the shading steps after copying them.
			/// param ref Object to be copied.
			CustomShadingInterface(const CustomShadingInterface& ref);
			/// Destructor
			virtual ~CustomShadingInterface();
			/// Abstract method. Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual CustomShadingInterface* Clone() const { return new CustomShadingInterface((*this)); }

			template <typename T > CustomShadingStep* AddNewShadingStep(CustomShadingSteps step) { return new T(this, step); }
			bool AddShadingStep(CustomShadingStep* value, CustomShadingSteps key);
			bool ContainsStep(CustomShadingSteps step) { return mStepMap.find(step) != mStepMap.end(); }

			/// Sets the target for every shading step in this interface
			/// param target Material to be rendered
			void SetTargetMaterial(GeometryMaterial::Material* target);

			/// Render step
			/// param step Step to be rendered
			/// param vertexBuf Pointer to the vertex buffer
			/// param indexBuffer Pointer to the index buffer
			/// param modelMatrix model matrix of the item to be added to the shadow map
			/// param totalVertexNum Total amount of vertices
			/// param totalIndexNum Total amount of indices
			virtual bool RenderStep(CustomShadingSteps step, QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& modelViewProjection, 
				unsigned int totalVertexNum, unsigned int totalIndexNum);
			/// Calls init on all the shading steps in the map. This method should be called before trying to use any shading step.
			void InitCustomSteps();

		protected:
			ShaderFiles::ShaderManager* mpShaderManager;
			Configuration::ConfigurationManager* mpConfInstance;

			std::map<CustomShadingSteps, CustomShadingStep*> mStepMap;

			/// Initializes managers and shaders
			virtual void getManagers();
			/// Copies the data of a Material object to the current object
			/// param ref Material to be copied
			virtual void copy(const CustomShadingInterface& ref);
		};
	}
}
#endif // !GEOMETRYCUSTOMSHADINGINTERFACE_H
