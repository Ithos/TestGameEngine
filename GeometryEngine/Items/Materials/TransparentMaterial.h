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
			/// \param customShading Interface to an object that implements custom shadow shading for this material
			/// \param thresholdValue Min alpha value below which the fragment is dropped completely. This value is clamped to the range [0, 1]
			/// \param globalAlphaValue Global alpha value to be applied to the whole model. This value is multiplied by other posible alphas. This value is clamped to the range [0, 1]
			/// \param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininessIf shininess is <= 0 it is set to 0.001 to avoid errors in the shaders.
			TransparentMaterial(const CustomShading::CustomShadingInterface* const customShading, float thresholdValue = 0.0f, float globalAlphaValue = 1.0f, 
				float shininess = 10.0f, bool translucent = false);
			/// Copy constructor
			/// \param ref Object to be copied.
			TransparentMaterial(const TransparentMaterial& ref);
			/// Destructor
			virtual ~TransparentMaterial();

			float GetThresholdValue() { return mThresholdValue; }
			float GetGlobalAlpha() { return mGlobalAlphaValue; }

			void SetThresholdValue(float thresholdValue) { mThresholdValue = checkAlphaValue(thresholdValue); }
			void SetGlobalAlpha(float globalAlpha) { mGlobalAlphaValue = checkAlphaValue(globalAlpha); }

			/// Returns whether the material allows transparencies. Overrides the parent method to return true.
			virtual bool IsTransparent() override { return true; }

			/// Returns whether translucent shadowing should be applied to this material or not.
			virtual bool IsTranslucent() override { return mIsTranslucent; }
			/// Sets whether translucent shadowing should be applied to this material or not.
			virtual void SetTranslucent(bool isTranslucent) { mIsTranslucent = isTranslucent; }

		protected:
			float mThresholdValue;
			float mGlobalAlphaValue;
			bool mIsTranslucent;

			///Empty constructor
			///Called from child objects copy constructor to avoid double initialization 
			TransparentMaterial() {}
			/// Copies the data of a Material object to the current object
			/// \param ref Material to be copied
			void copy(const TransparentMaterial & mat);
		private:
			/// Clamps a candidate alpha value to the range [0, 1]
			/// \param alphaValue Candidate alpha value
			/// \return value clamped to range [0, 1]
			float checkAlphaValue(float alphaValue);

		};
	}
}

#endif