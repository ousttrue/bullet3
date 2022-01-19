#include "BasicExample.h"
#define ARRAY_SIZE_Y 5
#define ARRAY_SIZE_X 5
#define ARRAY_SIZE_Z 5

void BasicExample::initPhysics(CommonCameraInterface* camera, struct GUIHelperInterface* m_guiHelper)
{
	m_guiHelper->setUpAxis(1);

	m_physics = new Physics();

	//m_dynamicsWorld->setGravity(btVector3(0,0,0));
	auto m_dynamicsWorld = m_physics->getDynamicsWorld();
	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);

	if (m_dynamicsWorld->getDebugDrawer())
		m_dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawContactPoints);

	///create a few basic rigid bodies
	btBoxShape* groundShape = m_physics->createBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

	//groundShape->initializePolyhedralFeatures();
	//btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),50);

	m_physics->m_collisionShapes.push_back(groundShape);

	{
		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -50, 0));
		btScalar mass(0.);
		m_physics->createRigidBody(mass, groundTransform, groundShape, btVector4(0, 0, 1, 1));
	}

	{
		//create a few dynamic rigidbodies
		// Re-using the same collision is better for memory usage and performance

		btBoxShape* colShape = m_physics->createBoxShape(btVector3(.1, .1, .1));

		//btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		m_physics->m_collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		for (int k = 0; k < ARRAY_SIZE_Y; k++)
		{
			for (int i = 0; i < ARRAY_SIZE_X; i++)
			{
				for (int j = 0; j < ARRAY_SIZE_Z; j++)
				{
					startTransform.setOrigin(btVector3(
						btScalar(0.2 * i),
						btScalar(2 + .2 * k),
						btScalar(0.2 * j)));

					m_physics->createRigidBody(mass, startTransform, colShape);
				}
			}
		}
	}

	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}
