#pragma once

#ifndef POSTPROCESSTEST_H
#define POSTPROCESSTEST_H

#include "BaseGeometryTest.h"

namespace UnitTest
{
	/// Test for black and white and blur post process. Just a few geometries and no shadows
	class CPostProcessTest : public CBaseGeometryTest
	{
	public:
		/// Constructor
		CPostProcessTest() : mpSphere(nullptr), mpCube(nullptr), mpSkyboxCube(nullptr), mpFloorCube(nullptr), mpMainLight(nullptr), mpSecondLight(nullptr), mpCam(nullptr),
			CBaseGeometryTest() {};
		/// Copy constructor
		/// \param ref Object to be copied
		CPostProcessTest(const CPostProcessTest& ref) : mpSphere(nullptr), mpCube(nullptr), mpSkyboxCube(nullptr), mpFloorCube(nullptr), mpMainLight(nullptr), mpSecondLight(nullptr), mpCam(nullptr)
		{
			copy(ref);
		}

		/// Destructor
		virtual ~CPostProcessTest();

		/// Init graphic geometry
		/// \param engine Geometry engine to be tested
		/// \param sceneWidth viewport width
		/// \param sceneHeight viewport height
		virtual void InitGeometry(GeometryEngine::GeometryEngine* engine, int sceneWidth, int sceneHeight);
		/// Update graphic geometry
		/// \param tick Elapsed time since last update in milliseconds
		virtual void UpdateGeometry(qint64 tick);
		/// Factory method. Creates a copy of this object. The copy is not initialized.
		virtual CBaseGeometryTest* Clone() const override { return new CPostProcessTest(*this); };

	protected:

		static const float SECS;
		static const QVector3D ROTATION;
		static const QVector3D MOVEMENT;
		static const QVector3D POINT;
		static const QVector3D POS_LIGHT;

		GeometryEngine::GeometryWorldItem::GeometryItem::Sphere* mpSphere;
		GeometryEngine::GeometryWorldItem::GeometryItem::Cube* mpCube;
		GeometryEngine::GeometryWorldItem::GeometryItem::Cube* mpSkyboxCube;
		GeometryEngine::GeometryWorldItem::GeometryItem::Cube* mpFloorCube;
		GeometryEngine::GeometryWorldItem::GeometryLight::Light* mpMainLight;
		GeometryEngine::GeometryWorldItem::GeometryLight::Light* mpSecondLight;
		GeometryEngine::GeometryWorldItem::GeometryCamera::Camera* mpCam;

		/// Copies the state of other object into this. The copy is not initialized.
		virtual void copy(const CPostProcessTest& ref);
		/// Executes and evaluates the result of the test
		virtual void evaluate() override;
		/// Destroys the geometry objects of the scene
		virtual void destroyGeometry();
	};
}

#endif // !POSTPROCESSTEST_H
