#pragma once

#ifndef RENDERSTEP_H
#define RENDERSTEP_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <unordered_set>
#include "Items\GeometryItem.h"
#include "Items\GraphicItems\Camera.h"
#include "Items\GraphicItems\Light.h"

namespace GeometryEngine
{
	class RenderStep : protected QOpenGLExtraFunctions
	{
	public:
		RenderStep();
		RenderStep(const RenderStep& ref) { copy(ref); };
		virtual ~RenderStep() {};
		virtual void Render(Camera* cam = nullptr, std::unordered_set<GeometryItem*> * items = nullptr, std::unordered_set<Light*> * lights = nullptr) = 0;
		virtual RenderStep* Clone() const = 0;
	protected:
		virtual void copy(const RenderStep& ref) {}
	};
}
#endif