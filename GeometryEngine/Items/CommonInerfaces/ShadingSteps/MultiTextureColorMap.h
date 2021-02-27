#pragma once

#ifndef GEOMETRYMULTITEXTURECOLORMAP_H
#define GEOMETRYMULTITEXTURECOLORMAP_H

#include "../CustomShadingStep.h"

namespace GeometryEngine
{
	namespace CustomShading
	{
		/// Custom color map calculation for alpha multi texture materials
		template<class T>
		class MultiTextureColorMap : public CustomShadingStep
		{
		public:
			/// Constructor
			/// \param parent Pointer to the CustomShadingInterface that contains it 
			/// \param step Value that indicates at which render stepthis shading technique will be used
			MultiTextureColorMap(CustomShadingInterface* parent, CustomShadingSteps step) : CustomShadingStep(parent, step) {};

			/// Copy constructor
			/// \param ref Object to be copied.
			MultiTextureColorMap(const CustomShadingStep& ref) { copy(ref); }

			/// Destructor
			virtual ~MultiTextureColorMap() {};

			/// Abstract method. Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual MultiTextureColorMap* Clone(CustomShadingInterface* parent, CustomShadingSteps step) const {
				MultiTextureColorMap* cloned = new MultiTextureColorMap((*this));
				cloned->AddToInterface(parent, step);
				return cloned;
			}
		protected:
			/// Sets the shaders that should be loaded
			virtual void initShaders() override
			{
				mVertexShaderKey = CustomShadingConstants::POSITION_TEX_COORD_VERTEX_SHADER;
				mFragmentShaderKey = CustomShadingConstants::MULTI_TEXTURE_COLOR_MAP_FRAGMENT_SHADER;
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
					mpProgram->setUniformValue("textureDiffuse", ((T*)mpTargetMaterial)->GetInitialTextureUnit() + 1);
					mpProgram->setUniformValue("textureReflective", ((T*)mpTargetMaterial)->GetInitialTextureUnit() + 2);
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

				// Tell OpenGL programmable pipeline how to locate texture coordinates
				int textureCoordinate = mpProgram->attributeLocation("TexCoord");
				mpProgram->enableAttributeArray(textureCoordinate);
				mpProgram->setAttributeBuffer(textureCoordinate, GL_FLOAT, VertexData::TEXTURE_COORDINATES_OFFSET, 2, sizeof(VertexData));

				((T*)mpTargetMaterial)->BindTextures();

				// Draw light
				glDrawElements(GL_TRIANGLE_STRIP, totalIndexNumber, GL_UNSIGNED_SHORT, 0);

				((T*)mpTargetMaterial)->UnbindTextures();

				vertexBuf->release();
				indexBuf->release();
			}
		};
	}
}

#endif