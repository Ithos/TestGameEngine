#pragma once

#ifndef GEOMETRYPASS_H
#define GEOMETRYPASS_H

#include "../RenderStep.h"

namespace GeometryEngine
{
	class GeometryPass : public RenderStep
	{
	public:
		GeometryPass() : RenderStep() {}
		GeometryPass(const GeometryPass& ref) { copy(ref); }
		virtual ~GeometryPass(){}

		virtual void Render(Camera* cam = nullptr, std::unordered_set<GeometryItem*> * items = nullptr, std::unordered_set<Light*> * lights = nullptr) override;
		virtual RenderStep* Clone() const override { return new GeometryPass(*this); }

	protected:
		void initStep();
		void renderGeometry(Camera * cam, std::unordered_set<GeometryItem*>* items);
		void drawItem(Camera* cam, GeometryItem* item);
		virtual void copy(const GeometryPass& ref) { RenderStep::copy(ref); }
	};
}

#endif