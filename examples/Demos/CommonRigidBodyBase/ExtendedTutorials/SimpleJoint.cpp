/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2015 Google Inc. http://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "SimpleJoint.h"

#include "CommonCameraInterface.h"
#include "btBulletDynamicsCommon.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btAlignedObjectArray.h"
#include <CommonRigidBodyBase.h>

struct SimpleJointExample : public CommonRigidBodyBase
{
	SimpleJointExample(struct GUIHelperInterface* helper)
		: CommonRigidBodyBase(helper)
	{
	}
	virtual ~SimpleJointExample() {}
	void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper) override;
	CameraResetInfo cameraResetInfo() const override
	{
		CameraResetInfo info;
		info.camDist = 41;
		info.pitch = -35;
		info.yaw = 52;
		info.camPosX = 0;
		info.camPosY = 0.46;
		info.camPosZ = 0;
		info.upAxis = 1;
		return info;
	}
};

void SimpleJointExample::initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper)
{
	m_physics = new Physics;
	auto m_dynamicsWorld = m_physics->getDynamicsWorld();

	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);

	if (m_dynamicsWorld->getDebugDrawer())
		m_dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawContactPoints);

	///create a few basic rigid bodies
	btBoxShape* groundShape = m_physics->createBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -50, 0));
	{
		btScalar mass(0.);
		m_physics->createRigidBody(mass, groundTransform, groundShape, btVector4(0, 0, 1, 1));
	}

	{
		//create a few dynamic rigidbodies
		// Re-using the same collision is better for memory usage and performance
		btBoxShape* colShape = m_physics->createBoxShape(btVector3(1, 1, 1));

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(
			btScalar(0),
			btScalar(10),
			btScalar(0)));
		btRigidBody* dynamicBox = m_physics->createRigidBody(mass, startTransform, colShape);

		//create a static rigid body
		mass = 0;
		startTransform.setOrigin(btVector3(
			btScalar(0),
			btScalar(20),
			btScalar(0)));

		btRigidBody* staticBox = m_physics->createRigidBody(mass, startTransform, colShape);

		//create a simple p2pjoint constraint
		btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*dynamicBox, *staticBox, btVector3(0, 3, 0), btVector3(0, 0, 0));
		p2p->m_setting.m_damping = 0.0625;
		p2p->m_setting.m_impulseClamp = 0.95;
		m_dynamicsWorld->addConstraint(p2p);
	}

	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}

CommonExampleInterface* ET_SimpleJointCreateFunc(CommonExampleOptions& options)
{
	return new SimpleJointExample(options.m_guiHelper);
}
