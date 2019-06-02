#pragma once

#ifndef GEOMETRYITEM_H
#define GEOMETRYITEM_H


#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector2D>
#include <QVector3D>
#include <list>

#include <ConfigurationManager.h>
#include <ShaderManager.h>
#include "WorldItem.h"
#include "Materials\Material.h"

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		class Material;
	}

	namespace GeometryWorldItem
	{
		namespace GeometryItem
		{
			class GeometryItem : public WorldItem
			{
			public:
				GeometryItem(const GeometryMaterial::Material& mat, const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);
				GeometryItem(const GeometryItem& ref);
				virtual ~GeometryItem();
				virtual void DrawItem(const QMatrix4x4& projection, const QMatrix4x4& view);
				virtual void Update(const QMatrix4x4& projection, const QMatrix4x4& view) override { DrawItem(projection, view); }
				virtual GeometryMaterial::Material* GetMaterialPtr() { return mpMaterial; }
				virtual QOpenGLBuffer* GetArrayBuffer() { return mpArrayBuf; }
				virtual QOpenGLBuffer* GetIndexBuffer() { return mpIndexBuf; }
				virtual unsigned int GetVertexNumber() { return mTotalVertexNumber; }
				virtual unsigned int GetIndexNumber() { return mTotalIndexNumber; }
				virtual void SetMaterial(GeometryMaterial::Material* mat);
				virtual void Copy(const GeometryItem& ref);
				virtual GeometryItem* Clone() const = 0;
				virtual bool AddRenderGroup(int group);
				virtual bool RemoveRenderGroup(int group);
				virtual const std::unordered_set<int>& GetRenderGroups() { return mRenderGroups; }
				virtual void ClearRenderGroups() { mRenderGroups.clear(); }

			protected:
				virtual void initGeometry() = 0;
				virtual void initItem();


				QOpenGLBuffer* mpArrayBuf;
				QOpenGLBuffer* mpIndexBuf;
				GeometryMaterial::Material* mpMaterial;
				Configuration::ConfigurationManager* mpConfInstance;
				std::string mVertexShaderKey;
				std::string mFragmentShaderKey;
				ShaderFiles::ShaderManager* mpShaderManager;
				unsigned int mTotalVertexNumber;
				unsigned int mTotalIndexNumber;
				std::unordered_set<int> mRenderGroups;


			};
		}
	}
}

#endif