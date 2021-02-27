#pragma once

#ifndef GEOMETRYCUSTOMSHADINGSTEP_H
#define GEOMETRYCUSTOMSHADINGSTEP_H

#include <QOpenGLShaderProgram>

#include <ConfigurationManager.h>
#include <ShaderManager.h>

#include "CustomShadingInterface.h"

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

	/// Namespace for manager and component classes involved in custom shading
	namespace CustomShading
	{
		/// Component class that executes a shading operation on target material T
		class CustomShadingStep
		{
		public:
			
			/// Constructor
			/// \param parent Pointer to the CustomShadingInterface that contains it 
			/// \param step Value that indicates at which render stepthis shading technique will be used
			CustomShadingStep(CustomShadingInterface* parent, CustomShadingSteps step);

			/// Copy constructor
			/// \param ref Object to be copied.
			CustomShadingStep(const CustomShadingStep& ref) { copy(ref); }

			/// Destructor
			virtual ~CustomShadingStep();

			/// Initializes the object
			/// \param shaderManager pointer to shader manager singleton
			virtual void initCustomShading(ShaderFiles::ShaderManager* shaderManager);

			/// Sets the target for this shading step
			/// \param target Material to be rendered
			virtual void SetTargetMaterial(GeometryMaterial::Material* target) { mpTargetMaterial = target; }

			/// Render shaders
			/// \param vertexBuf Pointer to the vertex buffer
			/// \param indexBuffer Pointer to the index buffer
			/// \param modelViewProjection combined matrices to be used 
			/// \param totalVertexNum Total amount of vertices
			/// \param totalIndexNum Total amount of indices
			virtual void RenderShaders(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& modelViewProjection, unsigned int totalVertexNum, unsigned int totalIndexNum);

			/// Abstract method. Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual CustomShadingStep* Clone(CustomShadingInterface* parent, CustomShadingSteps step) const = 0;

		protected:
			ShaderFiles::ShaderManager* mpShaderManager;

			CustomShadingInterface* mpParentInterface;
			GeometryMaterial::Material* mpTargetMaterial;

			QOpenGLShaderProgram* mpProgram; // Custom shader
			std::string mVertexShaderKey;
			std::string mFragmentShaderKey;

			/// Abstract method. Sets the shaders that should be loaded
			virtual void initShaders() = 0;

			/// Loads and compiles material shader program
			virtual void initProgram();

			/// Sets a new parent interface and adds the step to that interface
			/// \param parent Pointer to the CustomShadingInterface that contains it 
			/// \param step that indicates at which render stepthis shading technique will be used
			virtual void AddToInterface(CustomShadingInterface* parent, CustomShadingSteps step);

			/// Abstract method. Sends parameters to the shaders.
			/// \param projection Projection matrix
			/// \param view View matrix			 
			/// \param geometry Geometry item to be drawn
			virtual void setProgramParameters(const QMatrix4x4& modelViewProjectionMatrix) = 0;
			/// Abstract method. Binds shaders and draws.
			/// \param vertexBuf Vertex buffer
			/// \param indexBuf IndexBuffer
			/// \param totalVertexNum Number of vetices
			/// \param titalIndexNum Number of indices
			virtual void drawShader(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) = 0;
			/// Applies internal modifiers and calls renderShadowMap
			/// \param vertexBuf Vertex buffer
			/// \param indexBuf IndexBuffer
			/// \param totalVertexNum Number of vetices
			/// \param titalIndexNum Number of indices
			virtual void modifyRenderShader(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum);
			/// Copies the data of a CustomShadingStep object to the current object
			/// \param ref CustomShadingStep to be copied
			virtual void copy(const CustomShadingStep& ref);
		};
	}
}

#endif