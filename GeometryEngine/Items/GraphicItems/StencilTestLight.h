#pragma once

#ifndef STENCILTESTLIGHT_H
#define STENCILTESTLIGHT_H

#include "DeferredShadingLight.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryLight
		{
			class StencilTestLight : public DeferredShadingLight
			{
			public:
				StencilTestLight(GeometryItem::GeometryItem* boundingBox = nullptr, const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), 
					const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);

				virtual ~StencilTestLight();

				virtual void CalculateStencil(const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix) override;

				virtual bool GetStencilTest() override { return true; }

			protected:
				std::string mStencilVertexShaderKey;
				std::string mStencilFragmentShaderKey;
				QOpenGLShaderProgram* mpStencilProgram; // Stenil shader


				virtual void initStencilShaders();
				virtual void initStencilProgram();
				virtual void runStencilProgram(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum);
			};
		}
	}
}

#endif
