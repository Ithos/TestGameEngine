#pragma once

#ifndef GEOMETRYLIGHTFUNCTION_H
#define GEOMATRYLIGHTFUNCTION_H

#include <qmatrix4x4.h>
#include <qvector3d.h>

#include <QOpenGLShaderProgram>

#include "LightFunctionalities.h"

namespace GeometryEngine
{
	class BuffersInfo;

	namespace LightUtils
	{
		/// Class that applies a technique for the parent light
		class LightFunction
		{
		public:

			/// Constructor
			/// param parent Pointer to the LightFunctionalities that contains it 
			/// param step indicates to which function this class corresponds
			LightFunction(LightFunctionalities* parent, LightFunctions step);

			/// Copy constructor
			/// param ref Object to be copied.
			LightFunction(const LightFunction& ref) { copy(ref); }

			/// Destructor
			virtual ~LightFunction();

			/// Initializes the object
			/// param shaderManager pointer to shader manager singleton
			virtual void InitCustomShading(ShaderFiles::ShaderManager* shaderManager);
			/// Sets the target for this function
			/// param target Light to be rendered
			virtual void SetTargetLight(GeometryWorldItem::GeometryLight::Light* target) { mpTargetLight = target; }

			/// Apply light technique. Abstract method to be implemented by child classes.
			/// param vertexBuf Pointer to the vertex buffer
			/// param indexBuffer Pointer to the index buffer
			/// param projectionMatrix Projection matrix to be used 
			/// param viewMatrix ViewMatrix to be used
			/// param modelMatrix ModelMatrix of the object to be drawn or identity if it's not necessary
			/// param totalVertexNum Total amount of vertices
			/// param totalIndexNum Total amount of indices
			virtual void ApplyFunction(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const QMatrix4x4& modelMatrix,
				unsigned int totalVertexNum, unsigned int totalIndexNum);

			/// Abstract method. Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual LightFunction* Clone(LightFunctionalities* parent, LightFunctions step) const = 0;

		protected:
			ShaderFiles::ShaderManager* mpShaderManager;
			QOpenGLShaderProgram* mpProgram; // Custom shader

			LightFunctionalities* mpParentInterface;
			GeometryWorldItem::GeometryLight::Light* mpTargetLight;

			std::string mVertexShaderKey;
			std::string mFragmentShaderKey;

			/// Abstract method. Sets the shaders that should be loaded
			virtual void initShaders() = 0;

			/// Loads and compiles material shader program
			virtual void initProgram();

			/// Abstract method. Sends parameters to the shaders.
			/// param projectionMatrix Projection matrix to be used 
			/// param viewMatrix ViewMatrix to be used
			/// param modelMatrix ModelMatrix of the object to be drawn or identity if it's not necessary
			virtual void setProgramParameters(const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const QMatrix4x4& modelMatrix) = 0;

			/// Abstract method. Binds shaders and draws.
			/// param vertexBuf Vertex buffer
			/// param indexBuf IndexBuffer
			/// param totalVertexNum Number of vetices
			/// param titalIndexNum Number of indices
			virtual void applyShader(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) = 0;

			/// Sets a new parent interface and adds the step to that interface
			/// param parent Pointer to the LightFunctionalities that contains it 
			/// param step indicates to which function this class corresponds
			virtual void AddToInterface(LightFunctionalities* parent, LightFunctions step);

			/// Copies the data of a CustomShadingStep object to the current object
			/// param ref CustomShadingStep to be copied
			virtual void copy(const LightFunction& ref);

		};
	}
}

#endif