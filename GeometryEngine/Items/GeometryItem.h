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


namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		class Material;
	}

	///namespace that contains everething that have a position in the scene
	namespace GeometryWorldItem
	{
		///namespace that contains everything that may have a geometry
		namespace GeometryItem
		{
			/// Base class for every object in the scene that has a visual model
			class GeometryItem : public WorldItem
			{
			public:
				/// Constructor
				/// \param mat Material to be used for rendering
				/// \param pos Initial position of the item
				/// \param rot Initial rotaion of the item
				/// \param scale Initial scale to be applied to this item model
				/// \param parent Pointer to this items parent item, nullptr if none.
				GeometryItem(const GeometryMaterial::Material& mat, const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
					const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);
				/// Copy constructor
				/// \param ref Object to be copied.
				GeometryItem(const GeometryItem& ref);
				/// Destructor
				virtual ~GeometryItem();
				/// Draws the geometry using the material
				/// \param projection Projection matrix provided by the camera
				/// \param view View matrix, provided by the camera
				virtual void DrawItem(const QMatrix4x4& projection, const QMatrix4x4& view);
				/// Calls DrawItem				
				/// \param projection Projection matrix provided by the camera
				/// \param view View matrix, provided by the camera
				virtual void Update(const QMatrix4x4& projection, const QMatrix4x4& view) override { DrawItem(projection, view); }
				/// Gets a pointer to the item material
				/// \return pointer to the item Material
				virtual GeometryMaterial::Material* GetMaterialPtr() { return mpMaterial; }
				/// Gets a pointer to the array buffer of the item
				/// \return pointer to the array buffer of the item
				virtual QOpenGLBuffer* GetArrayBuffer() { return mpArrayBuf; }
				/// Gets a pointer to the index buffer of the item
				/// \return pointer to the index buffer of the item
				virtual QOpenGLBuffer* GetIndexBuffer() { return mpIndexBuf; }
				/// Gets the number of vertices of the model
				/// \return number of vertices of the model
				virtual unsigned int GetVertexNumber() { return mTotalVertexNumber; }
				/// Gets the number of indices of the model
				/// \return number of indices of the model
				virtual unsigned int GetIndexNumber() { return mTotalIndexNumber; }
				/// Sets the material of the item
				/// \param mat new material for the item
				virtual void SetMaterial(const GeometryMaterial::Material& mat);
				/// Copies the data from a GeometryItem into this object
				/// \param ref GeometryItem to be copied
				virtual void Copy(const GeometryItem& ref);
				/// Abstract method. Factory method. Creates a copy of this object
				/// \return Pointer to a copy of this object
				virtual GeometryItem* Clone() const = 0;
				/// Adds this item to a render group. Cameras only render objects contained in the same render groups that they are. All objects start in the null render group.
				/// \param group Identifier of the render group
				/// \return bool true if the object was not already contained in the render group
				virtual bool AddRenderGroup(int group);
				/// Removes this item from a render group. Cameras only render objects contained in the same render groups that they are. 
				/// All objects start in the null render group. An object cannot be removed from the null render group, if an object is not contained in any render group it's considered to be contained
				/// in the null render group.
				/// \param group Identifier of the render group
				/// \return bool true if the object was contained in the render group
				virtual bool RemoveRenderGroup(int group);
				/// Gets the set of render groups this object is contained in
				/// \return set of render groups this object is contained in
				virtual const std::unordered_set<int>& GetRenderGroups() { return mRenderGroups; }
				/// Removes the item from all render groups. An object is not contained in any render group it's considered to be contained in the null render group.
				virtual void ClearRenderGroups() { mRenderGroups.clear(); }
				/// Sets the visibility for this item
				/// \param visible New visibility state
				virtual void SetVisible(bool visible) { mVisible = visible; }
				/// Gets the visibility state for this item
				/// \return the visibility state for this item
				virtual bool IsVisible() { return mVisible; }
				/// Gets wether this items is taken into account when calculating shadows
				/// \return whether this item casts shadows
				virtual bool CastsShadows() { return mCastsShadow; }
				/// Sets whether this item casts shadows
				/// \param castsShadows New shadow state
				virtual void SetCastsShadows(bool castsShadows) { mCastsShadow = castsShadows; }
			protected:
				/// Abstract method. Initializes vertices an indices buffers
				virtual void initGeometry() = 0;
				/// Gets managers and creates buffers
				virtual void initItem();
				

				QOpenGLBuffer* mpArrayBuf;
				QOpenGLBuffer* mpIndexBuf;
				bool mVisible;
				bool mCastsShadow;
				GeometryMaterial::Material* mpMaterial;
				Configuration::ConfigurationManager* mpConfInstance;
				std::string mVertexShaderKey;
				std::string mFragmentShaderKey;
				ShaderFiles::ShaderManager* mpShaderManager;
				unsigned int mTotalVertexNumber;
				unsigned int mTotalIndexNumber;
				std::unordered_set<int> mRenderGroups;

			private:
				/// Deletes the material pointer and sets it as null. This method should only be used when destroying the object or when swaping the object material.
				void deleteMaterial();

			};
		}
	}
}

#endif