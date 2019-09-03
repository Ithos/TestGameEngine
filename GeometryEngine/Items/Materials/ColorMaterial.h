#pragma once

#ifndef GEOMETRYCOLORMATERIAL_H
#define GEOMETRYCOLORMATERIAL_H

#include "Material.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		/// Material that defines color by its ambient, diffuse specular and emissive components.
		class ColorMaterial : public Material
		{
		public:
			/// Constructor. Calls init material
			ColorMaterial(const QVector3D& ambient = QVector3D(0.0f, 0.0, 0.0f), const QVector3D& diffuse = QVector3D(0.0f, 0.0f, 0.0f), 
				const QVector3D& specular = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D& emissive = QVector3D(0.0f, 0.0f, 0.0f), float shininess = 10.0f);
			/// Copy constructor
			/// param ref Object to be copied.
			ColorMaterial(const ColorMaterial& mat);
			/// Destructor
			virtual ~ColorMaterial();
			///Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual Material* Clone() const override;


		protected:
			/// Sets the shaders that should be loaded
			virtual void initShaders();
			/// Sends parameters to the shaders.
			/// param projection Projection matrix
			/// param view View matrix			 
			/// param parent geometry item
			virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent) override;
			/// Binds shaders and draws.
			/// param vertexBuf Vertex buffer
			/// param indexBuf IndexBuffer
			/// param totalVertexNum Number of vetices
			/// param titalIndexNum Number of indices
			virtual void drawMaterial(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) override;
			/// Copies the data of a Material object to the current object
			/// param ref Material to be copied
			virtual void copy(const ColorMaterial& mat);
		};
	}
}
#endif