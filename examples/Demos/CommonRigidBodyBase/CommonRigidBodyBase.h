#pragma once
#include <CommonExampleInterface.h>
#include <btBulletDynamicsCommon.h>
#include <CommonGUIHelperInterface.h>
#include <CommonGraphicsAppInterface.h>
#include <functional>
#include "CommonCameraInterface.h"

using WorldFactory = std::function<btDiscreteDynamicsWorld*(
	btDispatcher* dispatcher,
	btBroadphaseInterface* pairCache,
	btConstraintSolver* constraintSolver,
	btCollisionConfiguration* collisionConfiguration)>;

// all supported solvers by bullet
enum class SolverEnumType
{
	SEQUENTIALIMPULSESOLVER = 0,
	GAUSSSEIDELSOLVER = 1,
	NNCGSOLVER = 2,
	DANZIGSOLVER = 3,
	LEMKESOLVER = 4,

	NUM_SOLVERS = 6
};

struct Physics
{
	//keep the collision shapes, for deletion/cleanup
	btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
	btBroadphaseInterface* m_broadphase = nullptr;
	btBroadphaseInterface* getBroadphase()
	{
		return m_broadphase;
	}
	btCollisionDispatcher* m_dispatcher = nullptr;
	btCollisionDispatcher* getDispatcher()
	{
		return m_dispatcher;
	}
	btConstraintSolver* m_solver = nullptr;
	btDefaultCollisionConfiguration* m_collisionConfiguration = nullptr;
	btDiscreteDynamicsWorld* m_dynamicsWorld = nullptr;
	btDiscreteDynamicsWorld* getDynamicsWorld()
	{
		return m_dynamicsWorld;
	}

	//data for picking objects
	class btRigidBody* m_pickedBody = nullptr;
	class btTypedConstraint* m_pickedConstraint = nullptr;
	int m_savedState = 0;
	btVector3 m_oldPickingPos;
	btVector3 m_hitPos;
	btScalar m_oldPickingDist = 0;

	Physics(
		btBroadphaseInterface* broadphase = nullptr,
		SolverEnumType solverType = SolverEnumType::SEQUENTIALIMPULSESOLVER,
		const WorldFactory& factory = {},
		bool useSoftbody = false);
	~Physics();
	void debugDraw(int debugDrawFlags);
	void removePickingConstraint();
	btBoxShape* createBoxShape(const btVector3& halfExtents);
	void deleteRigidBody(btRigidBody* body, struct GUIHelperInterface* helper);
	bool pickBody(const btVector3& rayFromWorld, const btVector3& rayToWorld);
	bool movePickedBody(const btVector3& rayFromWorld, const btVector3& rayToWorld);
	btRigidBody* createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape, const btVector4& color = btVector4(1, 0, 0, 1));
	void switchSolver(bool useMCLPSolver);
};

struct CommonRigidBodyBase : public CommonExampleInterface
{
	struct GUIHelperInterface* m_guiHelper = nullptr;
	Physics* m_physics = nullptr;

	CommonRigidBodyBase(struct GUIHelperInterface* helper);
	~CommonRigidBodyBase() override;
	void stepSimulation(float deltaTime) override;
	void physicsDebugDraw(int debugFlags) override;
	void exitPhysics() override;
	bool keyboardCallback(int key, int state) override;
	bool mouseMoveCallback(const CommonCameraInterface *camera, float x, float y) override;
	bool mouseButtonCallback(const CommonCameraInterface *camera, int button, int state, float x, float y, ButtonFlags flags) override;
	class btDiscreteDynamicsWorld* getDynamicsWorld() override { return m_physics ? m_physics->getDynamicsWorld() : nullptr; }
};
