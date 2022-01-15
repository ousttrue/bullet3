#pragma once

#include <btBulletDynamicsCommon.h>
#include "CommonExampleInterface.h"
#include <CommonGUIHelperInterface.h>
#include <CommonGraphicsAppInterface.h>

struct CommonRigidBodyBase : public CommonExampleInterface
{
	//keep the collision shapes, for deletion/cleanup
	btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
	btBroadphaseInterface* m_broadphase;
	btCollisionDispatcher* m_dispatcher;
	btConstraintSolver* m_solver;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btDiscreteDynamicsWorld* m_dynamicsWorld;

	//data for picking objects
	class btRigidBody* m_pickedBody;
	class btTypedConstraint* m_pickedConstraint;
	int m_savedState;
	btVector3 m_oldPickingPos;
	btVector3 m_hitPos;
	btScalar m_oldPickingDist;
	struct GUIHelperInterface* m_guiHelper;

	CommonRigidBodyBase(struct GUIHelperInterface* helper);
	virtual ~CommonRigidBodyBase();
	btDiscreteDynamicsWorld* getDynamicsWorld();
	virtual void createEmptyDynamicsWorld();
	virtual void stepSimulation(float deltaTime);
	virtual void physicsDebugDraw(int debugFlags);
	virtual void exitPhysics();
	virtual void debugDraw(int debugDrawFlags);
	virtual bool keyboardCallback(int key, int state);
	btVector3 getRayTo(int x, int y);
	virtual bool mouseMoveCallback(float x, float y);
	virtual bool mouseButtonCallback(int button, int state, float x, float y);
	virtual bool pickBody(const btVector3& rayFromWorld, const btVector3& rayToWorld);
	virtual bool movePickedBody(const btVector3& rayFromWorld, const btVector3& rayToWorld);
	virtual void removePickingConstraint();
	btBoxShape* createBoxShape(const btVector3& halfExtents);
	void deleteRigidBody(btRigidBody* body);
	btRigidBody* createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape, const btVector4& color = btVector4(1, 0, 0, 1));
	virtual void renderScene();
};
