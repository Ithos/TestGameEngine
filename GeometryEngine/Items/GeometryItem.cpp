#include "Materials\Material.h"
#include "GeometryItem.h"

GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem::GeometryItem(const GeometryMaterial::Material& mat, const QVector3D& pos, const QVector3D & rot,	const QVector3D & scale, WorldItem* parent):
	WorldItem(pos, rot, scale, parent), mpArrayBuf(nullptr), mpIndexBuf(nullptr), mpMaterial(nullptr), mpConfInstance(nullptr), mVertexShaderKey(""), mFragmentShaderKey(""), 
	mTotalVertexNumber(0), mTotalIndexNumber(0), mVisible(true), mCastsShadow(true)
{
	mpMaterial = mat.Clone();
}

GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem::GeometryItem(const GeometryItem & ref)
{
	this->Copy(ref);
}

GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem::~GeometryItem()
{
	if (mpArrayBuf != nullptr)
	{
		mpArrayBuf->destroy();
		delete(mpArrayBuf);
		mpArrayBuf = nullptr;
	}

	if (mpIndexBuf != nullptr)
	{
		mpIndexBuf->destroy();
		delete(mpIndexBuf);
		mpIndexBuf = nullptr;
	}

	deleteMaterial();
}

void GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem::DrawItem(const QMatrix4x4& projection, const QMatrix4x4& view)
{
	if (mpMaterial != nullptr)
	{
		mpMaterial->Draw(mpArrayBuf, mpIndexBuf, mTotalVertexNumber, mTotalIndexNumber, projection, view, (*this) );
	}
}

void GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem::SetMaterial(const GeometryMaterial::Material& mat)
{
	deleteMaterial();
	mpMaterial = mat.Clone();
}

void GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem::Copy(const GeometryItem & ref)
{
	this->WorldItem::Copy(ref);

	this->mpArrayBuf = nullptr;
	this->mpIndexBuf = nullptr;
	this->mpMaterial = nullptr; 
	this->mpConfInstance = nullptr; 
	this->mVertexShaderKey = "";
	this->mFragmentShaderKey = "";
	this->mTotalVertexNumber = 0; 
	this->mTotalIndexNumber = 0;
	this->mpMaterial = ref.mpMaterial->Clone();

}

bool GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem::AddRenderGroup(int group)
{
	if(mRenderGroups.find(group) != mRenderGroups.end())
		return false;

	mRenderGroups.insert(group);
	return true;
}

bool GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem::RemoveRenderGroup(int group)
{
	if (mRenderGroups.find(group) == mRenderGroups.end())
		return false;

	mRenderGroups.erase(group);
	return true;
}

void GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem::initItem()
{
	mpConfInstance = Configuration::ConfigurationManager::GetInstance();
	mpShaderManager = ShaderFiles::ShaderManager::GetInstance(mpConfInstance->getVertexShaderFolder(), mpConfInstance->getFragmentShaderFolder(),
		mpConfInstance->getVertexShaderConfig(), mpConfInstance->getFragmentShaderConfig());

	mpArrayBuf = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	mpIndexBuf = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

	// Generate 2 VBOs
	mpArrayBuf->create();
	mpIndexBuf->create();

	this->initGeometry();
}

void GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem::deleteMaterial()
{
	if (mpMaterial != nullptr)
	{
		delete(mpMaterial);
		mpMaterial = nullptr;
	}
}

