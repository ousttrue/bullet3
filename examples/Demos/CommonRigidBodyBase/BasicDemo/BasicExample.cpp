#include "BasicExample.h"
#define ARRAY_SIZE_Y 5
#define ARRAY_SIZE_X 5
#define ARRAY_SIZE_Z 5

CameraResetInfo BasicExample::cameraResetInfo() const
{
	CameraResetInfo info;
	info.camDist = 4;
	info.pitch = -35;
	info.yaw = 52;
	info.camPosX = 0;
	info.camPosY = 0;
	info.camPosZ = 0;
	info.upAxis = 1;
	return info;
}

void BasicExample::initWorld(Physics* physics)
{
	{
		///create a few basic rigid bodies
		btBoxShape* groundShape = physics->createBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
		physics->m_collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -50, 0));
		btScalar mass(0.);
		physics->createRigidBody(mass, groundTransform, groundShape, btVector4(0, 0, 1, 1));
	}

	{
		//create a few dynamic rigidbodies
		// Re-using the same collision is better for memory usage and performance

		btBoxShape* colShape = physics->createBoxShape(btVector3(.1, .1, .1));

		//btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		physics->m_collisionShapes.push_back(colShape);

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

					physics->createRigidBody(mass, startTransform, colShape);
				}
			}
		}
	}
}
