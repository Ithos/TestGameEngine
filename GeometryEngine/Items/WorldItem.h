#pragma once

#ifndef WORLDITEM_H
#define WORLDITEM_H

#include <QVector3D>
#include <QVector2D>
#include <QQuaternion>
#include <QMatrix4x4>
#include <unordered_set>

namespace GeometryEngine
{
	/// Struct that holds the vertex data to be passed to OpenGl and the memory offset for each varible
	struct VertexData
	{
		QVector3D position;
		QVector3D color;
		QVector2D texCoord;
		QVector3D normal;

		static const unsigned int POSITION_OFFSET = 0;
		static const unsigned int COLOR_OFFSET = sizeof(QVector3D);
		static const unsigned int TEXTURE_COORDINATES_OFFSET = sizeof(QVector3D) + sizeof(QVector3D);
		static const unsigned int NORMALS_OFFSET = sizeof(QVector3D) + sizeof(QVector3D) + sizeof(QVector2D);
	};
	///namespace that contains anithing that a scene may contain
	namespace GeometryWorldItem
	{
		/// Base class for every object that may be included in a scene
		class WorldItem
		{
		public:
			/// Constructor. Calls init.
			/// param pos Initial position of the item
			/// param rot Initial rotaion of the item
			/// param scale Initial scale to be applied to this item model
			/// param parent Pointer to this items parent item, nullptr if none.
			WorldItem(const QVector3D& pos = QVector3D(0.0f, 0.0f, 0.0f), const QVector3D & rot = QVector3D(0.0f, 0.0f, 0.0f),
				const QVector3D & scale = QVector3D(1.0f, 1.0f, 1.0f), WorldItem* parent = nullptr);

			/// Copy constructor
			/// param ref Object to be copied.
			WorldItem(const WorldItem& ref);

			/// Destructor
			virtual ~WorldItem();

			/// Sets a WorldItem as a child of this
			/// param child Child WorldItem
			/// return true if children was added false otherwise
			bool AddChild(WorldItem* child);
			/// Sets a WorlItem as a parent for this
			/// param parent Parent WorldItem
			/// return true if succesful false otherwise
			bool SetParent(WorldItem* parent);
			/// The WorldItem stops being considered a child of this (if it was a child in the first place). Its movement and rotation are no longer linked.
			/// param child WorldItem to be removed 
			/// return true if removed false otherwise
			bool RemoveChild(WorldItem* child);
			/// Removes this parent WorldItem. This movement and rotation are no longer linked
			/// return true if succesful false otherwise
			bool RemoveParent();
			/// Checks if a WorldItem is a child of this
			/// param child World item to be checked
			/// return true if child was found false otherwise
			bool FindChild(WorldItem* child) const;
			/// Returs a pointer to the parent WorldItem
			/// return parent WorldItem
			WorldItem* GetParent() const { return mpParent; }
			/// Returns the position of this modified by its parent position if it has one
			/// return position of this WorldItem
			QVector3D GetPosition() const;
			/// Returns the rotathion of this
			/// return the rotation of this WorldItem
			const QQuaternion& GetRotation() const { return mRotation; }
			/// Returns the scale vector of this
			/// return scale vector for this WorldItem
			const QVector3D& GetScale() const { return mScale; }
			/// Moves the WorldItem along a given vector
			/// param vector Vector of movement
			/// param updateChildren true if the children WorldItems should be updated false otherwise
			void Move(const QVector3D& vector, bool updateChildren = true);
			/// Roatates the WorldItem by a XYZ vector
			/// param rot XYZ rotaions in degrees
			/// param updateChildren true if the children WorldItems should be updated false otherwise
			void Rotate(const QVector3D& rot, bool updateChildren = true);
			/// Rotates the WorldItem by a Quaternion
			/// param rot Rotation expressed as a quaternion
			/// param updateChildren true if the children WorldItems should be updated false otherwise
			void Rotate(const QQuaternion& rot, bool updateChildren = true);
			/// Multiplies each component of the WorldItem scale vector by the correspondent component of the scalation vector
			/// param scale Scalation vector to be applied to  the WorldItem
			/// param updateChildren true if the children WorldItems should be updated false otherwise
			void Scale(const QVector3D& scale, bool updateChildren = true);
			/// Sets a new position to the WorldItem
			/// param vector New position
			/// param delayUpdate false if the model should be updated inmediately
			void SetPosition(const QVector3D& vector, bool delayUpdate = false);
			/// Sets the rotation quaternion of the WorldItem
			/// param rot New rotation quaternion
			/// param delayUpdate false if the model should be updated inmediately
			void SetRotation(const QQuaternion& rot, bool delayUpdate = false);
			/// Sets the rotation of the WorldItem by euler angles
			/// param rot Vector of Euler angles
			/// param delayUpdate false if the model should be updated inmediately
			void SetRotation(const QVector3D& rot, bool delayUpdate = false);
			/// Stes the scalation vector for the WorldItem
			/// param scale New scale vector
			/// param delayUpdate false if the model should be updated inmediately
			void SetScale(const QVector3D& scale, bool delayUpdate = false);
			/// Updates the model matrix
			/// param calculateChildren true if children matrix should be updated
			virtual void CalculateModelMatrix(bool calculateChildren = false);
			/// Updates the model matrix
			/// param calculateChildren true if children matrix should be updated
			virtual void UpdateModelMatrix(bool updateChildren = false);
			/// Method to be implemented by child classes. It should update the visual model and redraw the item.
			/// param projection Projection matrix provided by the camera
			/// param view View matrix, provided by the camera
			virtual void Update(const QMatrix4x4& projection, const QMatrix4x4& view) {};
			/// Transforms a vector from global coordinate systema to model coordinate system
			/// param vector Vector to be transformed
			/// return Transformed vector
			virtual QVector3D ToModelCoordSystem(const QVector3D& vector);
			/// Transforms a vector from global coordinate systema to model coordinate system
			/// param vector Vector to be transformed
			/// return Transformed vector
			virtual QVector3D ToGlobalCoordSystem(const QVector3D& vector);
			/// Gets the model matris for this WorldItem
			/// return ModelMatrix
			virtual const QMatrix4x4& GetModelMatrix() const { return mModelMatrix; }
			/// Copies the data from a WorldItem into this object
			/// param ref WorldItem to be copied
			virtual void Copy(const WorldItem& ref);

		protected:
			/// Position of the WorldItem in global coordinates
			QVector3D mPosition;
			/// Rotaion of the WorldItem as a Quaternion
			QQuaternion mRotation;
			/// Scale vector
			QVector3D mScale;
			/// Child list
			std::unordered_set< WorldItem* > mpChildren;
			/// Pointer to parent WorldItem
			WorldItem* mpParent; // DONT DELETE
			/// Model matrix
			QMatrix4x4 mModelMatrix;

		private:
			/// Initialization method. Sets te position, rotation, scale vector and parent for the WorldItem and updates its model matrix.
			/// param pos Initial position
			/// param rot Initial Euler angles rotation
			/// param scale initial scale vector
			/// param pointer to the parent WorldItem, nullptr if none
			void init(const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent);
		};
	}
}

#endif