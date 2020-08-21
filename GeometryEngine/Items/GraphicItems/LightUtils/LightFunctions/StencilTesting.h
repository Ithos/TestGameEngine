#pragma once

#ifndef GEOMETRYSTENCILTESTING_H
#define GEOMETRYSTENCILTESTING_H

#include "../GeometryEngine/Items/GeometryItem.h"
#include "../LightFunction.h"

namespace GeometryEngine
{
	namespace LightUtils
	{
		/// Class that performs the stencil test for the light
		template<class T>
		class StencilTesting : public LightFunction
		{
		public:
			/// Constructor
			/// param parent Pointer to the CustomShadingInterface that contains it 
			/// param Value that indicates at which render stepthis shading technique will be used
			StencilTesting(LightFunctionalities* parent, LightFunctions step) : LightFunction(parent, step) {};

			/// Copy constructor
			/// param ref Object to be copied.
			StencilTesting(const LightFunction& ref) { copy(ref); }

			/// Destructor
			virtual ~StencilTesting() {};

			/// Abstract method. Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual StencilTesting* Clone(LightFunctionalities* parent, LightFunctions step) const {
				StencilTesting* cloned = new StencilTesting((*this));
				cloned->AddToInterface(parent, step);
				return cloned;
			}

			/// Apply light technique.
			/// param vertexBuf Expected null as it isn't used
			/// param indexBuffer Expected null as it isn't used
			/// param projectionMatrix Projection matrix to be used 
			/// param viewMatrix ViewMatrix to be used
			/// param modelMatrix ModelMatrix of the object to be drawn or identity if it's not necessary
			/// param totalVertexNum Expected 0 as it isn't used
			/// param totalIndexNum Expected 0 as it isn't used
			virtual void ApplyFunction(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const QMatrix4x4& modelMatrix,
				unsigned int totalVertexNum, unsigned int totalIndexNum) override
			{
				assert(mpProgram != nullptr && "No shader program found");
				{
					// Bind shader pipeline for use
					if (!mpProgram->bind())
					{
						assert(false && "Shader failed to bind");
					}

					setProgramParameters(projectionMatrix, viewMatrix, modelMatrix);
					GeometryWorldItem::GeometryItem::GeometryItem* boundingBox = ((T*)mpTargetLight)->GetBoundingGeometry();
					applyShader(boundingBox->GetArrayBuffer(), boundingBox->GetIndexBuffer(), boundingBox->GetVertexNumber(), boundingBox->GetIndexNumber());
				}
			}

		protected:
			/// Sets the shaders that should be loaded
			virtual void initShaders() override
			{
				mVertexShaderKey = LightFuntionalitiesConstants::POSITION_VERTEX_SHADER;
				mFragmentShaderKey = LightFuntionalitiesConstants::NULL_FRAGMENT_SHADER;
			}

			/// Sends parameters to the shader.
			/// param projectionMatrix Projection matrix to be used 
			/// param viewMatrix ViewMatrix to be used
			/// param modelMatrix ModelMatrix of the object to be drawn or identity if it's not necessary
			virtual void setProgramParameters(const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const QMatrix4x4& modelMatrix) override
			{
				assert(mpProgram != nullptr && "Stencil Test shadow map program not found");
				assert(mpTargetLight != nullptr && "Stencil Test target light not found");
				{
					mpProgram->setUniformValue("mModelViewProjectionMatrix", projectionMatrix * viewMatrix * ((T*)mpTargetLight)->GetBoundingGeometry()->GetModelMatrix());
				}
			}

			/// Binds shaders and draws.
			/// param vertexBuf Vertex buffer
			/// param indexBuf IndexBuffer
			/// param totalVertexNum Number of vetices
			/// param titalIndexNum Number of indices
			virtual void applyShader(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber)
			{
				assert(mpProgram != nullptr && "Stencil Test Shadow Map --> Shader ShadowMap program Null");
				assert(mpTargetLight != nullptr && "Stencil Test target light not found");
				{
					// Tell OpenGL which VBOs to use
					vertexBuf->bind();
					indexBuf->bind();

					// Tell OpenGL programmable pipeline how to locate vertex position data
					int vertexLocation = mpProgram->attributeLocation("posAttr");
					mpProgram->enableAttributeArray(vertexLocation);
					mpProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

					// Draw light
					glDrawElements(GL_TRIANGLE_STRIP, totalIndexNumber, GL_UNSIGNED_SHORT, 0);
				}
			}

		};
	}
}

#endif