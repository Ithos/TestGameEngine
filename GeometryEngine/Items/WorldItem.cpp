#include "WorldItem.h"

GeometryEngine::GeometryWorldItem::WorldItem::WorldItem(const QVector3D& pos, const QVector3D & rot, const QVector3D & scale, WorldItem* parent) : mpParent(parent)
{
	init(pos, rot, scale, parent);
}

GeometryEngine::GeometryWorldItem::WorldItem::WorldItem(const WorldItem & ref)
{
	Copy(ref);
}


GeometryEngine::GeometryWorldItem::WorldItem::~WorldItem()
{
	for (auto iter = mpChildren.begin(); iter != mpChildren.end(); iter++)
	{
		(*iter)->RemoveParent();
	}

	this->RemoveParent();
}

bool GeometryEngine::GeometryWorldItem::WorldItem::AddChild( WorldItem* child)
{
	if (child->GetParent() == nullptr && 
		mpChildren.find(child) == mpChildren.end()
		&& child->SetParent(this))
	{
		mpChildren.insert(child);
		return true;
	}

	return false;
}

bool GeometryEngine::GeometryWorldItem::WorldItem::SetParent( WorldItem* parent)
{
	if (mpParent == nullptr && mpChildren.find(parent) ==mpChildren.end())
	{
		mpParent = parent;
		return true;
	}

	return false;
}

bool GeometryEngine::GeometryWorldItem::WorldItem::RemoveChild( WorldItem* child)
{
	if (child->GetParent() == this &&
		mpChildren.find(child) != mpChildren.end())
	{
		mpChildren.erase(child);
		child->RemoveParent();
		return true;
	}

	return false;
}

bool GeometryEngine::GeometryWorldItem::WorldItem::RemoveParent()
{
	if (mpParent != nullptr)
	{
		mpParent->RemoveChild(this);
		mpParent = nullptr;
		return true;
	}

	return false;
}

bool GeometryEngine::GeometryWorldItem::WorldItem::FindChild(WorldItem* child) const
{
	return mpChildren.find(child) != mpChildren.end();
}

QVector3D GeometryEngine::GeometryWorldItem::WorldItem::GetPosition() const
{
	if (mpParent == nullptr)
	{
		return mPosition;
	}
	else
	{
		return mpParent->GetPosition() + mPosition;
	}
}

void GeometryEngine::GeometryWorldItem::WorldItem::Move(const QVector3D & vector, bool updateChildren)
{
	mPosition += vector;

	UpdateModelMatrix(updateChildren);
}

void GeometryEngine::GeometryWorldItem::WorldItem::Rotate(const QVector3D & rot, bool updateChildren)
{
	QQuaternion tmpQuat = QQuaternion::fromEulerAngles(rot.x(), rot.y(), rot.z());
	mRotation *= tmpQuat;

	UpdateModelMatrix();

	if (updateChildren)
	{
		for (auto iter = mpChildren.begin(); iter != mpChildren.end(); ++iter)
		{
			(*iter)->Rotate(rot);
		}
	}
}

void GeometryEngine::GeometryWorldItem::WorldItem::Rotate(const QQuaternion & rot, bool updateChildren)
{
	mRotation *= rot;

	if (updateChildren)
	{
		for (auto iter = mpChildren.begin(); iter != mpChildren.end(); ++iter)
		{
			(*iter)->Rotate(rot);
		}
	}
}

void GeometryEngine::GeometryWorldItem::WorldItem::Scale(const QVector3D & scale, bool updateChildren)
{
	mScale *= scale;

	UpdateModelMatrix();

	if (updateChildren)
	{
		for (auto iter = mpChildren.begin(); iter != mpChildren.end(); ++iter)
		{
			(*iter)->Scale(scale);
		}
	}
}

void GeometryEngine::GeometryWorldItem::WorldItem::SetPosition(const QVector3D & pos, bool delayUpdate)
{
	mPosition.setX(pos.x());
	mPosition.setY(pos.y());
	mPosition.setZ(pos.z());

	if(!delayUpdate)
	{
		CalculateModelMatrix();
	}
}

void GeometryEngine::GeometryWorldItem::WorldItem::SetRotation(const QQuaternion & rot, bool delayUpdate)
{
	mRotation.setScalar(rot.scalar());
	mRotation.setVector(rot.vector());
	if (!delayUpdate)
	{
		CalculateModelMatrix();
	}
}

void GeometryEngine::GeometryWorldItem::WorldItem::SetRotation(const QVector3D & rot, bool delayUpdate)
{
	mRotation = mRotation.fromEulerAngles(rot);

	if (!delayUpdate)
	{
		CalculateModelMatrix();
	}
}

void GeometryEngine::GeometryWorldItem::WorldItem::SetScale(const QVector3D & scale, bool delayUpdate)
{
	mScale.setX(scale.x());
	mScale.setY(scale.y());
	mScale.setZ(scale.z());

	if (!delayUpdate)
	{
		CalculateModelMatrix();
	}
}

void GeometryEngine::GeometryWorldItem::WorldItem::init(const QVector3D & pos, const QVector3D & rot, const QVector3D & scale, WorldItem * parent)
{
	SetPosition(pos, true);
	SetRotation(rot, true);
	SetScale(scale, true);

	CalculateModelMatrix();

	SetParent(parent);
}

void GeometryEngine::GeometryWorldItem::WorldItem::CalculateModelMatrix(bool calculateChildren)
{
	UpdateModelMatrix();

	if (calculateChildren)
	{
		for (auto iter = mpChildren.begin(); iter != mpChildren.end(); ++iter)
		{
			(*iter)->CalculateModelMatrix(calculateChildren);
		}
	}
}

void GeometryEngine::GeometryWorldItem::WorldItem::UpdateModelMatrix(bool updateChildren)
{
	QVector3D tmp(GetPosition());

	mModelMatrix.setToIdentity();
	mModelMatrix.translate(tmp);
	mModelMatrix.rotate(mRotation);
	mModelMatrix.scale(mScale);

	if (updateChildren)
	{
		for (auto iter = mpChildren.begin(); iter != mpChildren.end(); ++iter)
		{
			(*iter)->UpdateModelMatrix(updateChildren);
		}
	}
}

QVector3D GeometryEngine::GeometryWorldItem::WorldItem::ToModelCoordSystem(const QVector3D & vector)
{
	return mRotation.inverted() * vector;
}

QVector3D GeometryEngine::GeometryWorldItem::WorldItem::ToGlobalCoordSystem(const QVector3D & vector)
{
	return mRotation * vector;
}

void GeometryEngine::GeometryWorldItem::WorldItem::Copy(const WorldItem & ref)
{
	this->mpParent = ref.mpParent;
	init(ref.mPosition, ref.mRotation.toEulerAngles(), ref.mScale, ref.mpParent);
}
