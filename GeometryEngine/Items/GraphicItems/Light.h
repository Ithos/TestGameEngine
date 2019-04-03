#pragma once

#ifndef GEOMETRYLIGHT_H
#define GEOMETRYLIGHT_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include <ConfigurationManager.h>
#include <ShaderManager.h>

#include "../WorldItem.h"

namespace GeometryEngine
{
	struct LightingTransformationData
	{
		const QMatrix4x4& ProjectionMatrix;
		const QMatrix4x4& ViewMatrix;
		const QMatrix4x4& ModelMatrix;
		const QQuaternion& ModelRotation;

		LightingTransformationData(const QMatrix4x4& projection, const QMatrix4x4& view, const QMatrix4x4& model, const QQuaternion& rotation): 
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
		unsigned int TexcoordTexture;
		bool UseAmbientTexture;
		bool UseDiffuseTexture;
		bool UseReflectiveTexture;
		bool UseEmissiveTexture;
		bool UsePositionTexture;
		bool UseNormalTexture;
		bool UseTexcoordTexture;
		const QVector2D& TextureSize;

		GBufferTextureInfo(unsigned int ambColorTexture, unsigned int diffColorTexture, unsigned int refColorTexture, unsigned int emiColorTexture, 
			unsigned int posTexture, unsigned int normalTexture, unsigned int texcoordTexture, bool useAmb, bool useDiff, bool useRef, bool useEmi,
			bool usePos, bool useNormal, bool useTexcoord, const QVector2D& texSize):
			AmbientTexture(ambColorTexture), DiffuseTexture(diffColorTexture), ReflectiveTexture(refColorTexture), EmissiveTexture(emiColorTexture), 
			PositionTexture(posTexture), NormalTexture(normalTexture), TexcoordTexture(texcoordTexture), UseAmbientTexture(useAmb), UseDiffuseTexture(useDiff), UseReflectiveTexture(useRef),
			UseEmissiveTexture(useEmi), UsePositionTexture(usePos), UseNormalTexture(useNormal), UseTexcoordTexture(useTexcoord), TextureSize(texSize) {}
	};

	namespace GeometryWorldItem
	{
		namespace GeometryLight
		{
			struct LightShaderConstants
			{
				static const std::string AMBIENT_LIGHT_VERTEX_SHADER;
				static const std::string AMBIENT_LIGHT_FRAGMENT_SHADER;

				static const std::string DIRECTIONAL_LIGHT_VERTEX_SHADER;
				static const std::string DIRECTIONAL_LIGHT_FRAGMENT_SHADER;

				static const std::string POINT_LIGHT_VERTEX_SHADER;
				static const std::string POINT_LIGHT_FRAGMENT_SHADER;

				static const std::string FLASHLIGHT_VERTEX_SHADER;
				static const std::string FLASHLIGHT_FRAGMENT_SHADER;

				static const std::string DEFERRED_SHADING_VERTEX_SHADER;

				static const std::string NULL_FRAGMENT_SHADER;
				static const std::string POSITION_VERTEX_SHADER;
			};

			class Light : public WorldItem
			{
			public:
				Light(const QVector3D& diffuse = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& ambient = QVector3D(1.0f, 1.0f, 1.0f), const QVector3D& specular = QVector3D(1.0f, 1.0f, 1.0f),
					const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);

				virtual ~Light();

				virtual void CalculateLighting(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, const LightingTransformationData& transformData, 
					const GBufferTextureInfo& gBuffTexInfo, const QVector3D& viewPos, unsigned int totalVertexNum, unsigned int totalIndexNum);

				virtual void LightFromBoundignGeometry(const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const GBufferTextureInfo& gBuffTexInfo, const QVector3D& viewPos)
				{
					assert(GetBoundingGeometry() != nullptr && "Bounding geometry not found");
				}

				virtual void CalculateStencil(const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix)
				{
					assert(GetStencilTest() && "Stencil test not found");
				}

				virtual WorldItem* const GetBoundingGeometry() { return nullptr; }
				virtual bool GetStencilTest() { return false; }
			protected:
				QVector3D mColorDiffuse; // Vec3 color  + float intensity
				QVector3D mColorAmbient;
				QVector3D mColorSpecular;
				QOpenGLShaderProgram* mpProgram; // Lighting shader
				Configuration::ConfigurationManager* mpConfInstance;
				ShaderFiles::ShaderManager* mpShaderManager;
				std::string mVertexShaderKey;
				std::string mFragmentShaderKey;

				virtual void initLight();
				virtual void initLightProgram();
				virtual void initLightShaders() = 0;
				virtual void setProgramParameters(const LightingTransformationData& transformData, const GBufferTextureInfo& GBuffTexInfo, const QVector3D& viewPos) = 0;
				virtual void calculateContribution(QOpenGLBuffer* arrayBuf, QOpenGLBuffer* indexBuf, unsigned int totalVertexNum, unsigned int totalIndexNum) = 0;
			};
		}
	}
}

#endif