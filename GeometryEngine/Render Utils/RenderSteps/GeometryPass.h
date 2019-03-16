#pragma once

#ifndef GEOMETRYPASS_H
#define GEOMETRYPASS_H

#include "../RenderStep.h"

namespace GeometryEngine
{
	namespace GeometryRenderStep
	{
		class GeometryPass : public RenderStep
		{
		public:
			GeometryPass() : RenderStep() {}
			GeometryPass(const GeometryPass& ref) { copy(ref); }
			virtual ~GeometryPass() {}

			virtual void Render(GeometryWorldItem::GeometryCamera::Camera* cam = nullptr, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*> * items = nullptr, 
				std::unordered_set<GeometryWorldItem::GeometryLight::Light*> * lights = nullptr) override;
			virtual RenderStep* Clone() const override { return new GeometryPass(*this); }

		protected:
			void initStep();
			void renderGeometry(GeometryWorldItem::GeometryCamera::Camera * cam, std::unordered_set<GeometryWorldItem::GeometryItem::GeometryItem*>* items);
			void drawItem(GeometryWorldItem::GeometryCamera::Camera* cam, GeometryWorldItem::GeometryItem::GeometryItem* item);
			virtual void copy(const GeometryPass& ref) { RenderStep::copy(ref); }
		};
	}
}

#endif