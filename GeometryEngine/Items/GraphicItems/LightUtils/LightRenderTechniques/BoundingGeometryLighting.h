#pragma once

#ifndef GEOMETRYBOUNDLIGHTING_H
#define GEOMETRYBOUNDLIGHTING_H

#include <assert.h>

#include "../../../CommonItemParameters.h"
#include "../LightRenderTechnique.h"
#include "../../../GeometryItem.h"

namespace GeometryEngine
{
	namespace LightUtils
	{
		/// Class that limits the lighting to a defined geometry
		template<class T>
		class BoundingGeometryLighting : public LightRenderTechnique
		{
		public:
			/// Constructor
			/// param parent Pointer to the CustomShadingInterface that contains it 
			/// param Value that indicates at which render stepthis shading technique will be used
			BoundingGeometryLighting(LightFunctionalities* parent, LightTechniques step) : LightRenderTechnique(parent, step) {};

			/// Copy constructor
			/// param ref Object to be copied.
			BoundingGeometryLighting(const LightRenderTechnique& ref) { copy(ref); }

			/// Destructor
			virtual ~BoundingGeometryLighting() {};

			/// Abstract method. Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual BoundingGeometryLighting* Clone(LightFunctionalities* parent, LightTechniques step) const {
				BoundingGeometryLighting* cloned = new BoundingGeometryLighting((*this));
				cloned->AddToInterface(parent, step);
				return cloned;
			}

			/// Apply light technique. 
			/// param projectionMatrix Camera projection matrix
			/// param viewMatrix Camera view matrix
			/// param gBuffInfo Texture info from the camera buffers
			/// param viewPos Position of the camera
			virtual void ApplyTechnique(const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const BuffersInfo& buffersInfo, const QVector3D& viewPos) override
			{
				assert(mpTargetLight != nullptr && "Parent light not found");
				GeometryWorldItem::GeometryItem::GeometryItem* boundingGeometry = ((T*)mpTargetLight)->GetBoundingGeometry();
				assert( boundingGeometry != nullptr && "Bounding geometry not found");

				boundingGeometry->CalculateModelMatrix();
				LightingTransformationData ltd(projectionMatrix, viewMatrix, boundingGeometry->GetModelMatrix(), boundingGeometry->GetRotation());

				assert(buffersInfo.GeometryBufferInfo != nullptr && "Geometry buffer info not found");

				mpTargetLight->CalculateLighting(boundingGeometry->GetArrayBuffer(), boundingGeometry->GetIndexBuffer(), ltd, (*buffersInfo.GeometryBufferInfo), viewPos,
					boundingGeometry->GetVertexNumber(), boundingGeometry->GetIndexNumber());
			}

		};
	}
}

#endif