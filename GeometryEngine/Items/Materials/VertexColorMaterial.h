#pragma once
#ifndef VERTEXCOLORMATERIAL_H
#define VERTEXCOLORMATERIAL_H

#include "Material.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		/// Material that defines color by vertex. Only used in very simple objects
		class VertexColorMaterial : public Material
		{
		public:
			/// Constructor. Calls init material
			/// \param shininess Shininess component. Equation: spec contribution = cos(alpha) ^ shininess. If shininess is <= 0 it is set to 0.001 to avoid errors in the shaders.
			VertexColorMaterial(float shininess = 10.0f);
			/// Copy constructor
			/// \param ref Object to be copied.
			VertexColorMaterial(const VertexColorMaterial& mat);
			/// Destructor
			virtual ~VertexColorMaterial();
			///Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual Material* Clone() const override { return new VertexColorMaterial((*this)); }
		protected:
			/// Sets the shaders that should be loaded
			virtual void initShaders();
			/// Sends parameters to the shaders.
			/// \param projection Projection matrix
			/// \param view View matrix			 
			/// \param parent geometry item
			virtual void setProgramParameters(const QMatrix4x4& projection, const QMatrix4x4& view, const GeometryWorldItem::GeometryItem::GeometryItem& parent) override;
			/// Binds shaders and draws.
			/// \param vertexBuf Vertex buffer
			/// \param indexBuf IndexBuffer
			/// \param totalVertexNum Number of vetices
			/// \param titalIndexNum Number of indices
			virtual void drawMaterial(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNumber, unsigned int totalIndexNumber) override;
			/// Copies the data of a Material object to the current object
			/// \param ref Material to be copied
			virtual void copy(const VertexColorMaterial& mat);
		};
	}
}

#endif