/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2015 Erwin Coumans  http://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

///May 2015: implemented the wheels using the Hinge2Constraint
///todo: add controls for the motors etc.

#include "Hinge2Vehicle.h"

#include "CommonCameraInterface.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

#include "BulletDynamics/MLCPSolvers/btDantzigSolver.h"
#include "BulletDynamics/MLCPSolvers/btSolveProjectedGaussSeidel.h"
#include "BulletDynamics/MLCPSolvers/btMLCPSolver.h"

class btVehicleTuning;

class btCollisionShape;

#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"
#include "BulletDynamics/ConstraintSolver/btSliderConstraint.h"

#include <CommonExampleInterface.h>
#include "LinearMath/btAlignedObjectArray.h"
#include "btBulletCollisionCommon.h"
#include <CommonGUIHelperInterface.h>
#include <CommonRenderInterface.h>
#include <CommonGraphicsAppInterface.h>

#include <CommonRigidBodyBase.h>

class Hinge2Vehicle : public CommonRigidBodyBase
{
public:
	/* extra stuff*/
	btVector3 m_cameraPosition;

	btRigidBody* m_carChassis;
	btRigidBody* localCreateRigidBody(btScalar mass, const btTransform& worldTransform, btCollisionShape* colSape);

	GUIHelperInterface* m_guiHelper;
	int m_wheelInstances[4];

	bool m_useDefaultCamera;
	//----------------------------

	class btTriangleIndexVertexArray* m_indexVertexArrays;

	btVector3* m_vertices;

	btCollisionShape* m_wheelShape;

	float m_cameraHeight;

	float m_minCameraDistance;
	float m_maxCameraDistance;

	Hinge2Vehicle(struct GUIHelperInterface* helper);

	virtual ~Hinge2Vehicle();

	virtual void stepSimulation(float deltaTime);

	virtual void resetForklift();

	virtual void clientResetScene();

	virtual void displayCallback();

	virtual void specialKeyboard(int key, int x, int y);

	virtual void specialKeyboardUp(int key, int x, int y);

	virtual bool keyboardCallback(int key, int state);

	virtual void physicsDebugDraw(int debugFlags, struct GUIHelperInterface *m_guiHelper);

	void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper)override;
	void exitPhysics()override;

	CameraResetInfo cameraResetInfo() const override
	{
		CameraResetInfo info;
		info.camDist = 8;
		info.pitch = -32;
		info.yaw = -45;
		info.camPosX = 0;
		info.camPosY = 0;
		info.camPosZ = 2;
		info.upAxis = 1;
		return info;
	}

	/*static DemoApplication* Create()
	{
		Hinge2Vehicle* demo = new Hinge2Vehicle();
		demo->myinit();
		demo->initPhysics();
		return demo;
	}
	*/
};

static btScalar maxMotorImpulse = 4000.f;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#ifndef M_PI_4
#define M_PI_4 0.785398163397448309616
#endif

//static int rightIndex = 0;
//static int upIndex = 1;
//static int forwardIndex = 2;
static btVector3 wheelDirectionCS0(0, -1, 0);
static btVector3 wheelAxleCS(-1, 0, 0);

static bool useMCLPSolver = false;  //true;

#include <stdio.h>  //printf debugging

#include "Hinge2Vehicle.h"

//static const int maxProxies = 32766;
//static const int maxOverlap = 65535;

static float gEngineForce = 0.f;

static float defaultBreakingForce = 10.f;
static float gBreakingForce = 100.f;

static float maxEngineForce = 1000.f;  //this should be engine/velocity dependent
//static float	maxBreakingForce = 100.f;

static float gVehicleSteering = 0.f;
static float steeringIncrement = 0.04f;
static float steeringClamp = 0.3f;
static float wheelRadius = 0.5f;
static float wheelWidth = 0.4f;
//static float	wheelFriction = 1000;//BT_LARGE_FLOAT;
//static float	suspensionStiffness = 20.f;
//static float	suspensionDamping = 2.3f;
//static float	suspensionCompression = 4.4f;
//static float	rollInfluence = 0.1f;//1.0f;

