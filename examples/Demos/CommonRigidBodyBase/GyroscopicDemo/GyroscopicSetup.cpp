#include "GyroscopicSetup.h"

#include <CommonRigidBodyBase.h>
#include "CommonCameraInterface.h"

struct GyroscopicSetup : public CommonRigidBodyBase
{
	GyroscopicSetup(struct GUIHelperInterface* helper);

	virtual ~GyroscopicSetup()
	{
	}
	void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper) override;

	virtual void physicsDebugDraw(int debugFlags, struct GUIHelperInterface *m_guiHelper);

	CameraResetInfo cameraResetInfo() const override
	{
		CameraResetInfo info;
		info.camDist = 20;
		info.pitch = -16;
		info.yaw = 180;
		info.camPosX = -2.4;
		info.camPosY = 0.4;
		info.camPosZ = -0.24;
		info.upAxis = 1;
		return info;
	}
};

static int gyroflags[4] = {
	0,  //none, no gyroscopic term
	BT_ENABLE_GYROSCOPIC_FORCE_EXPLICIT,
	BT_ENABLE_GYROSCOPIC_FORCE_IMPLICIT_WORLD,
	BT_ENABLE_GYROSCOPIC_FORCE_IMPLICIT_BODY};

static const char* gyroNames[4] = {
	"No Gyroscopic",
	"Explicit",
	"Implicit (World)",
	"Implicit (Body)"};

GyroscopicSetup::GyroscopicSetup(struct GUIHelperInterface* helper)
	: CommonRigidBodyBase(helper)
{
}

void GyroscopicSetup::initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper)
{
	m_physics = new Physics;
	auto m_dynamicsWorld = m_physics->getDynamicsWorld();
	m_dynamicsWorld->setGravity(btVector3(0, 0, 0));
	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);

	btVector3 positions[4] = {
		btVector3(-10, 8, 4),
		btVector3(-5, 8, 4),
		btVector3(0, 8, 4),
		btVector3(5, 8, 4),
	};

	for (int i = 0; i < 4; i++)
	{
		btCylinderShapeZ* pin = new btCylinderShapeZ(btVector3(0.1, 0.1, 0.2));
		btBoxShape* box = new btBoxShape(btVector3(1, 0.1, 0.1));
		box->setMargin(0.01);
		pin->setMargin(0.01);
		btCompoundShape* compound = new btCompoundShape();
		compound->addChildShape(btTransform::getIdentity(), pin);
		btTransform offsetBox(btMatrix3x3::getIdentity(), btVector3(0, 0, 0.2));
		compound->addChildShape(offsetBox, box);
		btScalar masses[2] = {0.3, 0.1};
		btVector3 localInertia;
		btTransform principal;
		compound->calculatePrincipalAxisTransform(masses, principal, localInertia);

		btRigidBody* body = new btRigidBody(1, 0, compound, localInertia);
		btTransform tr;
		tr.setIdentity();
		tr.setOrigin(positions[i]);
		body->setCenterOfMassTransform(tr);
		body->setAngularVelocity(btVector3(0, 0.1, 10));  //51));
		//body->setLinearVelocity(btVector3(3, 0, 0));
		body->setFriction(btSqrt(1));
		m_dynamicsWorld->addRigidBody(body);
		body->setFlags(gyroflags[i]);
		m_dynamicsWorld->getSolverInfo().m_maxGyroscopicForce = 10.f;
		body->setDamping(0.0000f, 0.000f);
	}

	{
		//btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(0.5)));
		btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

		m_physics->m_collisionShapes.push_back(groundShape);
		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, 0, 0));
		btRigidBody* groundBody;
		groundBody = m_physics->createRigidBody(0, groundTransform, groundShape);
		groundBody->setFriction(btSqrt(2));
	}
	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}

void GyroscopicSetup::physicsDebugDraw(int debugFlags, struct GUIHelperInterface *m_guiHelper)
{
	CommonRigidBodyBase::physicsDebugDraw(debugFlags, m_guiHelper);

	//render method names above objects
	auto m_dynamicsWorld = m_physics->getDynamicsWorld();
	for (int i = 0; i < m_dynamicsWorld->getNumCollisionObjects(); i++)
	{
		btRigidBody* body = btRigidBody::upcast(m_dynamicsWorld->getCollisionObjectArray()[i]);
		if (body && body->getInvMass() > 0)
		{
			btTransform tr = body->getWorldTransform();
			btVector3 pos = tr.getOrigin() + btVector3(0, 0, 2);
			btScalar size = 1;
			m_guiHelper->drawText3D(gyroNames[i], pos.x(), pos.y(), pos.z(), size);
		}
	}
}

class CommonExampleInterface* GyroscopicCreateFunc(CommonExampleOptions& options)
{
	return new GyroscopicSetup(options.m_guiHelper);
}
