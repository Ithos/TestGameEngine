#pragma once

#ifndef GEOMETRYPOINTLIGHT_H
#define GEOMETRYPOINTLIGHT_H

#include "../StencilTestLight.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryLight
		{
			/// Class that defines a light with attenuation with distance. Generally used for lightbulb type lights.
			class PointLight : public StencilTestLight
			{
			public:
				/// Constructor
				/// param attParam attenuation polynomial for the light
				/// param boundingBox Geomtry used to render the light. Light will be applied to every part of the scene that the geometry is drawn on top of.
				/// param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
				/// param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
				/// param specular Specular component of the light. Each color goes from 0.0 to 1.0
				/// param pos Initial position of the item
				/// param rot Initial rotaion of the item
				/// param scale Initial scale to be applied to this item model
				/// param manager Light functionalities manager, defaults to nullptr.
				/// param parent Pointer to this items parent item, nullptr if none.
				PointLight(const QVector3D& attParam, GeometryItem::GeometryItem* boundingBox = nullptr, 
					const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), const LightUtils::LightFunctionalities* const manager = nullptr, WorldItem* parent = nullptr);
				/// Copy constructor
				/// param ref Const reference to PointLight to be copied
				PointLight(const PointLight& ref) { copy(ref); initLight(); };
				/// Destructor
				~PointLight();
				/// Factory method. Returns a copy of this object.
				/// return A copy of this object.
				virtual PointLight* Clone() const { return new PointLight(*this); };
			protected:
				QVector3D mAttenuationParameters;
				/// Sets the keys for the light shaders
				virtual void initLightShaders();
				/// Sends parameters to the shaders.
				/// param transformData Matrices of the light
				/// param GBufferTextureInfo Data from the textures of the geometry buffer 
				/// param viewPos Position of the camera
				virtual void setProgramParameters(const LightingTransformationData & transformData,	const GBufferTextureInfo& gBuffTexInfo, const QVector3D & viewPos);
				/// Binds shaders and draws.
				/// param vertexBuf Vertex buffer
				/// param indexBuf IndexBuffer
				/// param totalVertexNum Number of vetices
				/// param titalIndexNum Number of indices
				virtual void calculateContribution(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum);
				/// The class overrides this method to initialize the bounding geometry for the light 
				virtual void initLight() override;
				/// Copies the data from a PointLight into this object
				/// param ref PointLight to be copied
				virtual void copy(const PointLight& ref) { DeferredShadingLight::copy(ref); this->mAttenuationParameters = ref.mAttenuationParameters; };
			};
		}
	}
}

#endif