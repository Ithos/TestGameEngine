#include "../GeometryItem.h"
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

const std::string GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_COLOR_MATERIAL_VERTEX_SHADER = "ALPHA_COLOR_MATERIAL_VERTEX_SHADER";
const std::string GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_COLOR_MATERIAL_FRAGMENT_SHADER = "ALPHA_COLOR_MATERIAL_FRAGMENT_SHADER";
const std::string GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_TEXTURE_MATERIAL_VERTEX_SHADER = "ALPHA_TEXTURE_MATERIAL_VERTEX_SHADER";
const std::string GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_MULTI_TEXTURE_MATERIAL_FRAGMENT_SHADER = "ALPHA_MULTI_TEXTURE_MATERIAL_FRAGMENT_SHADER";
const std::string GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_MULTI_TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER = "ALPHA_MULTI_TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER";
const std::string GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_TEXTURE_MATERIAL_FRAGMENT_SHADER = "ALPHA_TEXTURE_MATERIAL_FRAGMENT_SHADER";
const std::string GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_NORMALMAP_TEXTURE_MATERIAL_VERTEX_SHADER = "ALPHA_NORMALMAP_TEXTURE_MATERIAL_VERTEX_SHADER";
const std::string GeometryEngine::GeometryMaterial::MaterialConstants::ALPHA_TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER = "ALPHA_TEXTURE_NORMALMAP_MATERIAL_FRAGMENT_SHADER";


GeometryEngine::GeometryMaterial::Material::Material(float shininess) : mpProgram(nullptr), mpShaderManager(nullptr), mpConfInstance(nullptr), mShininess(shininess)
{
	mShininess = checkShininessValue(mShininess);
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
	assert(mpProgram != nullptr && "No material shader program found");
	{
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

	assert(mpProgram != nullptr && "No material shader program found");
	{
		// Link shader pipeline
		if (!mpProgram->link())
		{
			assert(false && "Material --> Program failed to link");
		}
	}
}

void GeometryEngine::GeometryMaterial::Material::copy(const Material & mat)
{
	this->mLit = mat.mLit;
	this->mpConfInstance = mat.mpConfInstance;
	this->mFragmentShaderKey = mat.mFragmentShaderKey;
	this->mpProgram = nullptr;
	this->mpShaderManager = mat.mpShaderManager;
	this->mVertexShaderKey = mat.mVertexShaderKey;
}
