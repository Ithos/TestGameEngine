#pragma once

#ifndef EMISSIVELIGHTING_H
#define EMISSIVELIGHTING_H

#include "../DeferredShadingLight.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryLight
		{
			/// Class that draws the emissive colors and textures of the scene
			/// This class expects a 2D bounding geometry that will be rendered in front of the camera, applying the light like a filter.
			class EmissiveLighting : public DeferredShadingLight
			{
			public: 
				/// Constructor
				/// param boundingBox Geomtry used to render the light. Light will be applied to every part of the scene that the geometry is drawn on top of. 
				/// This class expects a 2D bounding geometry that will be rendered in front of the camera, applying the light like a filter.
				/// param manager Light functionalities manager, defaults to nullptr.
				/// param parent Pointer to this items parent item, nullptr if none.
				EmissiveLighting(GeometryItem::GeometryItem* boundingBox = nullptr, const LightUtils::LightComponentManager* const manager = nullptr, WorldItem* parent = nullptr);
				/// Copy constructor
				/// param ref Const reference to EmissiveLighting to be copied
				EmissiveLighting(const EmissiveLighting& ref) { copy(ref); initLight(); };
				/// Destructor
				virtual ~EmissiveLighting();
				/// Factory method. Returns a copy of this object.
				/// return A copy of this object.
				virtual EmissiveLighting* Clone() const { return new EmissiveLighting(*this); };
			private:
				/// Sets the keys for the light shaders
				virtual void initLightShaders();
				/// Sends parameters to the shaders.
				/// param transformData Matrices of the light
				/// param buffInfo Data from the textures of the buffers
				/// param viewPos Position of the camera
				virtual void setProgramParameters(const LightingTransformationData & transformData,
					const BuffersInfo& buffInfo, const QVector3D & viewPos) override;
				/// Binds shaders and draws.
				/// param vertexBuf Array buffer
				/// param indexBuf IndexBuffer
				/// param totalVertexNum Number of vetices
				/// param titalIndexNum Number of indices
				virtual void calculateContribution(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum);
				/// Copies the data from a EmissiveLighting into this object
				/// param ref EmissiveLighting to be copied
				virtual void copy(const EmissiveLighting& ref) { DeferredShadingLight::copy(ref); };
			};
		}
	}
}

#endif