#pragma once

#ifndef GEOMETRYDIRECTIONALLIGHT_H
#define GEOMETRYDIRECTIONALLIGHT_H

#include "../DeferredShadingLight.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryLight
		{
			/// Class that defines a global ligthsource on the scene with a direction for reflection generally used as sunlight
			/// This class expects a 2D bounding geometry that will be rendered in front of the camera, applying the light like a filter.
			class DirectionalLight : public DeferredShadingLight
			{
			public:
				/// Constructor
				/// param direction light direction vector
				/// param boundingBox Geomtry used to render the light. Light will be applied to every part of the scene that the geometry is drawn on top of. 
				/// This class expects a 2D bounding geometry that will be rendered in front of the camera, applying the light like a filter.
				/// param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
				/// param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
				/// param specular Specular component of the light. Each color goes from 0.0 to 1.0
				/// param pos Initial position of the item
				/// param rot Initial rotaion of the item
				/// param scale Initial scale to be applied to this item model
				/// param parent Pointer to this items parent item, nullptr if none.
				DirectionalLight(const QVector3D& direction, GeometryItem::GeometryItem* boundingBox = nullptr, 
					const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);

				/// Destructor
				~DirectionalLight();
			protected:
				QVector3D mDirection;
				/// Sets the keys for the light shaders
				virtual void initLightShaders();
				/// Sends parameters to the shaders.
				/// param transformData Matrices of the light
				/// param GBufferTextureInfo Data from the textures of the geometry buffer 
				/// param viewPos Position of the camera
				virtual void setProgramParameters(const LightingTransformationData & transformData,
					const GBufferTextureInfo& gBuffTexInfo, const QVector3D & viewPos);
				/// Binds shaders and draws.
				/// param vertexBuf Array buffer
				/// param indexBuf IndexBuffer
				/// param totalVertexNum Number of vetices
				/// param titalIndexNum Number of indices
				virtual void calculateContribution(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum);
			};
		}
	}
}

#endif