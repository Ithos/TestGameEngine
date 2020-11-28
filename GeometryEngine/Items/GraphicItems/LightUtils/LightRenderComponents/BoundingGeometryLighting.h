#pragma once

#ifndef GEOMETRYBOUNDLIGHTING_H
#define GEOMETRYBOUNDLIGHTING_H

#include <assert.h>

#include "../../../CommonItemParameters.h"
#include "../LightRenderComponent.h"
#include "../../../GeometryItem.h"

namespace GeometryEngine
{
	namespace LightUtils
	{
		/// Class that limits the lighting to a defined geometry
		template<class T>
		class BoundingGeometryLighting : public LightRenderComponent
		{
		public:
			/// Constructor
			/// \param parent Pointer to the CustomShadingInterface that contains it 
			/// \param Value that indicates at which render step this shading technique will be used
			BoundingGeometryLighting(LightComponentManager* parent, LightRender step) : LightRenderComponent(parent, step) {};

			/// Copy constructor
			/// \param ref Object to be copied.
			BoundingGeometryLighting(const LightRenderComponent& ref) { copy(ref); }

			/// Destructor
			virtual ~BoundingGeometryLighting() {};

			/// Abstract method. Factory method. Creates a copy of this object
			/// \param parent Pointer to the LightFunctionalities that contains it 
			/// \param step indicates to which LightRender key this object corresponds
			/// \return Pointer to a copy of this object
			virtual BoundingGeometryLighting* Clone(LightComponentManager* parent, LightRender step) const {
				BoundingGeometryLighting* cloned = new BoundingGeometryLighting((*this));
				cloned->AddToInterface(parent, step);
				return cloned;
			}

			/// Apply light technique. 
			/// \param projectionMatrix Camera projection matrix
			/// \param viewMatrix Camera view matrix
			/// \param gBuffInfo Texture info from the camera buffers
			/// \param viewPos Position of the camera
			virtual void Render(const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const BuffersInfo& buffersInfo, const QVector3D& viewPos) override
			{
				assert(mpTargetLight != nullptr && "Parent light not found");
				GeometryWorldItem::GeometryItem::GeometryItem* boundingGeometry = ((T*)mpTargetLight)->GetBoundingGeometry();
				assert( boundingGeometry != nullptr && "Bounding geometry not found");

				boundingGeometry->CalculateModelMatrix();
				LightingTransformationData ltd(projectionMatrix, viewMatrix, boundingGeometry->GetModelMatrix(), boundingGeometry->GetRotation());

				assert(buffersInfo.GeometryBufferInfo != nullptr && "Geometry buffer info not found");

				mpTargetLight->CalculateLighting(boundingGeometry->GetArrayBuffer(), boundingGeometry->GetIndexBuffer(), ltd, buffersInfo, viewPos,
					boundingGeometry->GetVertexNumber(), boundingGeometry->GetIndexNumber());
			}

		};
	}
}

#endif