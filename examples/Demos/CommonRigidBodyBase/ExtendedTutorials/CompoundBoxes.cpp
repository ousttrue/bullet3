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

#include "CompoundBoxes.h"

#include "CommonCameraInterface.h"
#include "btBulletDynamicsCommon.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btAlignedObjectArray.h"
#include <CommonRigidBodyBase.h>
#include <vcruntime_new.h>

struct CompoundBoxesExample : public CommonRigidBodyBase
{
	CompoundBoxesExample(struct GUIHelperInterface* helper)
		: CommonRigidBodyBase(helper)
	{
	}
	virtual ~CompoundBoxesExample() {}
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

void CompoundBoxesExample::initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper)
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
		btBoxShape* cube = m_physics->createBoxShape(btVector3(0.5, 0.5, 0.5));

		// create a new compound shape for making an L-beam from `cube`s
		btCompoundShape* compoundShape = new btCompoundShape();

		btTransform transform;

		// add cubes in an L-beam fashion to the compound shape
		transform.setIdentity();
		transform.setOrigin(btVector3(0, 0, 0));
		compoundShape->addChildShape(transform, cube);

		transform.setIdentity();
		transform.setOrigin(btVector3(0, -1, 0));
		compoundShape->addChildShape(transform, cube);

		transform.setIdentity();
		transform.setOrigin(btVector3(0, 0, 1));
		compoundShape->addChildShape(transform, cube);

		btScalar masses[3] = {1, 1, 1};
		btTransform principal;
		btVector3 inertia;
		compoundShape->calculatePrincipalAxisTransform(masses, principal, inertia);

		// new compund shape to store
		btCompoundShape* compound2 = new btCompoundShape();
		m_physics->m_collisionShapes.push_back(compound2);
#if 0
		// less efficient way to add the entire compund shape 
		// to a new compund shape as a child
		compound2->addChildShape(principal.inverse(), compoundShape);
#else
		// recompute the shift to make sure the compound shape is re-aligned
		for (int i = 0; i < compoundShape->getNumChildShapes(); i++)
			compound2->addChildShape(compoundShape->getChildTransform(i) * principal.inverse(),
									 compoundShape->getChildShape(i));
#endif
		delete compoundShape;

		transform.setIdentity();
		transform.setOrigin(btVector3(0, 10, 0));
		m_physics->createRigidBody(1.0, transform, compound2);
	}

	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}

CommonExampleInterface* ET_CompoundBoxesCreateFunc(CommonExampleOptions& options)
{
	return new CompoundBoxesExample(options.m_guiHelper);
}
