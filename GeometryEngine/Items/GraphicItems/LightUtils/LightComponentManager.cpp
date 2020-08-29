#include "../Light.h"
#include "LightShadingComponent.h"
#include "LightRenderComponent.h"
#include "LightComponentManager.h"

const std::string GeometryEngine::LightUtils::LightFuntionalitiesConstants::POSITION_VERTEX_SHADER = "POSITION_VERTEX_SHADER";
const std::string GeometryEngine::LightUtils::LightFuntionalitiesConstants::NULL_FRAGMENT_SHADER = "NULL_FRAGMENT_SHADER";
const std::string GeometryEngine::LightUtils::LightFuntionalitiesConstants::SHADOW_MAP_FRAGMENT_SHADER = "SHADOW_MAP_FRAGMENT_SHADER";

GeometryEngine::LightUtils::LightComponentManager::LightComponentManager(const LightComponentManager & ref)
{
	copy(ref);
	InitFunctions();
}

GeometryEngine::LightUtils::LightComponentManager::~LightComponentManager()
{
	for (auto it = mShadingMap.begin(); it != mShadingMap.end(); ++it)
	{
		delete (*it).second;
		(*it).second = nullptr;
	}

	for (auto it = mRenderMap.begin(); it != mRenderMap.end(); ++it)
	{
		delete (*it).second;
		(*it).second = nullptr;
	}
}

bool GeometryEngine::LightUtils::LightComponentManager::AddNewLightShadingComponent(LightShadingComponent * value, LightShading key)
{
	if (ContainsLightShadingComponent(key)) return false;
	mShadingMap[key] = value;
	return true;
}

bool GeometryEngine::LightUtils::LightComponentManager::RemoveLightShadingComponent(LightShading key)
{
	if(!ContainsLightShadingComponent(key)) return false;
	delete mShadingMap[key];
	mShadingMap.erase(key);
	return true;
}

bool GeometryEngine::LightUtils::LightComponentManager::InitLightShadingComponent(LightShading key)
{
	if (!ContainsLightShadingComponent(key)) return false;
	mShadingMap[key]->InitCustomShading(mpShaderManager);
	return true;
}

bool GeometryEngine::LightUtils::LightComponentManager::AddNewLightRenderComponent(LightRenderComponent * value, LightRender key)
{
	if (ContainsLightRenderComponent(key)) return false;
	mRenderMap[key] = value;
	return true;
}

bool GeometryEngine::LightUtils::LightComponentManager::RemoveLightRenderComponent(LightRender key)
{
	if (!ContainsLightRenderComponent(key)) return false;
	delete mRenderMap[key];
	mRenderMap.erase(key);
	return true;
}

void GeometryEngine::LightUtils::LightComponentManager::SetTargetLight(GeometryWorldItem::GeometryLight::Light * target)
{
	for (auto it = mShadingMap.begin(); it != mShadingMap.end(); ++it)
	{
		(*it).second->SetTargetLight(target);
	}

	for (auto it = mRenderMap.begin(); it != mRenderMap.end(); ++it)
	{
		(*it).second->SetTargetLight(target);
	}
}

bool GeometryEngine::LightUtils::LightComponentManager::SetTargetLight(GeometryWorldItem::GeometryLight::Light * target, LightShading key)
{
	if (!ContainsLightShadingComponent(key)) return false;
	mShadingMap[key]->SetTargetLight(target);
	return true;
}

bool GeometryEngine::LightUtils::LightComponentManager::SetTargetLight(GeometryWorldItem::GeometryLight::Light * target, LightRender key)
{
	if (!ContainsLightRenderComponent(key)) return false;
	mRenderMap[key]->SetTargetLight(target);
	return true;
}

bool GeometryEngine::LightUtils::LightComponentManager::ApplyLightShading(LightShading component, QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, const QMatrix4x4& projectionMatrix, 
	const QMatrix4x4& viewMatrix, const QMatrix4x4& modelMatrix, unsigned int totalVertexNum, unsigned int totalIndexNum)
{
	if( !ContainsLightShadingComponent(component) ) return false;
	mShadingMap[component]->Render(vertexBuf, indexBuf, projectionMatrix, viewMatrix, modelMatrix, totalVertexNum, totalIndexNum);
	return true;
}

bool GeometryEngine::LightUtils::LightComponentManager::ApplyLightRender(LightRender component, const QMatrix4x4 & projectionMatrix, const QMatrix4x4 & viewMatrix, const BuffersInfo & buffersInfo, const QVector3D & viewPos)
{
	if( !ContainsLightRenderComponent(component) ) return false;
	mRenderMap[component]->Render(projectionMatrix, viewMatrix, buffersInfo, viewPos);
	return true;
}

void GeometryEngine::LightUtils::LightComponentManager::InitFunctions()
{
	getManagers();

	for (auto it = mShadingMap.begin(); it != mShadingMap.end(); ++it)
	{
		(*it).second->InitCustomShading(mpShaderManager);
	}
}

void GeometryEngine::LightUtils::LightComponentManager::getManagers()
{
	mpConfInstance = Configuration::ConfigurationManager::GetInstance();
	mpShaderManager = ShaderFiles::ShaderManager::GetInstance(mpConfInstance->getVertexShaderFolder(), mpConfInstance->getFragmentShaderFolder(),
		mpConfInstance->getVertexShaderConfig(), mpConfInstance->getFragmentShaderConfig());
}

void GeometryEngine::LightUtils::LightComponentManager::copy(const LightComponentManager & ref)
{
	for (auto it = ref.mShadingMap.begin(); it != ref.mShadingMap.end(); ++it)
	{
		this->mShadingMap[(*it).first] = (*it).second->Clone(this, (*it).first);
	}

	for (auto it = ref.mRenderMap.begin(); it != ref.mRenderMap.end(); ++it)
	{
		this->mRenderMap[(*it).first] = (*it).second->Clone(this, (*it).first);
	}
}



