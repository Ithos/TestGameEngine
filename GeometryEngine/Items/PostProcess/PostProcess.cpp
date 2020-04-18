#include <ConfigurationManager.h>
#include <ShaderManager.h>
#include <qopenglshaderprogram.h>
#include "../CommonItemParameters.h"
#include "../GeometryItem.h"
#include "PostProcess.h"

const std::string GeometryEngine::GeometryPostProcess::PostProcessShaderConstants::POST_PROCESS_VERTEX_SHADER = "POST_PROCESS_VERTEX_SHADER";
const std::string GeometryEngine::GeometryPostProcess::PostProcessShaderConstants::BLUR_POST_PROCESS_FRAGMENT_SHADER = "BLUR_POST_PROCESS_FRAGMENT_SHADER";
const std::string GeometryEngine::GeometryPostProcess::PostProcessShaderConstants::GREY_SCALE_PROCESS_FRAGMENT_SHADER = "GREY_SCALE_PROCESS_FRAGMENT_SHADER";

GeometryEngine::GeometryPostProcess::PostProcess::PostProcess(const GeometryWorldItem::GeometryItem::GeometryItem & boundingGeometry) : mpBoundingGeometry(nullptr),
mpProgram(nullptr), mpConfInstance(nullptr), mpShaderManager(nullptr), mVertexShaderKey(""), mFragmentShaderKey("")
{
	mpBoundingGeometry = boundingGeometry.Clone();
}

GeometryEngine::GeometryPostProcess::PostProcess::PostProcess(const PostProcess & ref)
{
}

GeometryEngine::GeometryPostProcess::PostProcess::~PostProcess()
{
	if (mpBoundingGeometry != nullptr)
	{
		delete mpBoundingGeometry;
		mpBoundingGeometry = nullptr;
	}

	if (mpProgram != nullptr)
	{
		delete mpProgram;
		mpProgram = nullptr;
	}
}

void GeometryEngine::GeometryPostProcess::PostProcess::ApplyPostProcess(const GBufferTextureInfo & gBuffTexInfo)
{
	assert(mpProgram != nullptr && "No post process shader program found");
	{
		// Bind shader pipeline for use
		if (!mpProgram->bind())
		{
			assert(false && "Post process shader failed to bind");
		}

		setProgramParameters(gBuffTexInfo);
		applyFilter();
	}
}

void GeometryEngine::GeometryPostProcess::PostProcess::initPostProcess()
{
	mpConfInstance = Configuration::ConfigurationManager::GetInstance();
	mpShaderManager = ShaderFiles::ShaderManager::GetInstance(mpConfInstance->getVertexShaderFolder(), mpConfInstance->getFragmentShaderFolder(),
		mpConfInstance->getVertexShaderConfig(), mpConfInstance->getFragmentShaderConfig());

	mpProgram = new QOpenGLShaderProgram();

	this->initPostProcessShaders();
	this->initPostProcessProgram();
}

void GeometryEngine::GeometryPostProcess::PostProcess::initPostProcessProgram()
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
			assert(false && "Post process vertex shader failed to compile");
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
			assert(false && "Post process fragment shader failed to compile");
		}
	}

	assert(mpProgram != nullptr && "No post process shader program found");
	{
		// Link shader pipeline
		if (!mpProgram->link())
		{
			assert(false && "Post process shader failed to link");
		}
	}
}

void GeometryEngine::GeometryPostProcess::PostProcess::copy(const PostProcess & ref)
{
	mVertexShaderKey = ref.mVertexShaderKey;
	mFragmentShaderKey = ref.mFragmentShaderKey;
	mpShaderManager = ref.mpShaderManager;
	mpConfInstance = ref.mpConfInstance;
	mpProgram = nullptr;
	mpBoundingGeometry = (ref.mpBoundingGeometry != nullptr ? ref.mpBoundingGeometry->Clone() : nullptr);
}
