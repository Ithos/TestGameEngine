#pragma once

#ifndef GEOMETRYQUAD_H
#define GEOMETRYQUAD_H

#include "../GeometryItem.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryItem
		{
			/// Geometry Item that defines a quad
			class Quad : public GeometryItem
			{
			public:
				/// Constructor
				/// param mat Material to be used for rendering
				/// param width width of the quad
				/// param heigth height of the quad
				/// param pos Initial position of the item
				/// param rot Initial rotaion of the item
				/// param scale Initial scale to be applied to this item model
				/// param parent Pointer to this items parent item, nullptr if none.
				Quad(const GeometryMaterial::Material& mat, float width, float height, const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);
				/// Copy constructor
				/// param ref Object to be copied
				Quad(const Quad& ref);
				/// Destructor
				virtual ~Quad() {};
				/// Copies the data from a Quad into this object
				/// param ref Quad to be copied
				virtual void Copy(const Quad& ref);
				/// Factory method. Creates a copy of this object
				/// return Pointer to a copy of this object
				virtual GeometryItem* Clone() const override;

			protected:
				/// Initializes vertices an indices buffers
				void initGeometry() override;
				float mWidth;
				float mHeight;
			};
		}
	}
}
#endif