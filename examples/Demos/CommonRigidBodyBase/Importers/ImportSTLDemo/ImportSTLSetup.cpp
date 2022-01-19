#include "ImportSTLSetup.h"
#include <vector>
#include <GLInstancingRenderer.h>
#include <GLInstanceGraphicsShape.h>
#include "CommonCameraInterface.h"
#include "btBulletDynamicsCommon.h"
#include "LoadMeshFromSTL.h"
#include <CommonRigidBodyBase.h>
#include "../../Utils/b3ResourcePath.h"
#include "../../Utils/b3BulletDefaultFileIO.h"

class ImportSTLSetup : public CommonRigidBodyBase
{
	const char* m_fileName;
	btVector3 m_scaling;

public:
	ImportSTLSetup(struct GUIHelperInterface* helper, const char* fileName);
	virtual ~ImportSTLSetup();

	void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper) override;
	CameraResetInfo cameraResetInfo() const override
	{
		CameraResetInfo info;
		info.camDist = 3.5;
		info.pitch = -28;
		info.yaw = -136;
		info.camPosX = 0.47;
		info.camPosY = 0;
		info.camPosZ = -0.64;
		info.upAxis = 2;
		return info;
	}
};

ImportSTLSetup::ImportSTLSetup(struct GUIHelperInterface* helper, const char* fileName)
	: CommonRigidBodyBase(helper),
	  m_scaling(btVector3(10, 10, 10))
{
	if (fileName)
	{
		m_fileName = fileName;
		m_scaling = btVector3(0.01, 0.01, 0.01);
	}
	else
	{
		m_fileName = "l_finger_tip.stl";
	}
}

ImportSTLSetup::~ImportSTLSetup()
{
}

void ImportSTLSetup::initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper)
{
	m_physics = new Physics;
	auto m_dynamicsWorld = m_physics->getDynamicsWorld();
	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);
	m_dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

	char relativeFileName[1024];
	if (!b3ResourcePath::findResourcePath(m_fileName, relativeFileName, 1024, 0))
	{
		b3Warning("Cannot find file %s\n", m_fileName);
		return;
	}

	btVector3 shift(0, 0, 0);
	//	int index=10;

	{
		b3BulletDefaultFileIO fileIO;
		GLInstanceGraphicsShape* gfxShape = LoadMeshFromSTL(relativeFileName, &fileIO);

		btTransform trans;
		trans.setIdentity();
		trans.setRotation(btQuaternion(btVector3(1, 0, 0), SIMD_HALF_PI));

		btVector3 position = trans.getOrigin();
		btQuaternion orn = trans.getRotation();

		btVector4 color(0, 0, 1, 1);

		int shapeId = m_guiHelper->getRenderInterface()->registerShape(&gfxShape->m_vertices->at(0).xyzw[0], gfxShape->m_numvertices, &gfxShape->m_indices->at(0), gfxShape->m_numIndices);

		m_guiHelper->getRenderInterface()->registerGraphicsInstance(shapeId, position, orn, color, m_scaling);
	}
}

class CommonExampleInterface* ImportSTLCreateFunc(struct CommonExampleOptions& options)
{
	return new ImportSTLSetup(options.m_guiHelper, options.m_fileName);
}
