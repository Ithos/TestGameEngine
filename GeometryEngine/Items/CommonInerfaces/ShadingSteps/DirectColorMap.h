#pragma once

#ifndef GEOMETRYDIRECTCOLORMAP_H
#define GEOMETRYDIRECTCOLORMAP_H

#include "../CustomShadingStep.h"

namespace GeometryEngine
{
	namespace CustomShading
	{
		/// Custom color map calculation for alpha color materials
		template<class T>
		class DirectColorMap : public CustomShadingStep
		{
		public:
			/// Constructor
			/// \param parent Pointer to the CustomShadingInterface that contains it 
			/// \param Value that indicates at which render stepthis shading technique will be used
			DirectColorMap(CustomShadingInterface* parent, CustomShadingSteps step) : CustomShadingStep(parent, step) {};

			/// Copy constructor
			/// \param ref Object to be copied.
			DirectColorMap(const CustomShadingStep& ref) { copy(ref); }

			/// Destructor
			virtual ~DirectColorMap() {};

			/// Abstract method. Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual DirectColorMap* Clone(CustomShadingInterface* parent, CustomShadingSteps step) const {
				DirectColorMap* cloned = new DirectColorMap((*this));
				cloned->AddToInterface(parent, step);
				return cloned;
			}
		protected:
			/// Sets the shaders that should be loaded
			virtual void initShaders() override
			{
				mVertexShaderKey = CustomShadingConstants::DIRECT_COLOR_MAP_VERTEX_SHADER;
				mFragmentShaderKey = CustomShadingConstants::DIRECT_COLOR_MAP_FRAGMENT_SHADER;
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
					mpProgram->setUniformValue("diffuseColor", ((T*)mpTargetMaterial)->GetDiffuse());
					mpProgram->setUniformValue("reflectiveColor", ((T*)mpTargetMaterial)->GetSpecular());
					mpProgram->setUniformValue("mThresholdAlphaValue", ((T*)mpTargetMaterial)->GetThresholdValue());
					mpProgram->setUniformValue("mGlobalAlphaValue", ((T*)mpTargetMaterial)->GetGlobalAlpha());
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

				vertexBuf->release();
				indexBuf->release();
			}

		};
	}
}
#endif
