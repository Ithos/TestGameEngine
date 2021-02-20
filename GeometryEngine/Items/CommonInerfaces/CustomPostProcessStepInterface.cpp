#include "..\Items\PostProcess\PostProcess.h"
#include "CustomPostProcessStep.h"
#include "CustomPostProcessStepInterface.h"

GeometryEngine::CustomShading::CustomPostProcessStepInterface::CustomPostProcessStepInterface(const CustomPostProcessStepInterface & ref)
{
	copy(ref);
	InitCustomSteps();
}

GeometryEngine::CustomShading::CustomPostProcessStepInterface::~CustomPostProcessStepInterface()
{
	for (auto it = mStepMap.begin(); it != mStepMap.end(); ++it)
	{
		delete (*it).second;
		(*it).second = nullptr;
	}
}

bool GeometryEngine::CustomShading::CustomPostProcessStepInterface::AddPostProcessStep(CustomPostProcessStep * value, CustomPostProcessSteps key)
{
	if (ContainsStep(key)) return false;
	mStepMap[key] = value;
	return true;
}

bool GeometryEngine::CustomShading::CustomPostProcessStepInterface::RemovePostProcessStep(CustomPostProcessSteps key)
{
	if (!ContainsStep(key)) return false;
	delete mStepMap[key];
	mStepMap.erase(key);
	return true;
}

void GeometryEngine::CustomShading::CustomPostProcessStepInterface::SetTargetPostProcess(GeometryEngine::GeometryPostProcess::PostProcess * target)
{
	for (auto it = mStepMap.begin(); it != mStepMap.end(); ++it)
	{
		(*it).second->SetTargetPostProcess(target);
	}
}

bool GeometryEngine::CustomShading::CustomPostProcessStepInterface::RenderStep(CustomPostProcessSteps step, const GBufferTextureInfo & gBuffTexInfo)
{
	if (!ContainsStep(step)) return false;

	mStepMap[step]->ApplyPostProcessStep(gBuffTexInfo);

	return true;
}

void GeometryEngine::CustomShading::CustomPostProcessStepInterface::InitCustomSteps()
{
	getManagers();

	for (auto it = mStepMap.begin(); it != mStepMap.end(); ++it)
	{
		(*it).second->initCustomShading(mpShaderManager);
	}
}

bool GeometryEngine::CustomShading::CustomPostProcessStepInterface::InitCustomStep(CustomPostProcessSteps key)
{
	if (!ContainsStep(key)) return false;
	mStepMap[key]->initCustomShading(mpShaderManager);
	return true;
}

void GeometryEngine::CustomShading::CustomPostProcessStepInterface::getManagers()
{
	mpConfInstance = Configuration::ConfigurationManager::GetInstance();
	mpShaderManager = ShaderFiles::ShaderManager::GetInstance(mpConfInstance->getVertexShaderFolder(), mpConfInstance->getFragmentShaderFolder(),
		mpConfInstance->getVertexShaderConfig(), mpConfInstance->getFragmentShaderConfig());
}

void GeometryEngine::CustomShading::CustomPostProcessStepInterface::copy(const CustomPostProcessStepInterface & ref)
{
	for (auto it = ref.mStepMap.begin(); it != ref.mStepMap.end(); ++it)
	{
		this->mStepMap[(*it).first] = (*it).second->Clone(this, (*it).first);
	}
}
