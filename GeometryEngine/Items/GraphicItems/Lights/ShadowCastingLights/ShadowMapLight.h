#pragma once

#ifndef GEOMETRYSHADOWMAPLIGHT_H
#define GEOMETRYSHADOWMAPLIGHT_H

#include "../../StencilTestLight.h"

namespace GeometryEngine
{
	namespace GeometryItemUtils
	{
		class Viewport;
	}

	namespace GeometryWorldItem
	{
		namespace GeometryLight
		{
			/// Struct that contains string constants with that represent keys for shadow map shaders
			struct ShadowMapConstants
			{
				static const std::string SHADOW_MAP_FRAGMENT_SHADER;
				static const std::string POSITION_VERTEX_SHADER;
				static const std::string DIRECTIONAL_SHADOW_LIGHT_FRAGMENT_SHADER;
				static const std::string DIRECTIONAL_SHADOW_LIGHT_VERTEX_SHADER;
				static const std::string SPOTLIGHT_SHADOW_LIGHT_FRAGMENT_SHADER;
				static const std::string SPOTLIGHT_SHADOW_LIGHT_VERTEX_SHADER;
			};

			/// Abstract class that defines common methods for lights using shadow maps
			class ShadowMapLight : public StencilTestLight
			{
			public:
				/// Constructor
				/// param viewport Viewport that will be used for the shadowmap calculation.
				/// param direction Shadowmaps work only with single direction lights 
				/// param boundingBox Geomtry used to render the light. Light will be applied to every part of the scene that the geometry is drawn on top of.
				/// param diffuse Diffuse component of the light as an rgb vector. Each color goes from 0.0 to 1.0
				/// param ambient Ambient component of the light. Each color goes from 0.0 to 1.0
				/// param specular Specular component of the light. Each color goes from 0.0 to 1.0
				/// param pos Initial position of the item
				/// param rot Initial rotaion of the item
				/// param mMaxShadowBias Max shadow bias value allowed when calculating dynamic shadow bias. A greater value prevents shadow acne but may cause some shadows disappear suddenly.
				/// param scale Initial scale to be applied to this item model
				/// param manager Light functionalities manager, defaults to nullptr.
				/// param parent Pointer to this items parent item, nullptr if none.
				ShadowMapLight(const GeometryItemUtils::Viewport& viewport, const QVector3D& direction, GeometryItem::GeometryItem* boundingBox = nullptr,
					const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f), float maxShadowBias = 0.0f,
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), const LightUtils::LightFunctionalities* const manager = nullptr, WorldItem* parent = nullptr);

				/// Copy constructor
				/// param ref Const reference to StencilTestLight to be copied
				ShadowMapLight(const ShadowMapLight& ref) { copy(ref); };

				/// Destructor
				virtual ~ShadowMapLight();

				/// Updates the model matrix of this Item and updates the View matrix of the viewport
				/// param calculateChildren true if children matrix should be updated
				virtual void UpdateModelMatrix(bool updateChildren = false) override;

				///  Resizes the viewport used to calculate the shadowmap
				virtual void ResizeElements(int screenWidth, int screenHeight) override;
				///Sets the light direction. This direction is affected by the item rotation
				void SetDirection(const QVector3D& dir) { mDirection = dir; }
				///Returs the light direction
				const QVector3D& GetDirection() { return mDirection; }
				///Sets the max value for the shadow bias to be applied. A greater value prevents shadow acne but may cause some shadows disappear suddenly.
				void SetMaxShadowBias(float maxBias) { mMaxShadowBias = maxBias; }
				/// Returns the max value for the shadow bias parameter.
				float GetMaxShadowBias() { return mMaxShadowBias; }
				/// Returns the light viewport object
				GeometryItemUtils::Viewport& GetLightViewport() { return *mpViewport; }

				/// Factory method. Returns a copy of this object.
				/// return A copy of this object.
				virtual ShadowMapLight* Clone() const = 0;

			protected:
				/// Private constructor for object copies
				ShadowMapLight() {}

				GeometryItemUtils::Viewport* mpViewport;
				QVector3D mDirection;

				float mMaxShadowBias;

				///Method that checks what light functionalities the manager contains and acts on them
				virtual void checkLightFunctionalities();
				/// Checks if the stencil test funcyionality exists and adds it to the manager if it doesn't.
				virtual void checkShadowMapFuntionality();
				/// Copies the data from a ShadowMapLight into this object
				/// param ref ShadowMapLight to be copied
				virtual void copy(const ShadowMapLight& ref);
			};
		}
	}
}
#endif