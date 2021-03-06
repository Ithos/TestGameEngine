#include "Cube.h"

GeometryEngine::GeometryWorldItem::GeometryItem::Cube::Cube( const GeometryMaterial::Material& mat, float size, const QVector3D& pos, const QVector3D & rot, 
	const QVector3D & scale, WorldItem* parent, std::vector<QVector2D>* textureCoordArray):
	GeometryItem(mat, pos, rot, scale, parent), mSize(size)
{
	initTextureCoordinates(textureCoordArray);
	initItem();
}

GeometryEngine::GeometryWorldItem::GeometryItem::Cube::Cube(const Cube & ref) : GeometryItem( (const GeometryItem&)ref )
{
	this->Copy(ref);
}

GeometryEngine::GeometryWorldItem::GeometryItem::Cube::~Cube()
{
}

void GeometryEngine::GeometryWorldItem::GeometryItem::Cube::Copy(const Cube & ref)
{
	this->GeometryItem::Copy(ref);
	this->mSize = ref.mSize;
	initItem();
}

GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem * GeometryEngine::GeometryWorldItem::GeometryItem::Cube::Clone() const
{
	return new Cube((*this));
}

void GeometryEngine::GeometryWorldItem::GeometryItem::Cube::initGeometry()
{
	float pos = mSize / 2.0f;
	// For cube we would need only 8 vertices but we have to
	// duplicate vertex for each face because texture coordinate
	// is different.
	VertexData vertices[] = {
		// Vertex data for face 0
		{ QVector3D(-pos, -pos,  pos), QVector3D(1.0f, 0.0f,0.0f), mTextureCoordinateArray[0],  QVector3D(0.0f, 0.0f, 1.0f) },  // v0
		{ QVector3D(pos, -pos,  pos),  QVector3D(1.0f, 0.0f,0.0f), mTextureCoordinateArray[1], QVector3D(0.0f, 0.0f, 1.0f) }, // v1
		{ QVector3D(-pos,  pos,  pos), QVector3D(1.0f, 0.0f,0.0f), mTextureCoordinateArray[2],  QVector3D(0.0f, 0.0f, 1.0f) },  // v2
		{ QVector3D(pos,  pos,  pos),  QVector3D(1.0f, 0.0f,0.0f), mTextureCoordinateArray[3], QVector3D(0.0f, 0.0f, 1.0f) }, // v3
		

		// Vertex data for face 1
		{ QVector3D(pos, -pos,  pos), QVector3D(0.0f, 1.0f,0.0f), mTextureCoordinateArray[4],  QVector3D(1.0f, 0.0f, 0.0f) }, // v4
		{ QVector3D(pos, -pos, -pos), QVector3D(0.0f, 1.0f,0.0f), mTextureCoordinateArray[5], QVector3D(1.0f, 0.0f, 0.0f) }, // v5
		{ QVector3D(pos,  pos,  pos), QVector3D(0.0f, 1.0f,0.0f), mTextureCoordinateArray[6],  QVector3D(1.0f, 0.0f, 0.0f) },  // v6
		{ QVector3D(pos,  pos, -pos), QVector3D(0.0f, 1.0f,0.0f), mTextureCoordinateArray[7], QVector3D(1.0f, 0.0f, 0.0f) }, // v7

																							   // Vertex data for face 2
		{ QVector3D(pos, -pos, -pos),  QVector3D(0.0f, 0.0f,1.0f),  mTextureCoordinateArray[8], QVector3D(0.0f, 0.0f, -1.0f) }, // v8
		{ QVector3D(-pos, -pos, -pos), QVector3D(0.0f, 0.0f,1.0f),  mTextureCoordinateArray[9],  QVector3D(0.0f, 0.0f, -1.0f) },  // v9
		{ QVector3D(pos,  pos, -pos),  QVector3D(0.0f, 0.0f,1.0f),  mTextureCoordinateArray[10], QVector3D(0.0f, 0.0f, -1.0f) }, // v10
		{ QVector3D(-pos,  pos, -pos), QVector3D(0.0f, 0.0f,1.0f),  mTextureCoordinateArray[11],  QVector3D(0.0f, 0.0f, -1.0f) },  // v11

																								// Vertex data for face 3
		{ QVector3D(-pos, -pos, -pos), QVector3D(1.0f, 1.0f,0.0f), mTextureCoordinateArray[12],  QVector3D(-1.0f, 0.0f, 0.0f) }, // v12
		{ QVector3D(-pos, -pos,  pos), QVector3D(1.0f, 1.0f,0.0f), mTextureCoordinateArray[13],   QVector3D(-1.0f, 0.0f, 0.0f) },  // v13
		{ QVector3D(-pos,  pos, -pos), QVector3D(1.0f, 1.0f,0.0f), mTextureCoordinateArray[14],  QVector3D(-1.0f, 0.0f, 0.0f) }, // v14
		{ QVector3D(-pos,  pos,  pos), QVector3D(1.0f, 1.0f,0.0f), mTextureCoordinateArray[15],   QVector3D(-1.0f, 0.0f, 0.0f) },  // v15

																								// Vertex data for face 4
		{ QVector3D(-pos, -pos, -pos), QVector3D(1.0f, 0.0f,1.0f), mTextureCoordinateArray[16], QVector3D(0.0f, -1.0f, 0.0f) }, // v16
		{ QVector3D(pos, -pos, -pos),  QVector3D(1.0f, 0.0f,1.0f), mTextureCoordinateArray[17], QVector3D(0.0f, -1.0f, 0.0f) }, // v17
		{ QVector3D(-pos, -pos,  pos), QVector3D(1.0f, 0.0f,1.0f), mTextureCoordinateArray[18], QVector3D(0.0f, -1.0f, 0.0f) }, // v18
		{ QVector3D(pos, -pos,  pos),  QVector3D(1.0f, 0.0f,1.0f), mTextureCoordinateArray[19], QVector3D(0.0f, -1.0f, 0.0f) }, // v19

																							   // Vertex data for face 5
		{ QVector3D(-pos,  pos,  pos), QVector3D(0.0f, 1.0f,1.0f), mTextureCoordinateArray[20], QVector3D(0.0f, 1.0f, 0.0f) }, // v20
		{ QVector3D(pos,  pos, pos),   QVector3D(0.0f, 1.0f,1.0f), mTextureCoordinateArray[21], QVector3D(0.0f, 1.0f, 0.0f) }, // v21
		{ QVector3D(-pos,  pos, -pos), QVector3D(0.0f, 1.0f,1.0f), mTextureCoordinateArray[22], QVector3D(0.0f, 1.0f, 0.0f) }, // v22
		{ QVector3D(pos,  pos, -pos),  QVector3D(0.0f, 1.0f,1.0f), mTextureCoordinateArray[23], QVector3D(0.0f, 1.0f, 0.0f) }  // v23
	};

	// Indices for drawing cube faces using triangle strips.
	// Triangle strips can be connected by duplicating indices
	// between the strips. If connecting strips have opposite
	// vertex order then last index of the first strip and first
	// index of the second strip needs to be duplicated. If
	// connecting strips have same vertex order then only last
	// index of the first strip needs to be duplicated.
	GLushort indices[] = {
		0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
		4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
		8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
		12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
		16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
		20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
	};

	// Transfer vertex data to VBO 0
	mpArrayBuf->bind();
	mpArrayBuf->allocate(vertices, 24 * sizeof(VertexData));

	// Transfer index data to VBO 1
	mpIndexBuf->bind();
	mpIndexBuf->allocate(indices, 34 * sizeof(GLushort));

	mTotalVertexNumber = 24;
	mTotalIndexNumber = 34;
}

void GeometryEngine::GeometryWorldItem::GeometryItem::Cube::initTextureCoordinates(std::vector<QVector2D>* textureCoordArray)
{
	if (textureCoordArray == nullptr || textureCoordArray->size() < 24)
	{
		for (int i = 0; i < 6; ++i)
		{
			mTextureCoordinateArray.push_back(QVector2D(0.0f, 0.0f));
			mTextureCoordinateArray.push_back(QVector2D(1.0f, 0.0f));
			mTextureCoordinateArray.push_back(QVector2D(0.0f, 1.0f));
			mTextureCoordinateArray.push_back(QVector2D(1.0f, 1.0f));
		}
	}
	else
	{
		for (int i = 0; i < 24; ++i)
		{
			mTextureCoordinateArray.push_back((*textureCoordArray)[i]);
		}
	}
	
}

