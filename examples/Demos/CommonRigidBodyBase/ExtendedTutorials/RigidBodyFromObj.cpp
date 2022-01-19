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

#include "RigidBodyFromObj.h"
#include <LoadMeshFromObj.h>
#include <GLInstanceGraphicsShape.h>
#include <CommonRigidBodyBase.h>
#include <b3ResourcePath.h>
#include <b3BulletDefaultFileIO.h>
#include <Bullet3Common/b3FileUtils.h>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btAlignedObjectArray.h>
#include "CommonCameraInterface.h"

struct RigidBodyFromObjExample : public CommonRigidBodyBase
{
	int m_options;

	RigidBodyFromObjExample(struct GUIHelperInterface* helper, int options)
		: CommonRigidBodyBase(helper),
		  m_options(options)
	{
	}
	virtual ~RigidBodyFromObjExample() {}
	void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper) override;
	CameraResetInfo cameraResetInfo() const override
	{
		CameraResetInfo info;
		info.camDist = 11;
		info.pitch = -35;
		info.yaw = 52;
		info.camPosX = 0;
		info.camPosY = 0.46;
		info.camPosZ = 0;
		info.upAxis = 1;
		return info;
	}
};

void RigidBodyFromObjExample::initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper)
{
	m_physics = new Physics;
	auto m_dynamicsWorld = m_physics->getDynamicsWorld();

	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);

	//if (m_dynamicsWorld->getDebugDrawer())
	//	m_dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe+btIDebugDraw::DBG_DrawContactPoints);

	///create a few basic rigid bodies
	btBoxShape* groundShape = m_physics->createBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -50, 0));
	{
		btScalar mass(0.);
		m_physics->createRigidBody(mass, groundTransform, groundShape, btVector4(0, 0, 1, 1));
	}

	//load our obj mesh
	const char* fileName = "teddy.obj";  //sphere8.obj";//sponza_closed.obj";//sphere8.obj";
	char relativeFileName[1024];
	if (b3ResourcePath::findResourcePath(fileName, relativeFileName, 1024, 0))
	{
		char pathPrefix[1024];
		b3FileUtils::extractPath(relativeFileName, pathPrefix, 1024);
	}

	b3BulletDefaultFileIO fileIO;
	GLInstanceGraphicsShape* glmesh = LoadMeshFromObj(relativeFileName, "", &fileIO);
	printf("[INFO] Obj loaded: Extracted %d verticed from obj file [%s]\n", glmesh->m_numvertices, fileName);

	const GLInstanceVertex& v = glmesh->m_vertices->at(0);
	btConvexHullShape* shape = new btConvexHullShape((const btScalar*)(&(v.xyzw[0])), glmesh->m_numvertices, sizeof(GLInstanceVertex));

	float scaling[4] = {0.1, 0.1, 0.1, 1};

	btVector3 localScaling(scaling[0], scaling[1], scaling[2]);
	shape->setLocalScaling(localScaling);

	if (m_options & OptimizeConvexObj)
	{
		shape->optimizeConvexHull();
	}

	if (m_options & ComputePolyhedralFeatures)
	{
		shape->initializePolyhedralFeatures();
	}

	//shape->setMargin(0.001);
	m_physics->m_collisionShapes.push_back(shape);

	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(1.f);
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	float color[4] = {1, 1, 1, 1};
	float orn[4] = {0, 0, 0, 1};
	float pos[4] = {0, 3, 0, 0};
	btVector3 position(pos[0], pos[1], pos[2]);
	startTransform.setOrigin(position);
	btRigidBody* body = m_physics->createRigidBody(mass, startTransform, shape);

	bool useConvexHullForRendering = ((m_options & ObjUseConvexHullForRendering) != 0);

	if (!useConvexHullForRendering)
	{
		int shapeId = m_guiHelper->registerGraphicsShape(&glmesh->m_vertices->at(0).xyzw[0],
														 glmesh->m_numvertices,
														 &glmesh->m_indices->at(0),
														 glmesh->m_numIndices,
														 B3_GL_TRIANGLES, -1);
		shape->setUserIndex(shapeId);
		int renderInstance = m_guiHelper->registerGraphicsInstance(shapeId, pos, orn, color, scaling);
		body->setUserIndex(renderInstance);
	}

	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}

CommonExampleInterface* ET_RigidBodyFromObjCreateFunc(CommonExampleOptions& options)
{
	return new RigidBodyFromObjExample(options.m_guiHelper, options.m_option);
}

B3_STANDALONE_EXAMPLE(ET_RigidBodyFromObjCreateFunc)
