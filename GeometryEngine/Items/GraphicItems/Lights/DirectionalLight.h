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
				/// \param direction light direction vector
				/// \param boundingBox Geomtry used to render the light. Light will be applied to every part of the scene that the geometry is drawn on top of. 
				/// This class expects a 2D bounding geometry that will be rendered in front of the camera, applying the light like a filter.
				/// \param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
				/// \param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
				/// \param specular Specular component of the light. Each color goes from 0.0 to 1.0
				/// \param pos Initial position of the item
				/// \param rot Initial rotaion of the item
				/// \param scale Initial scale to be applied to this item model
				/// \param manager Light functionalities manager, defaults to nullptr.
				/// \param parent Pointer to this items parent item, nullptr if none.
				DirectionalLight(const QVector3D& direction, GeometryItem::GeometryItem* boundingBox = nullptr, 
					const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), const LightUtils::LightComponentManager* const manager = nullptr, WorldItem* parent = nullptr);
				/// Copy constructor
				/// \param ref Const reference to DirectionalLight to be copied
				DirectionalLight(const DirectionalLight& ref) { copy(ref); initLight(); };
				/// Destructor
				virtual ~DirectionalLight();
				///Sets the light direction. This direction is affected by the item rotation
				void SetDirection(const QVector3D& dir) { mDirection = dir; }
				///Returs the light direction
				const QVector3D& GetDirection() { return mDirection; }
				/// Factory method. Returns a copy of this object.
				/// \return A copy of this object.
				virtual DirectionalLight* Clone() const { return new DirectionalLight(*this); };
			protected:
				QVector3D mDirection;
				/// Sets the keys for the light shaders
				virtual void initLightShaders();
				/// Sends parameters to the shaders.
				/// \param transformData Matrices of the light
				/// \param buffInfo Data from the textures of the buffers
				/// \param viewPos Position of the camera
				virtual void setProgramParameters(const LightingTransformationData & transformData,
					const BuffersInfo& buffInfo, const QVector3D & viewPos) override;
				/// Binds shaders and draws.
				/// \param vertexBuf Array buffer
				/// \param indexBuf IndexBuffer
				/// \param totalVertexNum Number of vetices
				/// \param titalIndexNum Number of indices
				virtual void calculateContribution(QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum);
				/// Copies the data from a DirectionalLight into this object
				/// \param ref DirectionalLight to be copied
				virtual void copy(const DirectionalLight& ref) { DeferredShadingLight::copy(ref); this->mDirection = ref.mDirection; };
			};
		}
	}
}

#endif