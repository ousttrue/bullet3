#include "CommonRigidBodyBase.h"
#include <CommonCallbacks.h>
#include <CommonRenderInterface.h>
#include <CommonCameraInterface.h>
#include <BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "CommonExampleInterface.h"
#include "Evolution/NN3DWalkersTimeWarpBase.h"

Physics::Physics(
	btBroadphaseInterface* broadphase,
	SolverEnumType solverType,
	const WorldFactory& factory,
	bool useSoftbody)
{
	///collision configuration contains default setup for memory, collision setup
	m_collisionConfiguration = useSoftbody
								   ? new btSoftBodyRigidBodyCollisionConfiguration()
								   : new btDefaultCollisionConfiguration();
	//m_collisionConfiguration->setConvexConvexMultipointIterations();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	m_broadphase = broadphase ? broadphase : new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	// m_solver = solver ? solver : new btSequentialImpulseConstraintSolver();
	// different solvers require different settings
	switch (solverType)
	{
		case SolverEnumType::SEQUENTIALIMPULSESOLVER:
		{
			//			b3Printf("=%s=",SolverType::SEQUENTIALIMPULSESOLVER);
			m_solver = new btSequentialImpulseConstraintSolver();
			break;
		}
		case SolverEnumType::NNCGSOLVER:
		{
			//			b3Printf("=%s=",SolverType::NNCGSOLVER);
			m_solver = new btNNCGConstraintSolver();
			break;
		}
		case SolverEnumType::DANZIGSOLVER:
		{
			//			b3Printf("=%s=",SolverType::DANZIGSOLVER);
			btDantzigSolver* mlcp = new btDantzigSolver();
			m_solver = new btMLCPSolver(mlcp);
			break;
		}
		case SolverEnumType::GAUSSSEIDELSOLVER:
		{
			//			b3Printf("=%s=",SolverType::GAUSSSEIDELSOLVER);
			btSolveProjectedGaussSeidel* mlcp = new btSolveProjectedGaussSeidel();
			m_solver = new btMLCPSolver(mlcp);
			break;
		}
		case SolverEnumType::LEMKESOLVER:
		{
			//			b3Printf("=%s=",SolverType::LEMKESOLVER);
			btLemkeSolver* mlcp = new btLemkeSolver();
			m_solver = new btMLCPSolver(mlcp);
			break;
		}

		default:
			break;
	}

	m_dynamicsWorld = factory
						  ? factory(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration)
						  : new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

	if (SOLVER_TYPE == SolverEnumType::DANZIGSOLVER || SOLVER_TYPE == SolverEnumType::GAUSSSEIDELSOLVER)
	{
		m_dynamicsWorld->getSolverInfo().m_minimumSolverBatchSize = 1;  //for mlcp solver it is better to have a small A matrix
	}
	else
	{
		m_dynamicsWorld->getSolverInfo().m_minimumSolverBatchSize = 128;  //for direct solver, it is better to solve multiple objects together, small batches have high overhead
	}

	m_dynamicsWorld->getDispatchInfo().m_useContinuous = true;  // set continuous collision

	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

Physics::~Physics()
{
	removePickingConstraint();

	//cleanup in the reverse order of creation/initialization
	if (m_dynamicsWorld)
	{
		for (int i = m_dynamicsWorld->getNumConstraints() - 1; i >= 0; i--)
		{
			auto constraint = m_dynamicsWorld->getConstraint(i);
			m_dynamicsWorld->removeConstraint(constraint);
			delete constraint;
		}
		//remove the rigidbodies from the dynamics world and delete them
		for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				while (body->getNumConstraintRefs())
				{
					btTypedConstraint* constraint = body->getConstraintRef(0);
					m_dynamicsWorld->removeConstraint(constraint);
					delete constraint;
				}
				delete body->getMotionState();
				m_dynamicsWorld->removeRigidBody(body);
			}
			m_dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}
	}

	//delete collision shapes
	for (int j = 0; j < m_collisionShapes.size(); j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}
	m_collisionShapes.clear();

	delete m_dynamicsWorld;
	m_dynamicsWorld = 0;

	delete m_solver;
	m_solver = 0;

	delete m_broadphase;
	m_broadphase = 0;

	delete m_dispatcher;
	m_dispatcher = 0;

	delete m_collisionConfiguration;
	m_collisionConfiguration = 0;
}

