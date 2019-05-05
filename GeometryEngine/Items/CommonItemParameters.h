#pragma once

#ifndef COMMONITEMPARAMETERS_H
#define COMMONITEMPARAMETERS_H

#include <qmatrix4x4.h>

namespace GeometryEngine
{
	struct LightingTransformationData
	{
		const QMatrix4x4& ProjectionMatrix;
		const QMatrix4x4& ViewMatrix;
		const QMatrix4x4& ModelMatrix;
		const QQuaternion& ModelRotation;

		LightingTransformationData(const QMatrix4x4& projection, const QMatrix4x4& view, const QMatrix4x4& model, const QQuaternion& rotation) :
			ProjectionMatrix(projection), ViewMatrix(view), ModelMatrix(model), ModelRotation(rotation) {}

		QMatrix3x3 GetNormalMatrix() const { return (ViewMatrix * ModelMatrix).normalMatrix(); }
	};

	struct GBufferTextureInfo
	{
		unsigned int AmbientTexture;
		unsigned int DiffuseTexture;
		unsigned int ReflectiveTexture;
		unsigned int EmissiveTexture;
		unsigned int PositionTexture;
		unsigned int NormalTexture;
		unsigned int FinalTexture;
		unsigned int TmpTexture;
		bool UseAmbientTexture;
		bool UseDiffuseTexture;
		bool UseReflectiveTexture;
		bool UseEmissiveTexture;
		bool UsePositionTexture;
		bool UseNormalTexture;
		bool UseTmpTexture;
		const QVector2D& TextureSize;

		GBufferTextureInfo(unsigned int ambColorTexture, unsigned int diffColorTexture, unsigned int refColorTexture, unsigned int emiColorTexture,
			unsigned int posTexture, unsigned int normalTexture, int tmpTexture, int finalTexture, bool useAmb, bool useDiff, bool useRef, bool useEmi,
			bool usePos, bool useNormal, bool useTmpTex, const QVector2D& texSize) :
			AmbientTexture(ambColorTexture), DiffuseTexture(diffColorTexture), ReflectiveTexture(refColorTexture), EmissiveTexture(emiColorTexture),
			PositionTexture(posTexture), NormalTexture(normalTexture), TmpTexture(tmpTexture), FinalTexture(finalTexture), UseAmbientTexture(useAmb),
			UseDiffuseTexture(useDiff), UseReflectiveTexture(useRef), UseEmissiveTexture(useEmi), UsePositionTexture(usePos), UseNormalTexture(useNormal), 
			UseTmpTexture(useTmpTex), TextureSize(texSize) {}

		GBufferTextureInfo(const QVector2D& texSize) :
			AmbientTexture(0), DiffuseTexture(0), ReflectiveTexture(0), EmissiveTexture(0),
			PositionTexture(0), NormalTexture(0), TmpTexture(0), FinalTexture(0), UseAmbientTexture(0),
			UseDiffuseTexture(false), UseReflectiveTexture(false), UseEmissiveTexture(false), UsePositionTexture(false), UseNormalTexture(false),
			UseTmpTexture(false), TextureSize(texSize) {}


	};
}

#endif