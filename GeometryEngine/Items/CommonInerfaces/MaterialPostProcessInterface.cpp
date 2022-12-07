#include "../Materials/Material.h"
#include "MaterialPostProcessStep.h"
#include "MaterialPostProcessInterface.h"

const std::string GeometryEngine::CustomShading::MaterialPostProcessConstants::POST_PROCESS_VERTEX_SHADER = "POST_PROCESS_VERTEX_SHADER";
const std::string GeometryEngine::CustomShading::MaterialPostProcessConstants::BLUR_POST_PROCESS_FRAGMENT_SHADER = "BLUR_POST_PROCESS_FRAGMENT_SHADER";
const std::string GeometryEngine::CustomShading::MaterialPostProcessConstants::GREY_SCALE_PROCESS_FRAGMENT_SHADER = "GREY_SCALE_PROCESS_FRAGMENT_SHADER";

GeometryEngine::CustomShading::MaterialPostProcessInterface::MaterialPostProcessInterface(const MaterialPostProcessInterface & ref)
{
	copy(ref);
	InitPostProcessSteps();
}

GeometryEngine::CustomShading::MaterialPostProcessInterface::~MaterialPostProcessInterface()
{
	for (auto it = mStepMap.begin(); it != mStepMap.end(); ++it)
	{
		delete (*it).second;
		(*it).second = nullptr;
	}
}

bool GeometryEngine::CustomShading::MaterialPostProcessInterface::AddPostProcessStep(MaterialPostProcessStep * value, MaterialPostProcessSteps key)
{
	if (ContainsStep(key)) return false;
	mStepMap[key] = value;
	return true;
}

bool GeometryEngine::CustomShading::MaterialPostProcessInterface::RemovePostProcessStep(MaterialPostProcessSteps key)
{
	if (!ContainsStep(key)) return false;
	delete mStepMap[key];
	mStepMap.erase(key);
	return true;
}

int GeometryEngine::CustomShading::MaterialPostProcessInterface::GetStepIterations(MaterialPostProcessSteps step)
{
	if (!ContainsStep(step)) return 0;
	return mStepMap[step]->GetIterations();
}

void GeometryEngine::CustomShading::MaterialPostProcessInterface::SetTargetMaterial(GeometryMaterial::Material * target)
{
	for (auto it = mStepMap.begin(); it != mStepMap.end(); ++it)
	{
		(*it).second->SetTargetMaterial(target);
	}
}

bool GeometryEngine::CustomShading::MaterialPostProcessInterface::RenderStep(MaterialPostProcessSteps step, QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, const QMatrix4x4 & modelViewProjection, unsigned int totalVertexNum, unsigned int totalIndexNum, const GBufferTextureInfo & gBuffTexInfo)
{
	if (!ContainsStep(step)) return false;

	mStepMap[step]->ApplyPostProcess(vertexBuf, indexBuf, modelViewProjection, totalVertexNum, totalIndexNum, gBuffTexInfo);

	return true;
}

void GeometryEngine::CustomShading::MaterialPostProcessInterface::InitPostProcessSteps()
{
	getManagers();

	for (auto it = mStepMap.begin(); it != mStepMap.end(); ++it)
	{
		(*it).second->initCustomShading(mpShaderManager);
	}
}

bool GeometryEngine::CustomShading::MaterialPostProcessInterface::InitPostProcessStep(MaterialPostProcessSteps key)
{
	if (!ContainsStep(key)) return false;
	mStepMap[key]->initCustomShading(mpShaderManager);
	return true;
}

void GeometryEngine::CustomShading::MaterialPostProcessInterface::getManagers()
{
	mpConfInstance = Configuration::ConfigurationManager::GetInstance();
	mpShaderManager = ShaderFiles::ShaderManager::GetInstance(mpConfInstance->getVertexShaderFolder(), mpConfInstance->getFragmentShaderFolder(),
		mpConfInstance->getVertexShaderConfig(), mpConfInstance->getFragmentShaderConfig());
}

void GeometryEngine::CustomShading::MaterialPostProcessInterface::copy(const MaterialPostProcessInterface & ref)
{
	for (auto it = ref.mStepMap.begin(); it != ref.mStepMap.end(); ++it)
	{
		this->mStepMap[(*it).first] = (*it).second->Clone(this, (*it).first);
	}
}