void Physics::debugDraw(int debugDrawFlags)
{
	if (m_dynamicsWorld)
	{
		if (m_dynamicsWorld->getDebugDrawer())
		{
			m_dynamicsWorld->getDebugDrawer()->setDebugMode(debugDrawFlags);
		}
		m_dynamicsWorld->debugDrawWorld();
	}
}

bool Physics::pickBody(const btVector3& rayFromWorld, const btVector3& rayToWorld)
{
	if (m_dynamicsWorld == 0)
		return false;

	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFromWorld, rayToWorld);

	rayCallback.m_flags |= btTriangleRaycastCallback::kF_UseGjkConvexCastRaytest;
	m_dynamicsWorld->rayTest(rayFromWorld, rayToWorld, rayCallback);
	if (rayCallback.hasHit())
	{
		btVector3 pickPos = rayCallback.m_hitPointWorld;
		btRigidBody* body = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
		if (body)
		{
			//other exclusions?
			if (!(body->isStaticObject() || body->isKinematicObject()))
			{
				m_pickedBody = body;
				m_savedState = m_pickedBody->getActivationState();
				m_pickedBody->setActivationState(DISABLE_DEACTIVATION);
				//printf("pickPos=%f,%f,%f\n",pickPos.getX(),pickPos.getY(),pickPos.getZ());
				btVector3 localPivot = body->getCenterOfMassTransform().inverse() * pickPos;
				btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*body, localPivot);
				m_dynamicsWorld->addConstraint(p2p, true);
				m_pickedConstraint = p2p;
				btScalar mousePickClamping = 30.f;
				p2p->m_setting.m_impulseClamp = mousePickClamping;
				//very weak constraint for picking
				p2p->m_setting.m_tau = 0.001f;
			}
		}

		//					pickObject(pickPos, rayCallback.m_collisionObject);
		m_oldPickingPos = rayToWorld;
		m_hitPos = pickPos;
		m_oldPickingDist = (pickPos - rayFromWorld).length();
		//					printf("hit !\n");
		//add p2p
	}
	return false;
}

void Physics::removePickingConstraint()
{
	if (m_pickedConstraint)
	{
		m_pickedBody->forceActivationState(m_savedState);
		m_pickedBody->activate();
		m_dynamicsWorld->removeConstraint(m_pickedConstraint);
		delete m_pickedConstraint;
		m_pickedConstraint = 0;
		m_pickedBody = 0;
	}
}

bool Physics::movePickedBody(const btVector3& rayFromWorld, const btVector3& rayToWorld)
{
	if (m_pickedBody && m_pickedConstraint)
	{
		btPoint2PointConstraint* pickCon = static_cast<btPoint2PointConstraint*>(m_pickedConstraint);
		if (pickCon)
		{
			//keep it at the same picking distance

			btVector3 newPivotB;

			btVector3 dir = rayToWorld - rayFromWorld;
			dir.normalize();
			dir *= m_oldPickingDist;

			newPivotB = rayFromWorld + dir;
			pickCon->setPivotB(newPivotB);
			return true;
		}
	}
	return false;
}

btBoxShape* Physics::createBoxShape(const btVector3& halfExtents)
{
	btBoxShape* box = new btBoxShape(halfExtents);
	m_collisionShapes.push_back(box);
	return box;
}

