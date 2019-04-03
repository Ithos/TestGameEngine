#pragma once
#ifndef VERTEXCOLORMATERIAL_H
#define VERTEXCOLORMATERIAL_H

#include "Material.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		class VertexColorMaterial : public Material
		{
		public:
			VertexColorMaterial();
			VertexColorMaterial(const VertexColorMaterial& mat);
			virtual ~VertexColorMaterial();
			virtual Material* Clone() const override { return new VertexColorMaterial((*this)); }
		protected:
			virtual void initShaders();
			virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent) override;
			virtual void drawMaterial(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) override;
			virtual void copy(const VertexColorMaterial& mat);
		};
	}
}

#endif