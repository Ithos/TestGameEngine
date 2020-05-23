#include "TransparentMaterial.h"

GeometryEngine::GeometryMaterial::TransparentMaterial::TransparentMaterial(float thresholdValue, float globalAlphaValue, float shininess): Material(shininess), 
mGlobalAlphaValue(globalAlphaValue), mThresholdValue(thresholdValue)
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
}

float GeometryEngine::GeometryMaterial::TransparentMaterial::checkAlphaValue(float alphaValue)
{
	if (alphaValue > 1.0f) return 1.0f; 
	if (alphaValue < 0.0f) return 0.0f; 
	return alphaValue; 
}
