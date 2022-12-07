#pragma once

#ifndef GEOMETRYALPHACOLORSHADOWMAP_H
#define GEOMETRYALPHACOLORSHADOWMAP_H

#include "../CustomShadingStep.h"

namespace GeometryEngine
{
	namespace CustomShading
	{
        /// Custom shadowmap calculation for alpha color materials
		template<class T>
		class AlphaColorShadowMap : public CustomShadingStep
		{
		public:

			/// Constructor
			/// \param parent Pointer to the CustomShadingInterface that contains it 
			/// \param step Value that indicates at which render stepthis shading technique will be used
			AlphaColorShadowMap(CustomShadingInterface* parent, CustomShadingSteps step) : CustomShadingStep(parent, step) {};

			/// Copy constructor
			/// \param ref Object to be copied.
			AlphaColorShadowMap(const CustomShadingStep& ref) { copy(ref); }

			/// Destructor
			virtual ~AlphaColorShadowMap() {};

			/// Abstract method. Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual AlphaColorShadowMap* Clone(CustomShadingInterface* parent, CustomShadingSteps step) const { 
				AlphaColorShadowMap* cloned = new AlphaColorShadowMap((*this)); 
				cloned->AddToInterface(parent, step);
				return cloned;
			}

		protected:
			/// Sets the shaders that should be loaded
			virtual void initShaders() override
			{
				mVertexShaderKey = CustomShadingConstants::POSITION_TEX_COORD_VERTEX_SHADER;
				mFragmentShaderKey = CustomShadingConstants::ALPHA_TEXTURE_SHADOWMAP;
			}

			/// Abstract method. Sends parameters to the shaders.
			/// \param projection Projection matrix
			/// \param view View matrix			 
			/// \param geometry Geometry item to be drawn
			virtual void setProgramParameters(const QMatrix4x4& modelViewProjectionMatrix) override
			{
				assert(mpProgram != nullptr && "Shadow map program not found");
				assert(mpTargetMaterial != nullptr && "Target material not found");
				{
					mpProgram->setUniformValue("mModelViewProjectionMatrix", modelViewProjectionMatrix);
					mpProgram->setUniformValue( "mThresholdAlphaValue", ((T*)mpTargetMaterial)->GetThresholdValue() );
					mpProgram->setUniformValue("mGlobalAlphaValue", ((T*)mpTargetMaterial)->GetGlobalAlpha() );
				}
			}

			/// Abstract method. Binds shaders and draws.
			/// \param vertexBuf Vertex buffer
			/// \param indexBuf IndexBuffer
			/// \param totalVertexNum Number of vetices
			/// \param titalIndexNum Number of indices
			virtual void drawShader(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) override
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

				mpProgram->disableAttributeArray(vertexLocation);

				vertexBuf->release();
				indexBuf->release();
			}
		};

	}
}
#endif // !GEOMETRYALPHACOLORSHADOWMAP_H

