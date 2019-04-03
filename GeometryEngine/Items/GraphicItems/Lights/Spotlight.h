#pragma once

#ifndef GEOMETRYSPOTLIGHT_H
#define GEOMETRYSPOTLIGHT_H

#include "../StencilTestLight.h"

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryLight
		{
			class Spotlight : public StencilTestLight
			{
			public:
				Spotlight(float maxLightAngle, const QVector3D& attParams, const QVector3D& direction, GeometryItem::GeometryItem* boundingBox = nullptr, 
					const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);

				~Spotlight();
			protected:
				QVector3D mDirection;
				QVector3D mAttenuationParameters;
				float mMaxLightAngle;
				virtual void initLightShaders();
				virtual void setProgramParameters(const LightingTransformationData& transformData, const GBufferTextureInfo& gBuffTexInfo,
					const QVector3D& viewPos);
				virtual void calculateContribution(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum);
				virtual void initLight();
			};
		}
	}
}

#endif