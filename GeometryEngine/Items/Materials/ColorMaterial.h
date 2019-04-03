#pragma once

#ifndef GEOMETRYCOLORMATERIAL_H
#define GEOMETRYCOLORMATERIAL_H

#include "Material.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		class ColorMaterial : public Material
		{
		public:
			ColorMaterial(const QVector3D& ambient = QVector3D(0.0f, 0.0, 0.0f), const QVector3D& diffuse = QVector3D(0.0f, 0.0f, 0.0f), 
				const QVector3D& specular = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D& emissive = QVector3D(0.0f, 0.0f, 0.0f), float shininess = 10.0f);
			ColorMaterial(const ColorMaterial& mat);
			virtual ~ColorMaterial();
			virtual Material* Clone() const override;


		protected:
			virtual void initShaders();

			virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent) override;
			virtual void drawMaterial(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) override;
			virtual void copy(const ColorMaterial& mat);
		};
	}
}
#endif