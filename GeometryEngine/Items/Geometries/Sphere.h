#pragma once

#ifndef GEOMETRYSPHERE_H
#define GEOMETRYSPHERE_H

#include "../GeometryItem.h"
#include <math.h>

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryItem
		{
			/// Geometric item that defines a visual sphere
			class Sphere : public GeometryItem
			{
			public:
				/// Constructor
				/// param mat Material to be used for rendering
				/// param size Radius of the sphere
				/// param verticalResolution number of horizontal cuts used to define the vertices of the sphere
				/// param horizontal resolution number of vertical cuts used to define the vertices of the sphere
				/// param pos Initial position of the item
				/// param rot Initial rotaion of the item
				/// param scale Initial scale to be applied to this item model
				/// param parent Pointer to this items parent item, nullptr if none.
				Sphere(const GeometryMaterial::Material& mat, float size = 1.0f, int verticalResolution = 6, int horizontalResolution = 12, 
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);
				/// Copy constructor
				/// param ref Object to be copied
				Sphere(const Sphere& ref);
				/// Destructor
				virtual ~Sphere();
				/// Copies the data from a Sphere into this object
				/// param ref Sphere to be copied
				virtual void Copy(const Sphere& ref);
				/// Factory method. Creates a copy of this object
				/// return Pointer to a copy of this object
				virtual GeometryItem* Clone() const override;
			protected:
				/// Initializes vertices an indices buffers
				void initGeometry() override;
				float mSize;
				int mNumPsiStep;
				int mNumPhiStep;
				VertexData* mVertices;
				GLushort * mIndices;
			};
		}
	}
}
#endif