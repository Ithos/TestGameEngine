#pragma once


#ifndef GEOMETRYCUBE_H
#define GEOMETRYCUBE_H

#include <vector>
#include "../GeometryItem.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryItem
		{
			/// Geometry Item that defines a Cube
			class Cube : public GeometryItem
			{
			public:
				/// Constructor
				/// param mat Material to be used for rendering
				/// param size length of the cube side
				/// param pos Initial position of the item
				/// param rot Initial rotaion of the item
				/// param scale Initial scale to be applied to this item model
				/// param parent Pointer to this items parent item, nullptr if none.
				/// param textureCoordArray array of at least 24 elements containing texture coordinates, the order of the faces is FRONT, RIGHT, BACK, LEFT, BOTTOM, TOP, by default the texture is repeated on every face
				Cube(const GeometryMaterial::Material& mat, float size = 1.0f, const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr, std::vector<QVector2D>* textureCoordArray = nullptr);
				/// Copy constructor
				/// param ref Object to be copied
				Cube(const Cube& ref);
				/// Destructor
				virtual ~Cube();
				/// Copies the data from a Cube into this object
				/// param ref Cube to be copied
				virtual void Copy(const Cube& ref);
				/// Factory method. Creates a copy of this object
				/// return Pointer to a copy of this object
				virtual GeometryItem* Clone() const override;
			protected:
				/// Initializes vertices an indices buffers
				void initGeometry() override;

				/// Initializes the texture coordinates
				void initTextureCoordinates(std::vector<QVector2D>* textureCoordArray);

				float mSize;
				std::vector<QVector2D> mTextureCoordinateArray;
			};
		}
	}
}

#endif