//static btScalar suspensionRestLength(0.6);

#define CUBE_HALF_EXTENTS 1

////////////////////////////////////

Hinge2Vehicle::Hinge2Vehicle(struct GUIHelperInterface* helper)
	: CommonRigidBodyBase(helper),
	  m_carChassis(0),
	  m_guiHelper(helper),
	  m_indexVertexArrays(0),
	  m_vertices(0),
	  m_cameraHeight(4.f),
	  m_minCameraDistance(3.f),
	  m_maxCameraDistance(10.f)
{
	m_wheelShape = 0;
	m_cameraPosition = btVector3(30, 30, 30);
	m_useDefaultCamera = false;
}

void Hinge2Vehicle::exitPhysics()
{
	delete m_indexVertexArrays;
	delete m_vertices;

	delete m_wheelShape;
	delete m_physics;
}

Hinge2Vehicle::~Hinge2Vehicle()
{
	//exitPhysics();
}

void Hinge2Vehicle::initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper)
{
	btVector3 worldMin(-1000, -1000, -1000);
	btVector3 worldMax(1000, 1000, 1000);
	auto broadphase = new btAxisSweep3(worldMin, worldMax);

	m_physics = new Physics(broadphase, useMCLPSolver ? SolverEnumType::DANZIGSOLVER : SolverEnumType::SEQUENTIALIMPULSESOLVER);

	btCollisionShape* groundShape = new btBoxShape(btVector3(50, 3, 50));
	m_physics->m_collisionShapes.push_back(groundShape);

	auto m_dynamicsWorld = m_physics->getDynamicsWorld();

	m_dynamicsWorld->getSolverInfo().m_numIterations = 100;
	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);

	//m_dynamicsWorld->setGravity(btVector3(0,0,0));
	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(btVector3(0, -3, 0));

	//either use heightfield or triangle mesh

	//create ground object
	localCreateRigidBody(0, tr, groundShape);

	btCollisionShape* chassisShape = new btBoxShape(btVector3(1.f, 0.5f, 2.f));
	m_physics->m_collisionShapes.push_back(chassisShape);

	btCompoundShape* compound = new btCompoundShape();
	m_physics->m_collisionShapes.push_back(compound);
	btTransform localTrans;
	localTrans.setIdentity();
	//localTrans effectively shifts the center of mass with respect to the chassis
	localTrans.setOrigin(btVector3(0, 1, 0));

	compound->addChildShape(localTrans, chassisShape);

	{
		btCollisionShape* suppShape = new btBoxShape(btVector3(0.5f, 0.1f, 0.5f));
		btTransform suppLocalTrans;
		suppLocalTrans.setIdentity();
		//localTrans effectively shifts the center of mass with respect to the chassis
		suppLocalTrans.setOrigin(btVector3(0, 1.0, 2.5));
		compound->addChildShape(suppLocalTrans, suppShape);
	}

	const btScalar FALLHEIGHT = 5;
	tr.setOrigin(btVector3(0, FALLHEIGHT, 0));

	const btScalar chassisMass = 2.0f;
	const btScalar wheelMass = 1.0f;
	m_carChassis = localCreateRigidBody(chassisMass, tr, compound);  //chassisShape);
	//m_carChassis->setDamping(0.2,0.2);

	//m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth,wheelRadius,wheelRadius));
	m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth, wheelRadius, wheelRadius));

	btVector3 wheelPos[4] = {
		btVector3(btScalar(-1.), btScalar(FALLHEIGHT - 0.25), btScalar(1.25)),
		btVector3(btScalar(1.), btScalar(FALLHEIGHT - 0.25), btScalar(1.25)),
		btVector3(btScalar(1.), btScalar(FALLHEIGHT - 0.25), btScalar(-1.25)),
		btVector3(btScalar(-1.), btScalar(FALLHEIGHT - 0.25), btScalar(-1.25))};

	for (int i = 0; i < 4; i++)
	{
		// create a Hinge2 joint
		// create two rigid bodies
		// static bodyA (parent) on top:

		btRigidBody* pBodyA = this->m_carChassis;
		pBodyA->setActivationState(DISABLE_DEACTIVATION);
		// dynamic bodyB (child) below it :
		btTransform tr;
		tr.setIdentity();
		tr.setOrigin(wheelPos[i]);

		btRigidBody* pBodyB = m_physics->createRigidBody(wheelMass, tr, m_wheelShape);
		pBodyB->setFriction(1110);
		pBodyB->setActivationState(DISABLE_DEACTIVATION);
		// add some data to build constraint frames
		btVector3 parentAxis(0.f, 1.f, 0.f);
		btVector3 childAxis(1.f, 0.f, 0.f);
		btVector3 anchor = tr.getOrigin();
		btHinge2Constraint* pHinge2 = new btHinge2Constraint(*pBodyA, *pBodyB, anchor, parentAxis, childAxis);

		//m_guiHelper->get2dCanvasInterface();

		//pHinge2->setLowerLimit(-SIMD_HALF_PI * 0.5f);
		//pHinge2->setUpperLimit(SIMD_HALF_PI * 0.5f);

		// add constraint to world
		m_dynamicsWorld->addConstraint(pHinge2, true);

		// Drive engine.
		pHinge2->enableMotor(3, true);
		pHinge2->setMaxMotorForce(3, 1000);
		pHinge2->setTargetVelocity(3, 0);

		// Steering engine.
		pHinge2->enableMotor(5, true);
		pHinge2->setMaxMotorForce(5, 1000);
		pHinge2->setTargetVelocity(5, 0);

		pHinge2->setParam(BT_CONSTRAINT_CFM, 0.15f, 2);
		pHinge2->setParam(BT_CONSTRAINT_ERP, 0.35f, 2);

		pHinge2->setDamping(2, 2.0);
		pHinge2->setStiffness(2, 40.0);

		pHinge2->setDbgDrawSize(btScalar(5.f));
	}

	resetForklift();

	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}

