#pragma once

#ifndef GEOMETRYDEFAULTSHADOWMAP_H
#define GEOMETRYDEFAULTSHADOWMAP_H

#include "../LightShadingComponent.h"
#include "../../../Item Utils/Viewport.h"

namespace GeometryEngine
{
	namespace LightUtils
	{

		/// Class that contains the default shadowmap calculation for the light
		template<class T>
		class DefaultShadowMap : public LightShadingComponent
		{
		public:
			/// Constructor
			/// \param parent Pointer to the LightFunctionalities that contains it 
			/// \param Value that indicates at which light function this fclass corresponds
			DefaultShadowMap(LightComponentManager* parent, LightShading step) : LightShadingComponent(parent, step) {};

			/// Copy constructor
			/// \param ref Object to be copied.
			DefaultShadowMap(const LightShadingComponent& ref) { copy(ref); }

			/// Destructor
			virtual ~DefaultShadowMap() {};

			/// Abstract method. Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual DefaultShadowMap* Clone(LightComponentManager* parent, LightShading step) const {
				DefaultShadowMap* cloned = new DefaultShadowMap((*this));
				cloned->AddToInterface(parent, step);
				return cloned;
			}

		protected:
			/// Sets the shaders that should be loaded
			virtual void initShaders() override
			{
				mVertexShaderKey = LightFuntionalitiesConstants::POSITION_VERTEX_SHADER;
				mFragmentShaderKey = LightFuntionalitiesConstants::SHADOW_MAP_FRAGMENT_SHADER;
			}

			/// Sends parameters to the shader.
			/// \param projectionMatrix Projection matrix to be used 
			/// \param viewMatrix ViewMatrix to be used
			/// \param modelMatrix ModelMatrix of the object to be drawn or identity if it's not necessary
			virtual void setProgramParameters(const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const QMatrix4x4& modelMatrix) override
			{
				assert(mpProgram != nullptr && "Stencil Test shadow map program not found");
				assert(mpTargetLight != nullptr && "Stencil Test target light not found");
				{
					T* light = ((T*)mpTargetLight);
					light->UpdateModelMatrix(true);
					GeometryItemUtils::Viewport& viewport = light->GetLightViewport();
					viewport.CalculateProjectionMatrix();
					QMatrix4x4 modelViewProjection = viewport.GetViewProjectionMatrix() * modelMatrix;

					mpProgram->setUniformValue("mModelViewProjectionMatrix", modelViewProjection);
				}
			}

			/// Binds shaders and draws.
			/// \param vertexBuf Vertex buffer
			/// \param indexBuf IndexBuffer
			/// \param totalVertexNum Number of vetices
			/// \param titalIndexNum Number of indices
			virtual void applyShader(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) override
			{
				assert(mpProgram != nullptr && "Stencil Test shadow map program not found");
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

					mpProgram->disableAttributeArray(vertexLocation);

					vertexBuf->release();
					indexBuf->release();
				}
			}

		};
	}
}
#endif