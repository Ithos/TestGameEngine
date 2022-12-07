#include "MaterialPostProcessInterface.h"
#include "CustomShadingInterface.h"
#include "MultiShadingInterface.h"

GeometryEngine::CustomShading::MultiShadingInterface::MultiShadingInterface(const MultiShadingInterface & ref)
{
	copy(ref);
}

GeometryEngine::CustomShading::MultiShadingInterface::~MultiShadingInterface()
{
	for (auto it = mListsMap.begin(); it != mListsMap.end(); ++it)
	{
		delete (*it).second;
		(*it).second = nullptr;
	}

	delete mShadingInterface;
	mShadingInterface = nullptr;
}

bool GeometryEngine::CustomShading::MultiShadingInterface::AddNewList(ShadingLists key)
{
	if (!ContainsList(key)) mListsMap[key] = new MaterialPostProcessInterface();
	else return false;
	return true;
}

bool GeometryEngine::CustomShading::MultiShadingInterface::RemoveList(ShadingLists key)
{
	if (ContainsList(key)) {
		delete mListsMap[key];
		mListsMap.erase(key);
		return true;
	}
	return false;
}

GeometryEngine::CustomShading::MaterialPostProcessInterface * GeometryEngine::CustomShading::MultiShadingInterface::GetList(ShadingLists key)
{
	if (ContainsList(key)) return mListsMap[key];
	return nullptr;
}

void GeometryEngine::CustomShading::MultiShadingInterface::SetTargetMaterial(GeometryMaterial::Material * target)
{
	for (auto it = mListsMap.begin(); it != mListsMap.end(); ++it) (*it).second->SetTargetMaterial(target);
	if (mShadingInterface != nullptr) mShadingInterface->SetTargetMaterial(target);
}

void GeometryEngine::CustomShading::MultiShadingInterface::InitLists()
{
	for (auto it = mListsMap.begin(); it != mListsMap.end(); ++it) (*it).second->InitPostProcessSteps();
}

void GeometryEngine::CustomShading::MultiShadingInterface::SetShadingInterface(const CustomShadingInterface * const shadingInterface)
{
	mShadingInterface = shadingInterface->Clone();
}

void GeometryEngine::CustomShading::MultiShadingInterface::InitShadingInterface()
{
	if (mShadingInterface != nullptr) mShadingInterface->InitCustomSteps();
}

void GeometryEngine::CustomShading::MultiShadingInterface::copy(const MultiShadingInterface & ref)
{
	for (auto it = ref.mListsMap.begin(); it != ref.mListsMap.end(); ++it)
	{
		this->mListsMap[(*it).first] = (*it).second->Clone();
	}

	if(ref.mShadingInterface != nullptr)this->mShadingInterface = ref.mShadingInterface->Clone();
	else this->mShadingInterface = nullptr;
}