void Hinge2Vehicle::physicsDebugDraw(int debugFlags, struct GUIHelperInterface *m_guiHelper)
{
	if (m_physics && m_physics->getDynamicsWorld()->getDebugDrawer())
	{
		auto m_dynamicsWorld = m_physics->getDynamicsWorld();
		m_dynamicsWorld->getDebugDrawer()->setDebugMode(debugFlags);
		m_dynamicsWorld->debugDrawWorld();
	}
}

void Hinge2Vehicle::stepSimulation(float deltaTime)
{
	float dt = deltaTime;

	if (m_physics)
	{
		auto m_dynamicsWorld = m_physics->getDynamicsWorld();

		//during idle mode, just run 1 simulation step maximum
		int maxSimSubSteps = 2;

		int numSimSteps;
		numSimSteps = m_dynamicsWorld->stepSimulation(dt, maxSimSubSteps);

		if (m_dynamicsWorld->getConstraintSolver()->getSolverType() == BT_MLCP_SOLVER)
		{
			btMLCPSolver* sol = (btMLCPSolver*)m_dynamicsWorld->getConstraintSolver();
			int numFallbacks = sol->getNumFallbacks();
			if (numFallbacks)
			{
				static int totalFailures = 0;
				totalFailures += numFallbacks;
				printf("MLCP solver failed %d times, falling back to btSequentialImpulseSolver (SI)\n", totalFailures);
			}
			sol->setNumFallbacks(0);
		}

//#define VERBOSE_FEEDBACK
#ifdef VERBOSE_FEEDBACK
		if (!numSimSteps)
			printf("Interpolated transforms\n");
		else
		{
			if (numSimSteps > maxSimSubSteps)
			{
				//detect dropping frames
				printf("Dropped (%i) simulation steps out of %i\n", numSimSteps - maxSimSubSteps, numSimSteps);
			}
			else
			{
				printf("Simulated (%i) steps\n", numSimSteps);
			}
		}
#endif  //VERBOSE_FEEDBACK
	}
}

