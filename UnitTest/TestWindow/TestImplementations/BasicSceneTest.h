#pragma once

#ifndef BASICSCENETEST_H
#define BASICSCENETEST_H

#include "BaseGeometryTest.h"

namespace UnitTest
{
	/// Test for basic representation, just a few geometries and no shadows
	class CBasicSceneTest : public CBaseGeometryTest
	{
	public:
		/// Constructor
		CBasicSceneTest() : mpSphere(nullptr), mpCube(nullptr), mpSkyboxCube(nullptr), mpFloorCube(nullptr), mpMainLight(nullptr), mpSecondLight(nullptr), mpCam(nullptr),
			CBaseGeometryTest()  {};
		/// Copy constructor
		/// \param ref Object to be copied
		CBasicSceneTest(const CBasicSceneTest& ref) : mpSphere(nullptr), mpCube(nullptr), mpSkyboxCube(nullptr), mpFloorCube(nullptr), mpMainLight(nullptr), mpSecondLight(nullptr), mpCam(nullptr)
		{ copy(ref); }

		/// Destructor
		virtual ~CBasicSceneTest();

		/// Init graphic geometry
		/// \param engine Geometry engine to be tested
		/// \param sceneWidth viewport width
		/// \param sceneHeight viewport height
		virtual void InitGeometry(GeometryEngine::GeometryEngine* engine, int sceneWidth, int sceneHeight);
		/// Update graphic geometry
		/// \param tick Elapsed time since last update in milliseconds
		virtual void UpdateGeometry(qint64 tick);
		/// Factory method. Creates a copy of this object. The copy is not initialized.
		virtual CBaseGeometryTest* Clone() const override { return new CBasicSceneTest(*this); } ;

	protected:

		static const float SECS;
		static const QVector3D ROTATION;
		static const QVector3D MOVEMENT;
		static const QVector3D POINT;
		static const QVector3D POS_LIGHT;

		GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem* mpSphere;
		GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem* mpCube;
		GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem* mpSkyboxCube;
		GeometryEngine::GeometryWorldItem::GeometryItem::GeometryItem* mpFloorCube;
		GeometryEngine::GeometryWorldItem::GeometryLight::Light* mpMainLight;
		GeometryEngine::GeometryWorldItem::GeometryLight::Light* mpSecondLight;
		GeometryEngine::GeometryWorldItem::GeometryCamera::Camera* mpCam;

		/// Copies the state of other object into this. The copy is not initialized.
		virtual void copy(const CBasicSceneTest& ref);
		/// Executes and evaluates the result of the test
		virtual void evaluate() override;
		/// Destroys the geometry objects of the scene
		virtual void destroyGeometry();

	};

}

#endif // !BASICSCENETEST_H
