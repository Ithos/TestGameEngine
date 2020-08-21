#include "LightRenderTechnique.h"

GeometryEngine::LightUtils::LightRenderTechnique::LightRenderTechnique(LightFunctionalities * parent, LightTechniques step) : mpTargetLight(nullptr), mpParentInterface(parent)
{
	mpParentInterface->AddNewLightTechnique(this, step);
}

void GeometryEngine::LightUtils::LightRenderTechnique::AddToInterface(LightFunctionalities * parent, LightTechniques step)
{
	mpParentInterface = parent;
	mpParentInterface->AddNewLightTechnique(this, step);
}

void GeometryEngine::LightUtils::LightRenderTechnique::copy(const LightRenderTechnique & ref)
{
	this->mpParentInterface = nullptr;
	this->mpTargetLight = nullptr;
}
