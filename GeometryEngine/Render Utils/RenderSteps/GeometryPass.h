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
		virtual ~GeometryPass(){}

		virtual void Render(Camera* cam = nullptr, std::unordered_set<GeometryItem*> * items = nullptr, std::unordered_set<Light*> * lights = nullptr) override;

	protected:
		void initStep();
		void renderGeometry(Camera * cam, std::unordered_set<GeometryItem*>* items);
		void drawItem(Camera* cam, GeometryItem* item);
	};
}

#endif