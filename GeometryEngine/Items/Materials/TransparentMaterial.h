#pragma once

#ifndef GEOMETRYTRANSPARENTMATERIAL_H
#define GEOMETRYTRANSPARENTMATERIAL_H

#include "Material.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		/// Class that represents a material that may contain transparencies. Implements CustomShadingInterface so child classes can implement their own shadowMap shaders.
		class TransparentMaterial : public Material
		{
		public:
			/// Constructor 
			/// param thresholdValue Min alpha value below which the fragment is dropped completely. This value is clamped to the range [0, 1]
			/// param globalAlphaValue Global alpha value to be applied to the whole model. This value is multiplied by other posible alphas. This value is clamped to the range [0, 1]
			/// param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininessIf shininess is <= 0 it is set to 0.001 to avoid errors in the shaders.
			TransparentMaterial(float thresholdValue = 0.0f, float globalAlphaValue = 1.0f, float shininess = 10.0f, bool translucent = false);
			/// Copy constructor
			/// param ref Object to be copied.
			TransparentMaterial(const TransparentMaterial& ref);
			/// Destructor
			virtual ~TransparentMaterial();

			float GetThresholdValue() { return mThresholdValue; }
			float GetGlobalAlpha() { return mGlobalAlphaValue; }

			void SetThresholdValue(float thresholdValue) { mThresholdValue = checkAlphaValue(thresholdValue); }
			void SetGlobalAlpha(float globalAlpha) { mGlobalAlphaValue = checkAlphaValue(globalAlpha); }

			/// Returns whether the material allows transparencies. Overrides the parent method to return true.
			virtual bool IsTransparent() override { return true; }

			/// Applies the material shadow map shaders.
			/// param vertexBuf Pointer to the vertex buffer
			/// param indexBuffer Pointer to the index buffer
			/// param modelMatrix model matrix of the item to be added to the shadow map
			/// param totalVertexNum Total amount of vertices
			/// param totalIndexNum Total amount of indices
			virtual void CalculateCustomShadowMap(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& modelViewProjection, unsigned int totalVertexNum, unsigned int totalIndexNum);

			/// Returns whether custom shadow map shaders should be used.
			virtual bool GetApplyCustomShadowMap() override { return mApplyCustomShadowMap; }

			/// Sets whether custom shadow map shaders should be used
			/// param applyCustom true if custom shadow maps should be used false otherwise.
			virtual void SetApplyCustomShadowMap(bool applyCustom) override { mApplyCustomShadowMap = applyCustom; }

			/// Returns whether translucent shadowing should be applied to this material or not.
			virtual bool IsTranslucent() override { return mIsTranslucent; }
			/// Sets whether translucent shadowing should be applied to this material or not.
			virtual void SetTranslucent(bool isTranslucent) { mIsTranslucent = isTranslucent; }

		protected:
			float mThresholdValue;
			float mGlobalAlphaValue;
			bool mIsTranslucent;

			QOpenGLShaderProgram* mpShadowMapProgram; // Lighting shader
			std::string mShadowMapVertexShaderKey;
			std::string mShadowMapFragmentShaderKey;

			/// Initializes managers and shaders
			virtual void initMaterial() override;
			/// Abstract method. Sets the material shadow map shaders that should be loaded
			virtual void initShadowMapShaders() = 0;
			/// Loads and compiles material shadow map shader program
			virtual void initShadowMapProgram();
			/// Abstract method. Sends parameters to the shadow map shaders.
			/// param modelViewProjectionMatrix Model matrix modified by the light's viewport viewProjection matrix
			virtual void setShadowProgramParameters(const QMatrix4x4& modelViewProjectionMatrix) = 0;
			/// Abstract method. Binds shaders and calculates the shadow map.
			/// param vertexBuf Vertex buffer
			/// param indexBuf IndexBuffer
			/// param totalVertexNum Number of vetices
			/// param titalIndexNum Number of indices
			virtual void renderShadowMap(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum) = 0;
			/// Applies internal modifiers and calls renderShadowMap
			/// param vertexBuf Vertex buffer
			/// param indexBuf IndexBuffer
			/// param totalVertexNum Number of vetices
			/// param titalIndexNum Number of indices
			virtual void modifyRenderShadowMap(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum);
			/// Copies the data of a Material object to the current object
			/// param ref Material to be copied
			void copy(const TransparentMaterial & mat);
		private:
			/// Clamps a candidate alpha value to the range [0, 1]
			/// param alphaValue Candidate alpha value
			/// return value clamped to range [0, 1]
			float checkAlphaValue(float alphaValue);

		};
	}
}

#endif