#pragma once

#ifndef GEOMETRYCUSTOMSHADINGINTERFACE_H
#define GEOMETRYCUSTOMSHADINGINTERFACE_H

#include <qopenglbuffer.h>
#include <qmatrix4x4.h>

namespace GeometryEngine
{
	/// Interface for objects that implement custom shadow map calculations
	class CustomShadingInterface
	{
	public:
		virtual void CalculateCustomShadowMap(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& modelViewProjection, unsigned int totalVertexNum, unsigned int totalIndexNum) = 0;
		virtual bool GetApplyCustomShadowMap() = 0;
		virtual void SetApplyCustomShadowMap(bool applyCustom) = 0;
	};
}
#endif // !GEOMETRYCUSTOMSHADINGINTERFACE_H
