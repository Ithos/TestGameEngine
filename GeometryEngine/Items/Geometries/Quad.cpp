#include "Quad.h"

GeometryEngine::GeometryWorldItem::GeometryItem::Quad::Quad(const GeometryMaterial::Material & mat, float width, float height, const QVector3D & pos, const QVector3D & rot, 
	const QVector3D & scale, WorldItem * parent):
	GeometryItem(mat, pos, rot, scale, parent), mWidth(width), mHeight(height)
{
	initItem();
}

GeometryEngine::GeometryWorldItem::GeometryItem::Quad::Quad(const Quad & ref) : GeometryItem( (const GeometryItem&)ref )
{
	this->Copy(ref);
}

void GeometryEngine::GeometryWorldItem::GeometryItem::Quad::Copy(const Quad & ref)
{
	this->GeometryItem::Copy(ref);

	mWidth = ref.mWidth;
	mHeight = ref.mHeight;

	initItem();
}

GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem * GeometryEngine::GeometryWorldItem::GeometryItem::Quad::Clone() const
{
	return new Quad((*this));
}

void GeometryEngine::GeometryWorldItem::GeometryItem::Quad::initGeometry()
{
	float xPos = mWidth / 2;
	float yPos = mHeight / 2;

	VertexData vertices[] = 
	{
		{ QVector3D(-xPos, -yPos,  0), QVector3D(1.0f, 0.0f,0.0f), QVector2D(0.0f,0.0f),  QVector3D(0.0f, 0.0f, 1.0f) },  // v0
		{ QVector3D(xPos, -yPos,  0),  QVector3D(1.0f, 0.0f,0.0f), QVector2D(1.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f) }, // v1
		{ QVector3D(-xPos,  yPos,  0), QVector3D(1.0f, 0.0f,0.0f), QVector2D(0.0f, 1.0f),  QVector3D(0.0f, 0.0f, 1.0f) },  // v2
		{ QVector3D(xPos,  yPos,  0),  QVector3D(1.0f, 0.0f,0.0f), QVector2D(1.0f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f) }
	};

	GLushort indices[] = {
		0,  1,  2,  3
	};

	// Transfer vertex data to VBO 0
	mpArrayBuf->bind();
	mpArrayBuf->allocate(vertices, 4 * sizeof(VertexData));

	// Transfer index data to VBO 1
	mpIndexBuf->bind();
	mpIndexBuf->allocate(indices, 4 * sizeof(GLushort));

	mTotalVertexNumber = 4;
	mTotalIndexNumber = 4;
}
