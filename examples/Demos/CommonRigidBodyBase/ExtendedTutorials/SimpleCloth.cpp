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

#include "SimpleCloth.h"

#include "CommonCameraInterface.h"
#include "btBulletDynamicsCommon.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btAlignedObjectArray.h"
#include <CommonRigidBodyBase.h>

#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"

struct SimpleClothExample : public CommonRigidBodyBase
{
	SimpleClothExample(struct GUIHelperInterface* helper)
		: CommonRigidBodyBase(helper)
	{
	}
	virtual ~SimpleClothExample() {}
	void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper) override;
	virtual btSoftRigidDynamicsWorld* getSoftDynamicsWorld()
	{
		///just make it a btSoftRigidDynamicsWorld please
		///or we will add type checking
		return (btSoftRigidDynamicsWorld*)m_physics->getDynamicsWorld();
	}
	CameraResetInfo cameraResetInfo() const override
	{
		CameraResetInfo info;
		info.camDist = 41;
		info.pitch = -35;
		info.yaw = 52;
		info.camPosX = 0;
		info.camPosY = 0.46;
		info.camPosZ = 0;
		return info;
	}

	void createSoftBody(const btScalar size, const int num_x, const int num_z, const int fixed = 1 + 2);
	btSoftBodyWorldInfo softBodyWorldInfo;
};

void SimpleClothExample::initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper)
{
	m_guiHelper->setUpAxis(1);

	m_physics = new Physics;
	auto m_dynamicsWorld = m_physics->getDynamicsWorld();
	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

	softBodyWorldInfo.m_broadphase = m_physics->getBroadphase();
	softBodyWorldInfo.m_dispatcher = m_physics->getDispatcher();
	softBodyWorldInfo.m_gravity = m_dynamicsWorld->getGravity();
	softBodyWorldInfo.m_sparsesdf.Initialize();

	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);

	if (m_dynamicsWorld->getDebugDrawer())
		m_dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawContactPoints);

	///create a few basic rigid bodies
	btBoxShape* groundShape = m_physics->createBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
	m_physics->m_collisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -50, 0));
	{
		btScalar mass(0.);
		m_physics->createRigidBody(mass, groundTransform, groundShape, btVector4(0, 0, 1, 1));
	}

	{
		const btScalar s = 4;  //size of cloth patch
		const int NUM_X = 31;  //vertices on X axis
		const int NUM_Z = 31;  //vertices on Z axis
		createSoftBody(s, NUM_X, NUM_Z);
	}

	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}

void SimpleClothExample::createSoftBody(const btScalar s,
										const int numX,
										const int numY,
										const int fixed)
{
	btSoftBody* cloth = btSoftBodyHelpers::CreatePatch(softBodyWorldInfo,
													   btVector3(-s / 2, s + 1, 0),
													   btVector3(+s / 2, s + 1, 0),
													   btVector3(-s / 2, s + 1, +s),
													   btVector3(+s / 2, s + 1, +s),
													   numX, numY,
													   fixed, true);

	cloth->getCollisionShape()->setMargin(0.001f);
	cloth->getCollisionShape()->setUserPointer((void*)cloth);
	cloth->generateBendingConstraints(2, cloth->appendMaterial());
	cloth->setTotalMass(10);
	//cloth->m_cfg.citerations = 10;
	//	cloth->m_cfg.diterations = 10;
	cloth->m_cfg.piterations = 5;
	cloth->m_cfg.kDP = 0.005f;
	getSoftDynamicsWorld()->addSoftBody(cloth);
}

CommonExampleInterface* ET_SimpleClothCreateFunc(CommonExampleOptions& options)
{
	return new SimpleClothExample(options.m_guiHelper);
}
