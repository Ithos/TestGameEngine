#pragma once

#pragma once

#ifndef GEOMETRYTRANSPOTLIGHT_H
#define GEOMETRYTRANSPOTLIGHT_H

#include "../ShadowMapLight.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryLight
		{
			///Class that defines a spotlight that casts dynamic shadows and translucent effects
			class TranslucentSpotlight : public ShadowMapLight
			{
			public:
				/// Constructor
				/// \param maxLightAngle Max angle for the light. Attenuation will be applied based on angle deviation
				/// \param attParams Attenuation polynomial for the light
				/// \param viewport Viewport that will be used for the shadowmap calculation.
				/// \param direction light direction vector
				/// \param boundingBox Geomtry used to render the light. Light will be applied to every part of the scene that the geometry is drawn on top of. 
				/// This class expects a 2D bounding geometry that will be rendered in front of the camera, applying the light like a filter.
				/// \param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
				/// \param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
				/// \param specular Specular component of the light. Each color goes from 0.0 to 1.0
				/// \param pos Initial position of the item
				/// \param rot Initial rotaion of the item
				/// \param mMaxShadowBias Max shadow bias value allowed when calculating dynamic shadow bias. A greater value prevents shadow acne but may cause some shadows disappear suddenly.
				/// \param scale Initial scale to be applied to this item model
				/// \param manager Light functionalities manager, defaults to nullptr.
				/// \param parent Pointer to this items parent item, nullptr if none.
				TranslucentSpotlight(float maxLightAngle, const QVector3D& attParams, const GeometryItemUtils::Viewport& viewport,
					const QVector3D& direction, GeometryItem::GeometryItem* boundingBox = nullptr,
					const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), float maxShadowBias = 0.0f,
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), const LightUtils::LightComponentManager* const manager = nullptr, WorldItem* parent = nullptr);

				/// Copy constructor
				/// \param ref Const reference to StencilTestLight to be copied
				TranslucentSpotlight(const TranslucentSpotlight& ref) { copy(ref); initLight(); };

				/// Destructor
				virtual ~TranslucentSpotlight();

				/// Factory method. Returns a copy of this object.
				/// \return A copy of this object.
				virtual TranslucentSpotlight* Clone() const override { return new TranslucentSpotlight((*this)); }
			protected:
				QVector3D mAttenuationParameters;
				float mMaxLightAngle;
				/// Private constructor for object copies
				TranslucentSpotlight() {}
				/// Sets the keys for the light shaders
				virtual void initLightShaders() override;
				/// Sends parameters to the shaders.
				/// \param transformData Matrices of the light
				/// \param buffInfo Data from the textures of the buffers
				/// \param viewPos Position of the camera
				virtual void setProgramParameters(const LightingTransformationData& transformData, const BuffersInfo& buffInfo, const QVector3D& viewPos) override;
				/// Binds shaders and draws.
				/// \param vertexBuf Vertex buffer
				/// \param indexBuf IndexBuffer
				/// \param totalVertexNum Number of vetices
				/// \param titalIndexNum Number of indices
				virtual void calculateContribution(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum) override;
				/// The class overrides this method to initialize the bounding geometry for the light 
				virtual void initLight() override;
				/// Copies the data from a ShadowMapLight into this object
				/// \param ref ShadowMapLight to be copied
				virtual void copy(const TranslucentSpotlight& ref);
			};
		}
	}
}
#endif