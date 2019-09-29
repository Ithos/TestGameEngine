#include "Material.h"

const std::string GeometryEngine::GeometryMaterial::MaterialConstants::TEXTURE_MATERIAL_VERTEX_SHADER = "TEXTURE_MATERIAL_VERTEX_SHADER";
const std::string GeometryEngine::GeometryMaterial::MaterialConstants::TEXTURE_MATERIAL_FRAGMENT_SHADER = "TEXTURE_MATERIAL_FRAGMENT_SHADER";

const std::string GeometryEngine::GeometryMaterial::MaterialConstants::COLOR_MATERIAL_VERTEX_SHADER = "COLOR_MATERIAL_VERTEX_SHADER";
const std::string GeometryEngine::GeometryMaterial::MaterialConstants::COLOR_MATERIAL_FRAGMENT_SHADER = "COLOR_MATERIAL_FRAGMENT_SHADER";

const std::string GeometryEngine::GeometryMaterial::MaterialConstants::VERTEX_COLOR_MATERIAL_VERTEX_SHADER = "VERTEX_COLOR_MATERIAL_VERTEX_SHADER";
const std::string GeometryEngine::GeometryMaterial::MaterialConstants::VERTEX_COLOR_MATERIAL_FRAGMENT_SHADER = "VERTEX_COLOR_MATERIAL_FRAGMENT_SHADER";
const std::string GeometryEngine::GeometryMaterial::MaterialConstants::MULTI_TEXTURE_MATERIAL_FRAGMENT_SHADER = "MULTI_TEXTURE_MATERIAL_FRAGMENT_SHADER";

const std::string GeometryEngine::GeometryMaterial::MaterialConstants::NORMALMAP_TEXTURE_MATERIAL_VERTEX_SHADER = "NORMALMAP_TEXTURE_MATERIAL_VERTEX_SHADER";
const std::string GeometryEngine::GeometryMaterial::MaterialConstants::TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER = "TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER";
const std::string GeometryEngine::GeometryMaterial::MaterialConstants::MULTI_TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER = "MULTI_TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER";


GeometryEngine::GeometryMaterial::Material::Material(const QVector3D & ambient, const QVector3D & diffuse, const QVector3D & specular, const QVector3D& emissive,
	float shininess) : mpProgram(nullptr),
	mAmbient(ambient), mDiffuse(diffuse), mSpecular(specular), mShininess(shininess), mEmissive(emissive), mpShaderManager(nullptr), mpConfInstance(nullptr)
{
}

GeometryEngine::GeometryMaterial::Material::Material(const Material & mat)
{
	copy(mat);
}

GeometryEngine::GeometryMaterial::Material::~Material()
{
	if (mpProgram != nullptr)
	{
		delete mpProgram;
		mpProgram = nullptr;
	}
}

void GeometryEngine::GeometryMaterial::Material::Draw(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf,
	unsigned int totalVertexNumber, unsigned int totalIndexNumber, const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem & parent)
{
	if (mpProgram != nullptr)
	{
		// Link shader pipeline
		if (!mpProgram->link())
		{
			assert(false && "Material --> Program failed to link");
		}

		// Bind shader pipeline for use
		if (!mpProgram->bind())
		{
			assert(false && "Material --> Program failed to bind");
		}

		setProgramParameters(projection, view, parent);

		drawMaterial(vertexBuf, indexBuf, totalVertexNumber, totalIndexNumber);
	}
}

void GeometryEngine::GeometryMaterial::Material::initMaterial()
{
	mpConfInstance = Configuration::ConfigurationManager::GetInstance();
	mpShaderManager = ShaderFiles::ShaderManager::GetInstance(mpConfInstance->getVertexShaderFolder(), mpConfInstance->getFragmentShaderFolder(),
		mpConfInstance->getVertexShaderConfig(), mpConfInstance->getFragmentShaderConfig());

	mpProgram = new QOpenGLShaderProgram();

	this->initShaders();
	this->initProgram();
}

void GeometryEngine::GeometryMaterial::Material::initProgram()
{
	if (mVertexShaderKey != "")
	{
		if (!mpShaderManager->IsLoaded(mVertexShaderKey))
		{
			mpShaderManager->LoadShader(mVertexShaderKey);
		}

		// Compile vertex shader
		if (!mpProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, mpShaderManager->GetLoadedShaderContent(mVertexShaderKey).c_str()))
		{
			assert(false && "Material --> VertexShader failed to compile");
		}
	}

	if (mFragmentShaderKey != "")
	{
		if (!mpShaderManager->IsLoaded(mFragmentShaderKey))
		{
			mpShaderManager->LoadShader(mFragmentShaderKey);
		}

		// Compile fragment shader
		if (!mpProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, mpShaderManager->GetLoadedShaderContent(mFragmentShaderKey).c_str()))
		{
			assert(false && "Material --> FragmentShader failed to compile");
		}
	}
}

void GeometryEngine::GeometryMaterial::Material::copy(const Material & mat)
{
	this->mAmbient = mat.mAmbient;
	this->mDiffuse = mat.mDiffuse;
	this->mSpecular = mat.mSpecular;
	this->mShininess = mat.mShininess;
	this->mEmissive = mat.mEmissive;
	this->mLit = mat.mLit;
	this->mpConfInstance = mat.mpConfInstance;
	this->mFragmentShaderKey = mat.mFragmentShaderKey;
	this->mpProgram = nullptr;
	this->mpShaderManager = mat.mpShaderManager;
	this->mVertexShaderKey = mat.mVertexShaderKey;
}
