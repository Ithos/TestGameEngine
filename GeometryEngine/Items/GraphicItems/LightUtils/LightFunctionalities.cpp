#include "../Light.h"
#include "LightFunction.h"
#include "LightRenderTechnique.h"
#include "LightFunctionalities.h"

const std::string GeometryEngine::LightUtils::LightFuntionalitiesConstants::POSITION_VERTEX_SHADER = "POSITION_VERTEX_SHADER";
const std::string GeometryEngine::LightUtils::LightFuntionalitiesConstants::NULL_FRAGMENT_SHADER = "NULL_FRAGMENT_SHADER";
const std::string GeometryEngine::LightUtils::LightFuntionalitiesConstants::SHADOW_MAP_FRAGMENT_SHADER = "SHADOW_MAP_FRAGMENT_SHADER";

GeometryEngine::LightUtils::LightFunctionalities::LightFunctionalities(const LightFunctionalities & ref)
{
	copy(ref);
	InitFunctions();
}

GeometryEngine::LightUtils::LightFunctionalities::~LightFunctionalities()
{
	for (auto it = mFunctionMap.begin(); it != mFunctionMap.end(); ++it)
	{
		delete (*it).second;
		(*it).second = nullptr;
	}

	for (auto it = mTechniqueMap.begin(); it != mTechniqueMap.end(); ++it)
	{
		delete (*it).second;
		(*it).second = nullptr;
	}
}

bool GeometryEngine::LightUtils::LightFunctionalities::AddNewLightFunction(LightFunction * value, LightFunctions key)
{
	if (ContainsFunction(key)) return false;
	mFunctionMap[key] = value;
	return true;
}

bool GeometryEngine::LightUtils::LightFunctionalities::RemoveLightFunction(LightFunctions key)
{
	if(!ContainsFunction(key)) return false;
	delete mFunctionMap[key];
	mFunctionMap.erase(key);
	return true;
}

bool GeometryEngine::LightUtils::LightFunctionalities::InitLightFunction(LightFunctions key)
{
	if (!ContainsFunction(key)) return false;
	mFunctionMap[key]->InitCustomShading(mpShaderManager);
	return true;
}

bool GeometryEngine::LightUtils::LightFunctionalities::AddNewLightTechnique(LightRenderTechnique * value, LightTechniques key)
{
	if (ContainsTechnique(key)) return false;
	mTechniqueMap[key] = value;
	return true;
}

bool GeometryEngine::LightUtils::LightFunctionalities::RemoveLightTechnique(LightTechniques key)
{
	if (!ContainsTechnique(key)) return false;
	delete mTechniqueMap[key];
	mTechniqueMap.erase(key);
	return true;
}

void GeometryEngine::LightUtils::LightFunctionalities::SetTargetLight(GeometryWorldItem::GeometryLight::Light * target)
{
	for (auto it = mFunctionMap.begin(); it != mFunctionMap.end(); ++it)
	{
		(*it).second->SetTargetLight(target);
	}

	for (auto it = mTechniqueMap.begin(); it != mTechniqueMap.end(); ++it)
	{
		(*it).second->SetTargetLight(target);
	}
}

bool GeometryEngine::LightUtils::LightFunctionalities::SetTargetLight(GeometryWorldItem::GeometryLight::Light * target, LightFunctions function)
{
	if (!ContainsFunction(function)) return false;
	mFunctionMap[function]->SetTargetLight(target);
	return true;
}

bool GeometryEngine::LightUtils::LightFunctionalities::SetTargetLight(GeometryWorldItem::GeometryLight::Light * target, LightTechniques technique)
{
	if (!ContainsTechnique(technique)) return false;
	mTechniqueMap[technique]->SetTargetLight(target);
	return true;
}

bool GeometryEngine::LightUtils::LightFunctionalities::ApplyFunction(LightFunctions function, QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, const QMatrix4x4& projectionMatrix, 
	const QMatrix4x4& viewMatrix, const QMatrix4x4& modelMatrix, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	if( !ContainsFunction(function) ) return false;
	mFunctionMap[function]->ApplyFunction(vertexBuf, indexBuf, projectionMatrix, viewMatrix, modelMatrix, totalVertexNum, totalIndexNum);
	return true;
}

bool GeometryEngine::LightUtils::LightFunctionalities::ApplyTechnique(LightTechniques technique, const QMatrix4x4 & projectionMatrix, const QMatrix4x4 & viewMatrix, const BuffersInfo & buffersInfo, const QVector3D & viewPos)
{
	if( !ContainsTechnique(technique) ) return false;
	mTechniqueMap[technique]->ApplyTechnique(projectionMatrix, viewMatrix, buffersInfo, viewPos);
	return true;
}

void GeometryEngine::LightUtils::LightFunctionalities::InitFunctions()
{
	getManagers();

	for (auto it = mFunctionMap.begin(); it != mFunctionMap.end(); ++it)
	{
		(*it).second->InitCustomShading(mpShaderManager);
	}
}

void GeometryEngine::LightUtils::LightFunctionalities::getManagers()
{
	mpConfInstance = Configuration::ConfigurationManager::GetInstance();
	mpShaderManager = ShaderFiles::ShaderManager::GetInstance(mpConfInstance->getVertexShaderFolder(), mpConfInstance->getFragmentShaderFolder(),
		mpConfInstance->getVertexShaderConfig(), mpConfInstance->getFragmentShaderConfig());
}

void GeometryEngine::LightUtils::LightFunctionalities::copy(const LightFunctionalities & ref)
{
	for (auto it = ref.mFunctionMap.begin(); it != ref.mFunctionMap.end(); ++it)
	{
		this->mFunctionMap[(*it).first] = (*it).second->Clone(this, (*it).first);
	}

	for (auto it = ref.mTechniqueMap.begin(); it != ref.mTechniqueMap.end(); ++it)
	{
		this->mTechniqueMap[(*it).first] = (*it).second->Clone(this, (*it).first);
	}
}



