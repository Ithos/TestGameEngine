#include "TransparentMaterial.h"

GeometryEngine::GeometryMaterial::TransparentMaterial::TransparentMaterial(const CustomShading::CustomShadingInterface* const customShading, float thresholdValue, float globalAlphaValue, 
	float shininess, bool translucent): Material(shininess, customShading), mGlobalAlphaValue(globalAlphaValue), mThresholdValue(thresholdValue), mIsTranslucent(translucent)
{
	mGlobalAlphaValue = checkAlphaValue(mGlobalAlphaValue);
	mThresholdValue = checkAlphaValue(mThresholdValue);
}

GeometryEngine::GeometryMaterial::TransparentMaterial::TransparentMaterial(const TransparentMaterial & ref)
{
	copy(ref);
}

GeometryEngine::GeometryMaterial::TransparentMaterial::~TransparentMaterial()
{
}

void GeometryEngine::GeometryMaterial::TransparentMaterial::copy(const TransparentMaterial & mat)
{
	Material::copy(mat);
	mGlobalAlphaValue = mat.mGlobalAlphaValue;
	mThresholdValue = mat.mThresholdValue;
	mIsTranslucent = mat.mIsTranslucent;
}

float GeometryEngine::GeometryMaterial::TransparentMaterial::checkAlphaValue(float alphaValue)
{
	if (alphaValue > 1.0f) return 1.0f; 
	if (alphaValue < 0.0f) return 0.0f; 
	return alphaValue; 
}
