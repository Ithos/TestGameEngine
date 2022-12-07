#pragma once

#ifndef GEOMETRYMATERIALPOSTPROCESSINTERFACE_H
#define GEOMETRYMATERIALPOSTPROCESSINTERFACE_H

#include <map>
#include <qmatrix4x4.h>

#include <QOpenGLBuffer>

#include <ConfigurationManager.h>
#include <ShaderManager.h>

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		class Material;
	}

	class GBufferTextureInfo;

	namespace CustomShading
	{
		/// Constants to order the material post process steps
		enum MaterialPostProcessSteps
		{
			FIRST_POST_PROCESS,
			SECOND_POST_PROCESS,
			THIRD_POST_PROCESS,
			FOURTH_POST_PROCESS,
			FIFTH_POST_PROCESS,
			SIXTH_POST_PROCESS,
			SEVENTH_POST_PROCESS,
			EIGTH_POST_PROCESS,
			NINTH_POST_PROCESS,
			TENTH_POST_PROCESS
		};


		class MaterialPostProcessStep;

		/// String constants that reference shader keys 
		struct MaterialPostProcessConstants
		{
			static const std::string POST_PROCESS_VERTEX_SHADER;
			static const std::string BLUR_POST_PROCESS_FRAGMENT_SHADER;
			static const std::string GREY_SCALE_PROCESS_FRAGMENT_SHADER;
		};

		/// Interface for objects that implement custom shadow map calculations
		class MaterialPostProcessInterface
		{
		public:
			/// Constructor
			MaterialPostProcessInterface() : mpShaderManager(nullptr), mpConfInstance(nullptr) {}
			/// Copy constructor. Automatiaclly initializes the shading steps after copying them.
			/// \param ref Object to be copied.
			MaterialPostProcessInterface(const MaterialPostProcessInterface& ref);
			/// Destructor
			virtual ~MaterialPostProcessInterface();
			/// Abstract method. Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual MaterialPostProcessInterface* Clone() const { return new MaterialPostProcessInterface((*this)); }
			/// Adds a new MaterialPostProcessStep T on the selected step slot
			/// \param step Step key where the MaterialPostProcessSteps object will be added
			template <typename T > MaterialPostProcessStep* AddNewPostProcessStep(MaterialPostProcessSteps step) { return new T(this, step); }
			/// Adds a new MaterialPostProcessStep on the selected function slot
			/// \param value MaterialPostProcessStep to be added to the manager
			/// \param key Step key where the MaterialPostProcessSteps object will be added
			bool AddPostProcessStep(MaterialPostProcessStep* value, MaterialPostProcessSteps key);
			/// Removes custom step from the manager
			/// \param key Custom step key to be removed
			bool RemovePostProcessStep(MaterialPostProcessSteps key);
			/// Checks if the manager contains a custom step key
			/// \param step Key to check
			bool ContainsStep(MaterialPostProcessSteps step) { return mStepMap.find(step) != mStepMap.end(); }
			/// Returns the number of times that this post process must beexecuted
			/// \param step Post process key
			int GetStepIterations(MaterialPostProcessSteps step);
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
			/// \param gBuffTexInfo geometry buffer data
			virtual bool RenderStep(MaterialPostProcessSteps step, QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& modelViewProjection,
				unsigned int totalVertexNum, unsigned int totalIndexNum, const GBufferTextureInfo& gBuffTexInfo);
			/// Calls init on all the shading steps in the map. This method should be called before trying to use any shading step.
			void InitPostProcessSteps();
			/// Calls init on the selected shading step. Steps should be initialited before trying to use them.
			bool InitPostProcessStep(MaterialPostProcessSteps key);
			/// Returns all steps ordered by their key
			const std::map<MaterialPostProcessSteps, MaterialPostProcessStep*>& GetSteps() { return mStepMap; }
		protected:
			ShaderFiles::ShaderManager* mpShaderManager;
			Configuration::ConfigurationManager* mpConfInstance;

			std::map<MaterialPostProcessSteps, MaterialPostProcessStep*> mStepMap;

			/// Initializes managers and shaders
			virtual void getManagers();
			/// Copies the data of a Material object to the current object
			/// \param ref Material to be copied
			virtual void copy(const MaterialPostProcessInterface& ref);

		};

	}
}
#endif //GEOMETRYMATERIALPOSTPROCESSINTERFACE_H
