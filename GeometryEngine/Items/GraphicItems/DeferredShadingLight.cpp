#include "../CommonItemParameters.h"
#include "../GeometryItem.h"
#include "DeferredShadingLight.h"

GeometryEngine::GeometryWorldItem::GeometryLight::DeferredShadingLight::DeferredShadingLight(GeometryItem::GeometryItem * boundingBox, const QVector3D & diffuse, const QVector3D & ambient,
	const QVector3D & specular, const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent) : Light(diffuse, ambient, specular, pos, rot, scale, parent),
	mpBoundingBox(boundingBox)
{
	if (boundingBox != nullptr)
	{
		mpBoundingBox = boundingBox->Clone();
	}
}

GeometryEngine::GeometryWorldItem::GeometryLight::DeferredShadingLight::~DeferredShadingLight()
{
	if (mpBoundingBox != nullptr)
	{
		delete(mpBoundingBox);
		mpBoundingBox = nullptr;
	}
}

void GeometryEngine::GeometryWorldItem::GeometryLight::DeferredShadingLight::LightFromBoundignGeometry(const QMatrix4x4& projectionMatrix, 
	const QMatrix4x4& viewMatrix, const GBufferTextureInfo& gBuffTexInfo, const QVector3D & viewPos)
{
	assert(GetBoundingGeometry() != nullptr && "Bounding geometry not found");
	mpBoundingBox->CalculateModelMatrix();
	LightingTransformationData ltd(projectionMatrix, viewMatrix, GetBoundingGeometry()->GetModelMatrix(), GetBoundingGeometry()->GetRotation());

	CalculateLighting(mpBoundingBox->GetArrayBuffer(), mpBoundingBox->GetIndexBuffer(), ltd, gBuffTexInfo, viewPos, mpBoundingBox->GetVertexNumber(), mpBoundingBox->GetIndexNumber());
}

void GeometryEngine::GeometryWorldItem::GeometryLight::DeferredShadingLight::ScaleBoundingBox(const QVector3D & attenuation)
{
	if (this->mpBoundingBox != nullptr)
	{
		QVector3D maxColorIntensity = this->mColorDiffuse + this->mColorAmbient + this->mColorSpecular;
		float maxIntensity = std::max(std::max(maxColorIntensity.x(), maxColorIntensity.y()), maxColorIntensity.z());
		if (maxIntensity > 1) maxIntensity = 1.0f;

		float ret = (-attenuation.y() + sqrtf(attenuation.y() * attenuation.y() - 4 * attenuation.z() * (attenuation.x() - 256 * maxIntensity)))
			/
			(2 * attenuation.z());

		mpBoundingBox->Scale(ret * QVector3D(1.0f, 1.0f, 1.0f));
	}
}
