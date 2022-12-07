#pragma once

#ifndef COMMONITEMPARAMETERS_H
#define COMMONITEMPARAMETERS_H

#include <qmatrix4x4.h>

namespace GeometryEngine
{
	/// Class that holds inofrmation about the light to be passed to the shader
	class LightingTransformationData
	{
	public:
		const QMatrix4x4& ProjectionMatrix;
		const QMatrix4x4& ViewMatrix;
		const QMatrix4x4& ModelMatrix;
		const QQuaternion& ModelRotation;

		LightingTransformationData(const QMatrix4x4& projection, const QMatrix4x4& view, const QMatrix4x4& model, const QQuaternion& rotation) :
			ProjectionMatrix(projection), ViewMatrix(view), ModelMatrix(model), ModelRotation(rotation) {}

		QMatrix3x3 GetNormalMatrix() const { return (ViewMatrix * ModelMatrix).normalMatrix(); }
	};

	/// Class that holds information about the geometry buffer textures to be passed to the shader
	class GBufferTextureInfo
	{
	public:
		unsigned int AmbientTexture;
		unsigned int DiffuseTexture;
		unsigned int ReflectiveTexture;
		unsigned int EmissiveTexture;
		unsigned int PositionTexture;
		unsigned int NormalTexture;
		unsigned int FinalTexture;
		unsigned int TmpTexture;
		unsigned int OriginTexture;
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
			UseTmpTexture(useTmpTex), TextureSize(texSize), OriginTexture(0){}

		GBufferTextureInfo(const QVector2D& texSize) :
			AmbientTexture(0), DiffuseTexture(0), ReflectiveTexture(0), EmissiveTexture(0),
			PositionTexture(0), NormalTexture(0), TmpTexture(0), FinalTexture(0), UseAmbientTexture(false),
			UseDiffuseTexture(false), UseReflectiveTexture(false), UseEmissiveTexture(false), UsePositionTexture(false), UseNormalTexture(false),
			UseTmpTexture(false), TextureSize(texSize), OriginTexture(0) {}


	};

	///Class that holds information about the translucency buffer
	class ShadingBufferTextureInfo
	{
	public:
		unsigned int DiffuseColorMapTexture;
		unsigned int SpecularColorMapTexture;
		unsigned int ShadowMapTexture;
		unsigned int TranslucentDepthMapTexture;
		const QVector2D& TextureSize;

		ShadingBufferTextureInfo(unsigned int diffuseColorMap, unsigned int specColorMap, unsigned int shadowMap, unsigned int translucentDepthMap, const QVector2D& texSize) :
			DiffuseColorMapTexture(diffuseColorMap), SpecularColorMapTexture(specColorMap), ShadowMapTexture(shadowMap), TranslucentDepthMapTexture(translucentDepthMap),
			TextureSize(texSize) {}

		ShadingBufferTextureInfo(const QVector2D& texSize) :
			DiffuseColorMapTexture(0), SpecularColorMapTexture(0), ShadowMapTexture(0), TranslucentDepthMapTexture(0), TextureSize(texSize) {}
	};

	/// Class that holds information about different buffers. This class is used to hold and pass information, it doesn´t own the info classes and it won´t destory them.
	class BuffersInfo
	{
	public:
		BuffersInfo(): GeometryBufferInfo(nullptr), ShadingBufferInfo(nullptr) {}

		GBufferTextureInfo* GeometryBufferInfo;
		ShadingBufferTextureInfo* ShadingBufferInfo;
	};
}

#endif