void Hinge2Vehicle::displayCallback(void)
{
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//renderme();

	//optional but useful: debug drawing
	if (m_physics)
		m_physics->getDynamicsWorld()->debugDrawWorld();

	//	glFlush();
	//	glutSwapBuffers();
}

void Hinge2Vehicle::clientResetScene()
{
	exitPhysics();
	initPhysics({}, {});
}

void Hinge2Vehicle::resetForklift()
{
	gVehicleSteering = 0.f;
	gBreakingForce = defaultBreakingForce;
	gEngineForce = 0.f;

	m_carChassis->setCenterOfMassTransform(btTransform::getIdentity());
	m_carChassis->setLinearVelocity(btVector3(0, 0, 0));
	m_carChassis->setAngularVelocity(btVector3(0, 0, 0));
	m_physics->getDynamicsWorld()->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_carChassis->getBroadphaseHandle(), m_physics->getDynamicsWorld()->getDispatcher());
}

bool Hinge2Vehicle::keyboardCallback(int key, int state)
{
	bool handled = false;
	bool isShiftPressed = false; // m_guiHelper->getAppInterface()->m_window->isModifierKeyPressed(B3G_SHIFT);

	if (state)
	{
		if (isShiftPressed)
		{
		}
		else
		{
			switch (key)
			{
				case B3G_LEFT_ARROW:
				{
					handled = true;
					gVehicleSteering += steeringIncrement;
					if (gVehicleSteering > steeringClamp)
						gVehicleSteering = steeringClamp;

					break;
				}
				case B3G_RIGHT_ARROW:
				{
					handled = true;
					gVehicleSteering -= steeringIncrement;
					if (gVehicleSteering < -steeringClamp)
						gVehicleSteering = -steeringClamp;

					break;
				}
				case B3G_UP_ARROW:
				{
					handled = true;
					gEngineForce = maxEngineForce;
					gBreakingForce = 0.f;
					break;
				}
				case B3G_DOWN_ARROW:
				{
					handled = true;
					gEngineForce = -maxEngineForce;
					gBreakingForce = 0.f;
					break;
				}

				case B3G_F7:
				{
					handled = true;
					btDiscreteDynamicsWorld* world = (btDiscreteDynamicsWorld*)m_physics->getDynamicsWorld();
					world->setLatencyMotionStateInterpolation(!world->getLatencyMotionStateInterpolation());
					printf("world latencyMotionStateInterpolation = %d\n", world->getLatencyMotionStateInterpolation());
					break;
				}
				case B3G_F6:
				{
					handled = true;
					//switch solver (needs demo restart)
					useMCLPSolver = !useMCLPSolver;
					printf("switching to useMLCPSolver = %d\n", useMCLPSolver);

					m_physics->switchSolver(useMCLPSolver);

					//exitPhysics();
					//initPhysics();
					break;
				}

				case B3G_F5:
					handled = true;
					m_useDefaultCamera = !m_useDefaultCamera;
					break;
				default:
					break;
			}
		}
	}
	else
	{
	}
	return handled;
}

void Hinge2Vehicle::specialKeyboardUp(int key, int x, int y)
{
}

void Hinge2Vehicle::specialKeyboard(int key, int x, int y)
{
}

btRigidBody* Hinge2Vehicle::localCreateRigidBody(btScalar mass, const btTransform& startTransform, btCollisionShape* shape)
{
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

#define USE_MOTIONSTATE 1
#ifdef USE_MOTIONSTATE
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(cInfo);
	//body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

#else
	btRigidBody* body = new btRigidBody(mass, 0, shape, localInertia);
	body->setWorldTransform(startTransform);
#endif  //

	m_physics->getDynamicsWorld()->addRigidBody(body);
	return body;
}

CommonExampleInterface* Hinge2VehicleCreateFunc(struct CommonExampleOptions& options)
{
	return new Hinge2Vehicle(options.m_guiHelper);
}
