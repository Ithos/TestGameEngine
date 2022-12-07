#pragma once

#ifndef GEOMETRYMATERIALBLURPOSTPROCESS_H
#define GEOMETRYMATERIALBLURPOSTPROCESS_H

#include "../MaterialPostProcessStep.h"
#include "../Items/CommonItemParameters.h"

namespace GeometryEngine
{
	namespace CustomShading
	{
		/// Standard blur post process that is applied only to objects that contain the parent material
		class MaterialBlurPostProcess : public MaterialPostProcessStep
		{
		public:
			/// Constructor
			/// \param parent Pointer to the CustomShadingInterface that contains it 
			/// \param step Value that indicates at which render stepthis shading technique will be used
			MaterialBlurPostProcess(MaterialPostProcessInterface* parent, MaterialPostProcessSteps step) : MaterialPostProcessStep(parent, step), mIsVertical(false), mSwapOriginTexture(false){};

			/// Copy constructor
			/// \param ref Object to be copied.
			MaterialBlurPostProcess(const MaterialBlurPostProcess& ref) { copy(ref); }

			/// Destructor
			virtual ~MaterialBlurPostProcess() {};

			/// Abstract method. Factory method. Creates a copy of this object
			/// \param parent Pointer to the CustomShadingInterface that contains it 
			/// \param step Value that indicates at which render stepthis shading technique will be used
			/// \return Pointer to a copy of this object
			virtual MaterialBlurPostProcess* Clone(MaterialPostProcessInterface* parent, MaterialPostProcessSteps step) const {
				MaterialBlurPostProcess* cloned = new MaterialBlurPostProcess((*this));
				cloned->AddToInterface(parent, step);
				return cloned;
			}

			bool GetVertical() const { return mIsVertical; }
			void SetVertical(bool IsVertical) { mIsVertical = IsVertical; }
			bool GetOrigin() const { return mSwapOriginTexture; }
			void SetOrigin(bool swapOrigin) { mSwapOriginTexture = swapOrigin; }

			void SetSecondStepParameters() { SetVertical(true); SetOrigin(true); }

		protected:
			bool mIsVertical;
			bool mSwapOriginTexture;

			/// Sets the shaders that should be loaded
			virtual void initShaders() override
			{
				mVertexShaderKey = MaterialPostProcessConstants::POST_PROCESS_VERTEX_SHADER;
				mFragmentShaderKey = MaterialPostProcessConstants::BLUR_POST_PROCESS_FRAGMENT_SHADER;
			}

			/// Sends parameters to the shaders.
			/// \param modelViewProjectionMatrix Projection matrix
			/// \param gBuffTexInfo geometry buffer data
			virtual void setProgramParameters(const QMatrix4x4& modelViewProjectionMatrix, const GBufferTextureInfo& gBuffTexInfo) override
			{
				assert(mpProgram != nullptr && "Material blur post process program not found");
				assert(mpTargetMaterial != nullptr && "Material blur post process target material not found");
				{
					// Set matrices
					mpProgram->setUniformValue("modelViewProjectionMatrix", modelViewProjectionMatrix);
					unsigned int sourceTextureLocation = this->mSwapOriginTexture ? gBuffTexInfo.TmpTexture : gBuffTexInfo.OriginTexture;
					mpProgram->setUniformValue("mTexture", sourceTextureLocation);
					mpProgram->setUniformValue("mTextureSize", gBuffTexInfo.TextureSize);
					mpProgram->setUniformValue("isVertical", this->mIsVertical);
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
				assert(mpProgram != nullptr && "Material blur post process --> Shader post process program Null");
				assert(mpTargetMaterial != nullptr && "Material blur post process target material not found");
				{
					// Tell OpenGL which VBOs to use
					vertexBuf->bind();
					indexBuf->bind();

					int vertexLocation = mpProgram->attributeLocation("posAttr");
					mpProgram->enableAttributeArray(vertexLocation);
					mpProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

					glDrawElements(GL_TRIANGLE_STRIP, totalIndexNumber, GL_UNSIGNED_SHORT, 0);

					mpProgram->disableAttributeArray(vertexLocation);

					vertexBuf->release();
					indexBuf->release();
				}
			}

			/// Copies the data of a MaterialBlurPostProcess object to the current object
			/// \param ref MaterialBlurPostProcess to be copied
			virtual void copy(const MaterialPostProcessStep& ref) override
			{
				MaterialPostProcessStep::copy(ref);
				const MaterialBlurPostProcess & tRef = ((const MaterialBlurPostProcess&)ref);
				this->mIsVertical = tRef.mIsVertical;
				this->mSwapOriginTexture = tRef.mSwapOriginTexture;
			}
		};
	}
}

#endif // GEOMETRYMATERIALBLURPOSTPROCESS_H