void Physics::deleteRigidBody(btRigidBody* body, struct GUIHelperInterface* guiHelper)
{
	int graphicsUid = body->getUserIndex();
	guiHelper->removeGraphicsInstance(graphicsUid);
	m_dynamicsWorld->removeRigidBody(body);
	btMotionState* ms = body->getMotionState();
	delete body;
	delete ms;
}

btRigidBody* Physics::createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape, const btVector4& color)
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

	body->setUserIndex(-1);
	m_dynamicsWorld->addRigidBody(body);
	return body;
}

void Physics::switchSolver(bool useMCLPSolver)
{
	delete m_solver;
	if (useMCLPSolver)
	{
		btDantzigSolver* mlcp = new btDantzigSolver();
		//btSolveProjectedGaussSeidel* mlcp = new btSolveProjectedGaussSeidel;
		btMLCPSolver* sol = new btMLCPSolver(mlcp);
		m_solver = sol;
	}
	else
	{
		m_solver = new btSequentialImpulseConstraintSolver();
	}

	m_dynamicsWorld->setConstraintSolver(m_solver);
}

///
/// CommonRigidBodyBase
///
CommonRigidBodyBase::CommonRigidBodyBase(GUIHelperInterface* helper)
{
}

CommonRigidBodyBase::~CommonRigidBodyBase()
{
	if (m_physics)
	{
		delete m_physics;
	}
}

Physics* CommonRigidBodyBase::createPhysics()
{
	return new Physics;
}

void CommonRigidBodyBase::initPhysics(CommonCameraInterface* camera, struct GUIHelperInterface* m_guiHelper)
{
	m_physics = createPhysics();
	initWorld(m_physics);
}

void CommonRigidBodyBase::stepSimulation(float deltaTime)
{
	if (m_physics)
	{
		m_physics->getDynamicsWorld()->stepSimulation(deltaTime);
	}
}

void CommonRigidBodyBase::physicsDebugDraw(int debugFlags)
{
	if (m_physics)
	{
		if (auto drawer = m_physics->getDynamicsWorld()->getDebugDrawer())
		{
			drawer->setDebugMode(debugFlags);
		}
	}
}

bool CommonRigidBodyBase::keyboardCallback(int key, int state)
{
	if ((key == B3G_F3) && state && m_physics)
	{
		btDefaultSerializer* serializer = new btDefaultSerializer();
		m_physics->getDynamicsWorld()->serialize(serializer);

		FILE* file = fopen("testFile.bullet", "wb");
		fwrite(serializer->getBufferPointer(), serializer->getCurrentBufferSize(), 1, file);
		fclose(file);
		//b3Printf("btDefaultSerializer wrote testFile.bullet");
		delete serializer;
		return true;
	}
	return false;  //don't handle this key
}

bool CommonRigidBodyBase::mouseMoveCallback(const CommonCameraInterface* camera, float x, float y)
{
	if (!camera)
	{
		btAssert(0);
		return false;
	}

	btVector3 rayTo = camera->getRayTo(int(x), int(y));
	btVector3 rayFrom;
	camera->getCameraPosition(rayFrom);

	if (m_physics)
	{
		m_physics->movePickedBody(rayFrom, rayTo);
	}

	return false;
}

bool CommonRigidBodyBase::mouseButtonCallback(const CommonCameraInterface* camera, int button, int state, float x, float y, ButtonFlags flags)
{
	if (!camera)
	{
		btAssert(0);
		return false;
	}

	if (state == 1)
	{
		if (button == 0)
		{
			if (!(flags & ButtonFlagsAlt) && !(flags & ButtonFlagsCtrl))
			{
				btVector3 camPos;
				camera->getCameraPosition(camPos);

				btVector3 rayFrom = camPos;
				btVector3 rayTo = camera->getRayTo(int(x), int(y));

				if (m_physics)
				{
					m_physics->pickBody(rayFrom, rayTo);
				}
			}
		}
	}
	else
	{
		if (button == 0)
		{
			if (m_physics)
			{
				m_physics->removePickingConstraint();
			}
		}
	}

	return false;
}
