/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  https://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "RaytestDemo.h"
///btBulletDynamicsCommon.h is the main Bullet include file, contains most common include files.
#include "CommonCameraInterface.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"

#include <stdio.h>  //printf debugging

#include "LinearMath/btAlignedObjectArray.h"

///RaytestDemo shows how to use the btCollisionWorld::rayTest feature

#include <CommonRigidBodyBase.h>

class RaytestDemo : public CommonRigidBodyBase
{
public:
	RaytestDemo(struct GUIHelperInterface* helper)
		: CommonRigidBodyBase(helper)
	{
	}
	virtual ~RaytestDemo()
	{
	}
	void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper) override;

	virtual void exitPhysics();

	void castRays();

	virtual void stepSimulation(float deltaTime);

	CameraResetInfo cameraResetInfo() const override
	{
		CameraResetInfo info;
		info.camDist = 18;
		info.pitch = -30;
		info.yaw = 129;
		info.camPosX = -4.6;
		info.camPosY = -4.7;
		info.camPosZ = -5.75;
		info.upAxis = 1;
		return info;
	}
};

void RaytestDemo::castRays()
{
	auto m_dynamicsWorld = m_physics->getDynamicsWorld();

	static float up = 0.f;
	static float dir = 1.f;
	//add some simple animation
	//if (!m_idle)
	{
		up += 0.01 * dir;

		if (btFabs(up) > 2)
		{
			dir *= -1.f;
		}

		btTransform tr = m_dynamicsWorld->getCollisionObjectArray()[1]->getWorldTransform();
		static float angle = 0.f;
		angle += 0.01f;
		tr.setRotation(btQuaternion(btVector3(0, 1, 0), angle));
		m_dynamicsWorld->getCollisionObjectArray()[1]->setWorldTransform(tr);
	}

	///step the simulation
	if (m_dynamicsWorld)
	{
		m_dynamicsWorld->updateAabbs();
		m_dynamicsWorld->computeOverlappingPairs();

		btVector3 red(1, 0, 0);
		btVector3 blue(0, 0, 1);

		///all hits
		{
			btVector3 from(-30, 1 + up, 0);
			btVector3 to(30, 1, 0);
			m_dynamicsWorld->getDebugDrawer()->drawLine(from, to, btVector4(0, 0, 0, 1));
			btCollisionWorld::AllHitsRayResultCallback allResults(from, to);
			allResults.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
			//kF_UseGjkConvexRaytest flag is now enabled by default, use the faster but more approximate algorithm
			//allResults.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
			allResults.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;

			m_dynamicsWorld->rayTest(from, to, allResults);

			for (int i = 0; i < allResults.m_hitFractions.size(); i++)
			{
				btVector3 p = from.lerp(to, allResults.m_hitFractions[i]);
				m_dynamicsWorld->getDebugDrawer()->drawSphere(p, 0.1, red);
				m_dynamicsWorld->getDebugDrawer()->drawLine(p, p + allResults.m_hitNormalWorld[i], red);
			}
		}

		///first hit
		{
			btVector3 from(-30, 1.2, 0);
			btVector3 to(30, 1.2, 0);
			m_dynamicsWorld->getDebugDrawer()->drawLine(from, to, btVector4(0, 0, 1, 1));

			btCollisionWorld::ClosestRayResultCallback closestResults(from, to);
			closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

			m_dynamicsWorld->rayTest(from, to, closestResults);

			if (closestResults.hasHit())
			{
				btVector3 p = from.lerp(to, closestResults.m_closestHitFraction);
				m_dynamicsWorld->getDebugDrawer()->drawSphere(p, 0.1, blue);
				m_dynamicsWorld->getDebugDrawer()->drawLine(p, p + closestResults.m_hitNormalWorld, blue);
			}
		}
	}
}

void RaytestDemo::stepSimulation(float deltaTime)
{
	castRays();
	CommonRigidBodyBase::stepSimulation(deltaTime);
}

void RaytestDemo::initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper)
{
	m_physics = new Physics;
	auto m_dynamicsWorld = m_physics->getDynamicsWorld();

	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);

	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

	///create a few basic rigid bodies
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
	//	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),50);

	m_physics->m_collisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -50, 0));

	//We can also use DemoApplication::localCreateRigidBody, but for clarity it is provided here:
	{
		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		body->setFriction(1);
		//add the body to the dynamics world
		m_dynamicsWorld->addRigidBody(body);
	}

	{
		btVector3 convexPoints[] = {btVector3(-1, -1, -1), btVector3(-1, -1, 1), btVector3(-1, 1, 1), btVector3(-1, 1, -1),
									btVector3(2, 0, 0)};

		btVector3 quad[] = {
			btVector3(0, 1, -1),
			btVector3(0, 1, 1),
			btVector3(0, -1, 1),
			btVector3(0, -1, -1)};

		btTriangleMesh* mesh = new btTriangleMesh();
		mesh->addTriangle(quad[0], quad[1], quad[2], true);
		mesh->addTriangle(quad[0], quad[2], quad[3], true);

		btBvhTriangleMeshShape* trimesh = new btBvhTriangleMeshShape(mesh, true, true);
		//btGImpactMeshShape * trimesh = new btGImpactMeshShape(mesh);
		//trimesh->updateBound();

#define NUM_SHAPES 6
		btCollisionShape* colShapes[NUM_SHAPES] = {
			trimesh,
			new btConvexHullShape(&convexPoints[0].getX(), sizeof(convexPoints) / sizeof(btVector3), sizeof(btVector3)),
			new btSphereShape(1),
			new btCapsuleShape(0.2, 1),
			new btCylinderShape(btVector3(0.2, 1, 0.2)),
			new btBoxShape(btVector3(1, 1, 1))};

		for (int i = 0; i < NUM_SHAPES; i++)
			m_physics->m_collisionShapes.push_back(colShapes[i]);

		for (int i = 0; i < 6; i++)
		{
			//create a few dynamic rigidbodies
			// Re-using the same collision is better for memory usage and performance

			/// Create Dynamic Objects
			btTransform startTransform;
			startTransform.setIdentity();
			startTransform.setOrigin(btVector3((i - 3) * 5, 1, 0));

			btScalar mass(1.f);

			if (!i)
				mass = 0.f;

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);

			btVector3 localInertia(0, 0, 0);
			btCollisionShape* colShape = colShapes[i % NUM_SHAPES];
			if (isDynamic)
				colShape->calculateLocalInertia(mass, localInertia);

			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, 0, colShape, localInertia);
			rbInfo.m_startWorldTransform = startTransform;
			btRigidBody* body = new btRigidBody(rbInfo);
			body->setRollingFriction(0.03);
			body->setSpinningFriction(0.03);
			body->setFriction(1);
			body->setAnisotropicFriction(colShape->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

			m_dynamicsWorld->addRigidBody(body);
		}
	}

	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}

void RaytestDemo::exitPhysics()
{
	delete m_physics;
}

class CommonExampleInterface* RaytestCreateFunc(struct CommonExampleOptions& options)
{
	return new RaytestDemo(options.m_guiHelper);
}
