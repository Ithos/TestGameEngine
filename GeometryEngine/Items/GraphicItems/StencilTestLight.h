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
				/// \param boundingBox Geomtry used to render the light. Light will be applied to every part of the scene that the geometry is drawn on top of.
				/// \param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
				/// \param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
				/// \param specular Specular component of the light. Each color goes from 0.0 to 1.0
				/// \param pos Initial position of the item
				/// \param rot Initial rotaion of the item
				/// \param scale Initial scale to be applied to this item model
				/// \param manager Light functionalities manager, defaults to nullptr.
				/// \param parent Pointer to this items parent item, nullptr if none.
				StencilTestLight(GeometryItem::GeometryItem* boundingBox = nullptr, const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), 
					const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), const LightUtils::LightComponentManager* const manager = nullptr, WorldItem* parent = nullptr);

				/// Copy constructor
				/// \param ref Const reference to StencilTestLight to be copied
				StencilTestLight(const StencilTestLight& ref) { copy(ref); initLight(); };

				/// Destructor
				virtual ~StencilTestLight() {};

				/// Factory method. Returns a copy of this object.
				/// \return A copy of this object.
				virtual StencilTestLight* Clone() const = 0;

			protected:
				///Method that checks what light functionalities the manager contains and acts on them
				virtual void checkLightFunctionalities() override;
				/// Checks if the stencil test funcyionality exists and adds it to the manager if it doesn't.
				virtual void checkStencylTestFunctionality();
				/// Copies the data from a DeferredShadingLight into this object
				/// \param ref DeferredShadingLight to be copied
				virtual void copy(const StencilTestLight& ref) { DeferredShadingLight::copy(ref); };
			};
		}
	}
}

#endif
