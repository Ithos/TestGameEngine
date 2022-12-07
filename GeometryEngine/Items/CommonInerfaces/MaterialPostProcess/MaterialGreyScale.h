#pragma once

#ifndef GEOMETRYMATERIALGREYSCALEPOSTPROCESS_H
#define GEOMETRYMATERIALGREYSCALEPOSTPROCESS_H

#include "../MaterialPostProcessStep.h"
#include "../Items/CommonItemParameters.h"

namespace GeometryEngine
{
	namespace CustomShading
	{
		/// Standard blur post process that is applied only to objects that contain the parent material
		class MaterialGreyScale : public MaterialPostProcessStep
		{
		public:
			/// Constructor
			/// \param parent Pointer to the CustomShadingInterface that contains it 
			/// \param step Value that indicates at which render stepthis shading technique will be used
			MaterialGreyScale(MaterialPostProcessInterface* parent, MaterialPostProcessSteps step) : MaterialPostProcessStep(parent, step){};

			/// Copy constructor
			/// \param ref Object to be copied.
			MaterialGreyScale(const MaterialGreyScale& ref) { copy(ref); }

			/// Destructor
			virtual ~MaterialGreyScale() {};

			/// Abstract method. Factory method. Creates a copy of this object
			/// \param parent Pointer to the CustomShadingInterface that contains it 
			/// \param step Value that indicates at which render stepthis shading technique will be used
			/// \return Pointer to a copy of this object
			virtual MaterialGreyScale* Clone(MaterialPostProcessInterface* parent, MaterialPostProcessSteps step) const {
				MaterialGreyScale* cloned = new MaterialGreyScale((*this));
				cloned->AddToInterface(parent, step);
				return cloned;
			}

		protected:

			/// Sets the shaders that should be loaded
			virtual void initShaders() override
			{
				mVertexShaderKey = MaterialPostProcessConstants::POST_PROCESS_VERTEX_SHADER;
				mFragmentShaderKey = MaterialPostProcessConstants::GREY_SCALE_PROCESS_FRAGMENT_SHADER;
			}

			/// Sends parameters to the shaders.
			/// \param modelViewProjectionMatrix Projection matrix
			/// \param gBuffTexInfo geometry buffer data
			virtual void setProgramParameters(const QMatrix4x4& modelViewProjectionMatrix, const GBufferTextureInfo& gBuffTexInfo) override
			{
				assert(mpProgram != nullptr && "Material grey scale post process program not found");
				assert(mpTargetMaterial != nullptr && "Material grey scale post process target material not found");
				{
					// Set matrices
					mpProgram->setUniformValue("modelViewProjectionMatrix", modelViewProjectionMatrix);
					mpProgram->setUniformValue("mTexture", gBuffTexInfo.OriginTexture);
					mpProgram->setUniformValue("mTextureSize", gBuffTexInfo.TextureSize);
				}
			}

			/// Abstract method. Binds shaders and draws.
			/// \param vertexBuf Vertex buffer
			/// \param indexBuf IndexBuffer
			/// \param totalVertexNum Number of vetices
			/// \param t0talIndexNum Number of indices
			/// \param gBuffTexInfo geometry buffer data
			virtual void applyPostProcess(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber,
				const GBufferTextureInfo& gBuffTexInfo) override
			{
				assert(mpProgram != nullptr && "Material grey scale post process --> Shader post process program Null");
				assert(mpTargetMaterial != nullptr && "Material grey scale post process target material not found");
				{
					// Tell OpenGL which VBOs to use
					vertexBuf->bind();
					indexBuf->bind();

					int vertexLocation = mpProgram->attributeLocation("posAttr");
					mpProgram->enableAttributeArray(vertexLocation);
					mpProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

					// Draw light
					glDrawElements(GL_TRIANGLE_STRIP, totalIndexNumber, GL_UNSIGNED_SHORT, 0);

					mpProgram->disableAttributeArray(vertexLocation);

					vertexBuf->release();
					indexBuf->release();
				}
			}

		};
	}
}


#endif // !GEOMETRYMATERIALGREYSCALEPOSTPROCESS_H
