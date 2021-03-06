#include "../Materials/Material.h"
#include "CustomShadingStep.h"
#include "CustomShadingInterface.h"

const std::string GeometryEngine::CustomShading::CustomShadingConstants::POSITION_TEX_COORD_VERTEX_SHADER = "POSITION_TEX_COORD_VERTEX_SHADER";
const std::string GeometryEngine::CustomShading::CustomShadingConstants::ALPHA_COLOR_SHADOWMAP = "ALPHA_COLOR_SHADOWMAP";
const std::string GeometryEngine::CustomShading::CustomShadingConstants::ALPHA_TEXTURE_SHADOWMAP = "ALPHA_TEXTURE_SHADOWMAP";
const std::string GeometryEngine::CustomShading::CustomShadingConstants::ALPHA_MULTI_TEXTURE_SHADOWMAP = "ALPHA_MULTI_TEXTURE_SHADOWMAP";
const std::string GeometryEngine::CustomShading::CustomShadingConstants::DIRECT_COLOR_MAP_VERTEX_SHADER = "DIRECT_COLOR_MAP_VERTEX_SHADER";
const std::string GeometryEngine::CustomShading::CustomShadingConstants::DIRECT_COLOR_MAP_FRAGMENT_SHADER = "DIRECT_COLOR_MAP_FRAGMENT_SHADER";
const std::string GeometryEngine::CustomShading::CustomShadingConstants::TEXTURE_COLOR_MAP_FRAGMENT_SHADER = "TEXTURE_COLOR_MAP_FRAGMENT_SHADER";
const std::string GeometryEngine::CustomShading::CustomShadingConstants::MULTI_TEXTURE_COLOR_MAP_FRAGMENT_SHADER = "MULTI_TEXTURE_COLOR_MAP_FRAGMENT_SHADER";

GeometryEngine::CustomShading::CustomShadingInterface::CustomShadingInterface(const CustomShadingInterface & ref)
{
	copy(ref); 
	InitCustomSteps();
}

GeometryEngine::CustomShading::CustomShadingInterface::~CustomShadingInterface()
{
	for (auto it = mStepMap.begin(); it != mStepMap.end(); ++it)
	{
		delete (*it).second;
		(*it).second = nullptr;
	}
}

bool GeometryEngine::CustomShading::CustomShadingInterface::AddShadingStep(CustomShadingStep * value, CustomShadingSteps key)
{
	if (ContainsStep(key)) return false;
	mStepMap[key] = value;
	return true;
}

bool GeometryEngine::CustomShading::CustomShadingInterface::RemoveShadingStep(CustomShadingSteps key)
{
	if (!ContainsStep(key)) return false;
	delete mStepMap[key];
	mStepMap.erase(key);
	return true;
}

void GeometryEngine::CustomShading::CustomShadingInterface::SetTargetMaterial(GeometryEngine::GeometryMaterial::Material * target)
{
	for (auto it = mStepMap.begin(); it != mStepMap.end(); ++it)
	{
		(*it).second->SetTargetMaterial(target);
	}
}

bool GeometryEngine::CustomShading::CustomShadingInterface::RenderStep(CustomShadingSteps step, QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& modelViewProjection,
	unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	if (!ContainsStep(step)) return false; 

	mStepMap[step]->RenderShaders(vertexBuf, indexBuf, modelViewProjection, totalVertexNum, totalIndexNum);

	return true;
}

void GeometryEngine::CustomShading::CustomShadingInterface::InitCustomSteps()
{
	getManagers();

	for (auto it = mStepMap.begin(); it != mStepMap.end(); ++it)
	{
		(*it).second->initCustomShading(mpShaderManager);
	}
}

bool GeometryEngine::CustomShading::CustomShadingInterface::InitCustomStep(CustomShadingSteps key)
{
	if (!ContainsStep(key)) return false;
	mStepMap[key]->initCustomShading(mpShaderManager);
	return true;
}

void GeometryEngine::CustomShading::CustomShadingInterface::getManagers()
{
	mpConfInstance = Configuration::ConfigurationManager::GetInstance();
	mpShaderManager = ShaderFiles::ShaderManager::GetInstance(mpConfInstance->getVertexShaderFolder(), mpConfInstance->getFragmentShaderFolder(),
		mpConfInstance->getVertexShaderConfig(), mpConfInstance->getFragmentShaderConfig());
}

void GeometryEngine::CustomShading::CustomShadingInterface::copy(const CustomShadingInterface & ref)
{
	for (auto it = ref.mStepMap.begin(); it != ref.mStepMap.end(); ++it)
	{
		this->mStepMap[(*it).first] = (*it).second->Clone(this, (*it).first);
	}
}
