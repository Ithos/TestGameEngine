#include "TextureMaterial.h"

GeometryEngine::GeometryMaterial::TextureMaterial::TextureMaterial(const std::string& texDir, float shininess) : 
	Material(QVector3D(0.0f,0.0f,0.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), shininess), mpTexDirManager(nullptr)
{
	std::list<TextureParameters*> tmpList;
	TextureParameters tmpParam = TextureParameters(texDir, -1);
	tmpList.push_back(&tmpParam);
	initMaterial(tmpList);
}

GeometryEngine::GeometryMaterial::TextureMaterial::TextureMaterial(const std::list<TextureParameters* >& textureDirs, float shininess) : 
	Material(QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 0.0f), shininess), mpTexDirManager(nullptr)
{
	initMaterial( textureDirs );
}

GeometryEngine::GeometryMaterial::TextureMaterial::TextureMaterial(const TextureMaterial & mat)
{
	copy(mat);
	buildTextures(this->mTexturesList);
}

GeometryEngine::GeometryMaterial::Material * GeometryEngine::GeometryMaterial::TextureMaterial::Clone() const
{
	return new TextureMaterial(*this);
}

GeometryEngine::GeometryMaterial::TextureMaterial::~TextureMaterial()
{
	DeleteAllTextures();
}

void GeometryEngine::GeometryMaterial::TextureMaterial::AddTexture(const TextureParameters& texDir)
{
	mTexturesList.push_back(new TextureParameters(texDir) );
}

void GeometryEngine::GeometryMaterial::TextureMaterial::AddTextures(std::list<TextureParameters*> textureDirs)
{
	for (auto it = textureDirs.begin(); it != textureDirs.end(); ++it)
	{
		AddTexture( *(*it) );
	}
}

void GeometryEngine::GeometryMaterial::TextureMaterial::InsertTexture(const TextureParameters& texDir, int index)
{
	auto iter = mTexturesList.begin();
	std::advance(iter, index);
	mTexturesList.insert(iter, new TextureParameters(texDir));
}

void GeometryEngine::GeometryMaterial::TextureMaterial::DeleteTexture(int index)
{
	auto iter = mTexturesList.begin();
	std::advance(iter, index);
	delete((*iter));
	(*iter) = nullptr;
	mTexturesList.erase(iter);
}

void GeometryEngine::GeometryMaterial::TextureMaterial::DeleteAllTextures()
{
	for (auto it = mTexturesList.begin(); it != mTexturesList.end(); ++it)
	{
		delete((*it));
		(*it) = nullptr;
	}

	mTexturesList.clear();
}

void GeometryEngine::GeometryMaterial::TextureMaterial::setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem & parent)
{
	assert(mpProgram != nullptr && "Texture Material --> Shader Program Null");
	if(mpProgram != nullptr)
	{
		// Set matrices
		mpProgram->setUniformValue("modelViewProjectionMatrix", projection * view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelViewMatrix", view * parent.GetModelMatrix());
		mpProgram->setUniformValue("modelMatrix", parent.GetModelMatrix());

		mpProgram->setUniformValue("texture", TEXTURE_UNIT );
	}
}

void GeometryEngine::GeometryMaterial::TextureMaterial::drawMaterial(QOpenGLBuffer * vertexBuf, QOpenGLBuffer * indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber)
{
	assert(mpProgram != nullptr && "Texture Material --> Shader Program Null");
	if(mpProgram != nullptr)
	{
		// Tell OpenGL which VBOs to use
		vertexBuf->bind();
		indexBuf->bind();

		// Tell OpenGL programmable pipeline how to locate vertex position data
		int vertexLocation = mpProgram->attributeLocation("posAttr");
		mpProgram->enableAttributeArray(vertexLocation);
		mpProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, VertexData::POSITION_OFFSET, 3, sizeof(VertexData));

		// Tell OpenGL programmable pipeline how to locate texture coordinates
		int textureCoordinate = mpProgram->attributeLocation("TexCoord");
		mpProgram->enableAttributeArray(textureCoordinate);
		mpProgram->setAttributeBuffer(textureCoordinate, GL_FLOAT, VertexData::TEXTURE_COORDINATES_OFFSET, 2, sizeof(VertexData));

		// Tell OpenGL programmable pipeline how to locate normals
		int normalVector = mpProgram->attributeLocation("aNormal");
		mpProgram->enableAttributeArray(normalVector);
		mpProgram->setAttributeBuffer(normalVector, GL_FLOAT, VertexData::NORMALS_OFFSET, 3, sizeof(VertexData));

		int vertexCount = 0;

		if (mTexturesList.size() == 1)
		{
			auto it = mTexturesList.begin();
			std::advance(it, 0);

			if ((*it)->Texture != nullptr)
				(*it)->Texture->bind(TEXTURE_UNIT);

			glDrawElements(GL_TRIANGLE_STRIP, totalIndexNumber, GL_UNSIGNED_SHORT, 0);
			return;
		}

		for (int i = 0; i < mTexturesList.size(); ++i)
		{
			auto it = mTexturesList.begin();
			std::advance(it, i);

			if ((*it)->Texture != nullptr)
				(*it)->Texture->bind(TEXTURE_UNIT);

			int vertexNum = (*it)->VertexNumber > 0 ? (*it)->VertexNumber : totalVertexNumber;

			/// Maybe do the same, but with indices?
			glDrawArrays(GL_TRIANGLE_STRIP, vertexCount, vertexNum); /// Draw sets of 3 vertex you can also go for sets of 4 by swapping 3s for 4s in the call to the method

			vertexCount += (*it)->VertexNumber;
		}
	}
}

void GeometryEngine::GeometryMaterial::TextureMaterial::copy(const TextureMaterial & mat)
{
	Material::copy(mat);
	initMaterial(mat.mTexturesList);
}

void GeometryEngine::GeometryMaterial::TextureMaterial::initTextures(const std::list<TextureParameters*  >& textureDirs)
{
	AddTextures(textureDirs);
}

void GeometryEngine::GeometryMaterial::TextureMaterial::initMaterial(const std::list<TextureParameters* >& textureDirs)
{
	Material::initMaterial();
	initTextures(textureDirs);
	mpTexDirManager = TexturesFiles::Textures::InitInstance(mpConfInstance->getTexturesFolder(), mpConfInstance->getTexturesConfig());
}

void GeometryEngine::GeometryMaterial::TextureMaterial::initShaders()
{
	mVertexShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::TEXTURE_MATERIAL_VERTEX_SHADER;
	mFragmentShaderKey = GeometryEngine::GeometryMaterial::MaterialConstants::TEXTURE_MATERIAL_FRAGMENT_SHADER;
}

void GeometryEngine::GeometryMaterial::TextureMaterial::buildTextures(const std::list<TextureParameters* >& textureDirs)
{
	for (auto it = textureDirs.begin(); it != textureDirs.end(); ++it)
	{
		if ((*it)->Texture == nullptr) (*it)->Build();
	}
}

