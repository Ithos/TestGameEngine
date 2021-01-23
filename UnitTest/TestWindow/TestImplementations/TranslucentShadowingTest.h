#pragma once
#ifndef TRANSLUCENTSHADOWINGTEST_H
#define TRANSLUCENTSHADOWINGTEST_H

#include "BaseGeometryTest.h"

namespace GeometryEngine
{
	class GeometryEngine;
}

namespace UnitTest
{
	/// Test for a scene with dynamic translucent illumination and transparencies
	class CTranslucentShadowingTest : public CBaseGeometryTest
	{
	public:
		/// Constructor
		CTranslucentShadowingTest() : mpSphere(nullptr), mpCube(nullptr), mpQuad(nullptr), mpSkyboxCube(nullptr), mpFloorCube(nullptr), mpMainLight(nullptr), mpSecondLight(nullptr), mpCam(nullptr),
			CBaseGeometryTest() {};
		/// Copy constructor
		/// \param ref Object to be copied
		CTranslucentShadowingTest(const CTranslucentShadowingTest& ref) : mpSphere(nullptr), mpCube(nullptr), mpQuad(nullptr), mpSkyboxCube(nullptr), mpFloorCube(nullptr), mpMainLight(nullptr),
			mpSecondLight(nullptr), mpCam(nullptr)
		{
			copy(ref);
		}

		/// Destructor
		virtual ~CTranslucentShadowingTest();

		/// Init graphic geometry
		/// \param engine Geometry engine to be tested
		/// \param sceneWidth viewport width
		/// \param sceneHeight viewport height
		virtual void InitGeometry(GeometryEngine::GeometryEngine* engine, int sceneWidth, int sceneHeight);
		/// Update graphic geometry
		/// \param tick Elapsed time since last update in milliseconds
		virtual void UpdateGeometry(qint64 tick);
		/// Factory method. Creates a copy of this object. The copy is not initialized.
		virtual CBaseGeometryTest* Clone() const override { return new CTranslucentShadowingTest(*this); };

	protected:

		static const float SECS;
		static const QVector3D ROTATION;
		static const QVector3D MOVEMENT;
		static const QVector3D POINT;
		static const QVector3D POS_LIGHT;

		GeometryEngine::GeometryWorldItem::GeometryItem::Sphere* mpSphere;
		GeometryEngine::GeometryWorldItem::GeometryItem::Cube* mpCube;
		GeometryEngine::GeometryWorldItem::GeometryItem::Quad* mpQuad;
		GeometryEngine::GeometryWorldItem::GeometryItem::Cube* mpSkyboxCube;
		GeometryEngine::GeometryWorldItem::GeometryItem::Cube* mpFloorCube;
		GeometryEngine::GeometryWorldItem::GeometryLight::Light* mpMainLight;
		GeometryEngine::GeometryWorldItem::GeometryLight::Light* mpSecondLight;
		GeometryEngine::GeometryWorldItem::GeometryCamera::Camera* mpCam;

		/// Copies the state of other object into this. The copy is not initialized.
		virtual void copy(const CTranslucentShadowingTest& ref);
		/// Executes and evaluates the result of the test
		virtual void evaluate() override;
		/// Destroys the geometry objects of the scene
		virtual void destroyGeometry();
	};
}

#endif // !TRANSLUCENTSHADOWINGTEST_H
