#pragma once

#ifndef DEFERREDSHADINGLIGHT_H
#define DEFERREDSHADINGLIGHT_H

#include "Light.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryItem
		{
			class GeometryItem;
		}

		namespace GeometryLight
		{
			/// Class for lights that  are applied to a defined geometry on a separate rendering step
			class DeferredShadingLight : public Light
			{
			public:
				/// Constructor
				/// \param boundingBox Geomtry used to render the light. Light will be applied to every part of the scene that the geometry is drawn on top of.
				/// \param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
				/// \param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
				/// \param specular Specular component of the light. Each color goes from 0.0 to 1.0
				/// \param pos Initial position of the item
				/// \param rot Initial rotaion of the item
				/// \param scale Initial scale to be applied to this item model
				/// \param manager Light functionalities manager, defaults to nullptr.
				/// \param parent Pointer to this items parent item, nullptr if none.
				DeferredShadingLight(GeometryItem::GeometryItem* boundingBox = nullptr, const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), const LightUtils::LightComponentManager* const manager = nullptr, WorldItem* parent = nullptr);

				/// Copy constructor
				/// \param ref Const reference to DeferredShadingLight to be copied
				DeferredShadingLight(const DeferredShadingLight& ref) { copy(ref); initLight(); };

				/// Destructor
				virtual ~DeferredShadingLight();

				/// Method to be implemented by child classes. Returns a pointer to the boundng geometry of the ligh or nullptr if there is none
				/// \return Pointer to the boundng geometry of the ligh or nullptr if there is none
				virtual GeometryItem::GeometryItem* const GetBoundingGeometry() { return mpBoundingBox; }

				/// Factory method. Returns a copy of this object.
				/// \return A copy of this object.
				virtual DeferredShadingLight* Clone() const = 0;

			protected:
				GeometryItem::GeometryItem * mpBoundingBox;
				///Method that checks what light functionalities the manager contains and acts on them
				virtual void checkLightFunctionalities() override;
				/// Checks if the bounding geometry technique exists and adds it to the manager if it doesn't.
				virtual void checkDeferredShadingTechnique();
				/// Scales the bounding box of the light following the light attenuation function.
				/// \param light attenuation polynomial
				virtual void ScaleBoundingBox(const QVector3D& attenuation);
				/// Copies the data from a DeferredShadingLight into this object
				/// \param ref DeferredShadingLight to be copied
				virtual void copy(const DeferredShadingLight& ref);
			};
		}
	}
}

#endif