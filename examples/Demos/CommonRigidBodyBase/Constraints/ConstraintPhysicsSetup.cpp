#include "ConstraintPhysicsSetup.h"
#include <CommonParameterInterface.h>
#include "CommonRigidBodyBase.h"

static btScalar val;
static btScalar targetVel = 0;
static btScalar maxImpulse = 10000;
static btHingeAccumulatedAngleConstraint* spDoorHinge = 0;
static btScalar actualHingeVelocity = 0.f;

static btVector3 btAxisA(0, 1, 0);

CameraResetInfo ConstraintPhysicsSetup::cameraResetInfo() const
{
	CameraResetInfo info;
	info.camDist = 7;
	info.pitch = -44;
	info.yaw = 721;
	info.camPosX = 8;
	info.camPosY = 1;
	info.camPosZ = -11;
	info.upAxis = 1;
	return info;
}

void ConstraintPhysicsSetup::initWorld(Physics* physics)
{
	auto m_dynamicsWorld = physics->getDynamicsWorld();

	int mode = btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawConstraints + btIDebugDraw::DBG_DrawConstraintLimits;
	m_dynamicsWorld->getDebugDrawer()->setDebugMode(mode);

	// {
	// 	SliderParams slider("target vel", &targetVel);
	// 	slider.m_minVal = -4;
	// 	slider.m_maxVal = 4;
	// 	m_guiHelper->getParameterInterface()->registerSliderFloatParameter(slider);
	// }

	// {
	// 	SliderParams slider("max impulse", &maxImpulse);
	// 	slider.m_minVal = 0;
	// 	slider.m_maxVal = 1000;
	// 	m_guiHelper->getParameterInterface()->registerSliderFloatParameter(slider);
	// }

	// {
	// 	SliderParams slider("actual vel", &actualHingeVelocity);
	// 	slider.m_minVal = -4;
	// 	slider.m_maxVal = 4;
	// 	m_guiHelper->getParameterInterface()->registerSliderFloatParameter(slider);
	// }

	// val = 1.f;
	// {
	// 	SliderParams slider("angle", &val);
	// 	slider.m_minVal = -720;
	// 	slider.m_maxVal = 720;
	// 	m_guiHelper->getParameterInterface()->registerSliderFloatParameter(slider);
	// }

	{  // create a door using hinge constraint attached to the world
		btCollisionShape* pDoorShape = new btBoxShape(btVector3(2.0f, 5.0f, 0.2f));
		physics->m_collisionShapes.push_back(pDoorShape);
		btTransform doorTrans;
		doorTrans.setIdentity();
		doorTrans.setOrigin(btVector3(-5.0f, -2.0f, 0.0f));
		btRigidBody* pDoorBody = physics->createRigidBody(1.0, doorTrans, pDoorShape);
		pDoorBody->setActivationState(DISABLE_DEACTIVATION);
		const btVector3 btPivotA(10.f + 2.1f, -2.0f, 0.0f);  // right next to the door slightly outside

		spDoorHinge = new btHingeAccumulatedAngleConstraint(*pDoorBody, btPivotA, btAxisA);

		m_dynamicsWorld->addConstraint(spDoorHinge);

		spDoorHinge->setDbgDrawSize(btScalar(5.f));
	}
}

void ConstraintPhysicsSetup::stepSimulation(float deltaTime)
{
	val = spDoorHinge->getAccumulatedHingeAngle() * SIMD_DEGS_PER_RAD;
	if (m_physics)
	{
		spDoorHinge->enableAngularMotor(true, targetVel, maxImpulse);

		m_physics->getDynamicsWorld()->stepSimulation(deltaTime, 10, 1. / 240.);

		btHingeConstraint* hinge = spDoorHinge;

		if (hinge)
		{
			const btRigidBody& bodyA = hinge->getRigidBodyA();
			const btRigidBody& bodyB = hinge->getRigidBodyB();

			btTransform trA = bodyA.getWorldTransform();
			btVector3 angVelA = bodyA.getAngularVelocity();
			btVector3 angVelB = bodyB.getAngularVelocity();

			{
				btVector3 ax1 = trA.getBasis() * hinge->getFrameOffsetA().getBasis().getColumn(2);
				btScalar vel = angVelA.dot(ax1);
				vel -= angVelB.dot(ax1);
				printf("hinge velocity (q) = %f\n", vel);
				actualHingeVelocity = vel;
			}
			btVector3 ortho0, ortho1;
			btPlaneSpace1(btAxisA, ortho0, ortho1);
			{
				btScalar vel2 = angVelA.dot(ortho0);
				vel2 -= angVelB.dot(ortho0);
				printf("hinge orthogonal1 velocity (q) = %f\n", vel2);
			}
			{
				btScalar vel0 = angVelA.dot(ortho1);
				vel0 -= angVelB.dot(ortho1);
				printf("hinge orthogonal0 velocity (q) = %f\n", vel0);
			}
		}
	}
}
