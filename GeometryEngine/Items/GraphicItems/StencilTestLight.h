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
			/// Deferred shading light that adds a stencil test to ensure that light defined in enclosed geometries doesn't light amything else on the scene
			class StencilTestLight : public DeferredShadingLight
			{
			public:
				/// Constructor
				/// param boundingBox Geomtry used to render the light. Light will be applied to every part of the scene that the geometry is drawn on top of.
				/// param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
				/// param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
				/// param specular Specular component of the light. Each color goes from 0.0 to 1.0
				/// param pos Initial position of the item
				/// param rot Initial rotaion of the item
				/// param scale Initial scale to be applied to this item model
				/// param parent Pointer to this items parent item, nullptr if none.
				StencilTestLight(GeometryItem::GeometryItem* boundingBox = nullptr, const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), 
					const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);

				/// Destructor
				virtual ~StencilTestLight();

				/// Updates the stencil buffer which indicates where the light calculations will be applied
				/// param projectionMatrix Camera projection matrix
				/// param viewMatrix View matrix of the camera
				virtual void CalculateStencil(const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix) override;

				/// Method used to check if the light performs a stencil test step
				virtual bool GetStencilTest() override { return true; }

			protected:
				std::string mStencilVertexShaderKey;
				std::string mStencilFragmentShaderKey;
				QOpenGLShaderProgram* mpStencilProgram; // Stenil shader

				/// Gets managers and initializes the stencil shaders
				virtual void initStencilShaders();
				/// Load stencil shaders
				virtual void initStencilProgram();
				/// Binds stencil shaders, sends vertex data and renders the scene
				/// param indexBuf IndexBuffer
				/// param arrayBuf Array buffer
				/// param totalVertexNum Number of vetices
				/// param titalIndexNum Number of indices
				virtual void runStencilProgram(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum);
			};
		}
	}
}

#endif
