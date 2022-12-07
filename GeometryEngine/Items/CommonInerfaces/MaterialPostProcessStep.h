#pragma once

#ifndef GEOMETRYMATERIALPOSTPROCESSSTEP_H
#define GEOMETRYMATERIALPOSTPROCESSSTEP_H

#include <QOpenGLShaderProgram>

#include <ConfigurationManager.h>
#include <ShaderManager.h>

#include "MaterialPostProcessInterface.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryItem
		{
			class GeometryItem;
		}
	}

	namespace GeometryMaterial
	{
		class Material;
	}

	class GBufferTextureInfo;

	namespace CustomShading
	{
		/// Component class that executes a post process operation on target material T
		class MaterialPostProcessStep
		{
		public:
			/// Constructor
			/// \param parent Pointer to the CustomShadingInterface that contains it 
			/// \param step Value that indicates at which render stepthis shading technique will be used
			/// \param iterations Number of times that the post process should be executed
			MaterialPostProcessStep(MaterialPostProcessInterface* parent, MaterialPostProcessSteps step, unsigned int iterations = 1);
			/// Copy constructor
			/// \param ref Object to be copied.
			MaterialPostProcessStep(const MaterialPostProcessStep& ref) { copy(ref); }
			/// Destructor
			virtual ~MaterialPostProcessStep();
			/// Initializes the object
			/// \param shaderManager pointer to shader manager singleton
			virtual void initCustomShading(ShaderFiles::ShaderManager* shaderManager);
			/// Sets the target for this shading step
			/// \param target Material to be rendered
			virtual void SetTargetMaterial(GeometryMaterial::Material* target) { mpTargetMaterial = target; }
			/// Abstract method. Binds shaders and draws.
			/// \param vertexBuf Pointer to the vertex buffer
			/// \param indexBuffer Pointer to the index buffer
			/// \param modelMatrix model matrix of the item to be added to the shadow map
			/// \param totalVertexNum Total amount of vertices
			/// \param totalIndexNum Total amount of indices
			/// \param gBuffTexInfo geometry buffer data
			virtual void ApplyPostProcess(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& modelViewProjection,
				unsigned int totalVertexNum, unsigned int totalIndexNum, const GBufferTextureInfo& gBuffTexInfo);
			/// Returns the number of iterations that the post process should be executed
			virtual unsigned int const GetIterations() { return mIterations; }
			/// Sets the number of iterations for this shading step
			/// \param iterations Number of iterations this post process should be executed
			virtual void SetTargetMaterial(unsigned int iterations) { mIterations = iterations; }
			/// Abstract method. Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual MaterialPostProcessStep* Clone(MaterialPostProcessInterface* parent, MaterialPostProcessSteps step) const = 0;

		protected:
			ShaderFiles::ShaderManager* mpShaderManager;

			MaterialPostProcessInterface* mpParentInterface;
			GeometryMaterial::Material* mpTargetMaterial;

			QOpenGLShaderProgram* mpProgram; // Custom shader
			std::string mVertexShaderKey;
			std::string mFragmentShaderKey;
			unsigned int mIterations;

			/// Protected constructor for copying objects
			MaterialPostProcessStep() {};
			/// Abstract method. Sets the shaders that should be loaded
			virtual void initShaders() = 0;
			/// Loads and compiles material shader program
			virtual void initProgram();
			/// Sets a new parent interface and adds the step to that interface
			/// \param parent Pointer to the CustomShadingInterface that contains it 
			/// \param step that indicates at which render stepthis shading technique will be used
			virtual void AddToInterface(MaterialPostProcessInterface* parent, MaterialPostProcessSteps step);
			/// Abstract method. Sends parameters to the shaders.
			/// \param modelViewProjectionMatrix Projection matrix
			/// \param gBuffTexInfo geometry buffer data
			virtual void setProgramParameters(const QMatrix4x4& modelViewProjectionMatrix, const GBufferTextureInfo& gBuffTexInfo) = 0;
			/// Abstract method. Binds shaders and draws.
			/// \param vertexBuf Vertex buffer
			/// \param indexBuf IndexBuffer
			/// \param totalVertexNum Number of vetices
			/// \param titalIndexNum Number of indices
			/// \param gBuffTexInfo geometry buffer data
			virtual void applyPostProcess(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber, const GBufferTextureInfo& gBuffTexInfo) = 0;
			/// Applies internal modifiers and applies the post process
			/// \param vertexBuf Vertex buffer
			/// \param indexBuf IndexBuffer
			/// \param totalVertexNum Number of vetices
			/// \param titalIndexNum Number of indices
			/// \param gBuffTexInfo geometry buffer data
			virtual void modifyRenderShader(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum, const GBufferTextureInfo& gBuffTexInfo);
			/// Copies the data of a CustomShadingStep object to the current object
			/// \param ref CustomShadingStep to be copied
			virtual void copy(const MaterialPostProcessStep& ref);
		};
	}
}

#endif //GEOMETRYMATERIALPOSTPROCESSSTEP_H