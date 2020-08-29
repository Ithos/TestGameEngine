#include "LightRenderComponent.h"

GeometryEngine::LightUtils::LightRenderComponent::LightRenderComponent(LightComponentManager * parent, LightRender step) : mpTargetLight(nullptr), mpParentInterface(parent)
{
	mpParentInterface->AddNewLightRenderComponent(this, step);
}

void GeometryEngine::LightUtils::LightRenderComponent::AddToInterface(LightComponentManager * parent, LightRender step)
{
	mpParentInterface = parent;
	mpParentInterface->AddNewLightRenderComponent(this, step);
}

void GeometryEngine::LightUtils::LightRenderComponent::copy(const LightRenderComponent & ref)
{
	this->mpParentInterface = nullptr;
	this->mpTargetLight = nullptr;
}
