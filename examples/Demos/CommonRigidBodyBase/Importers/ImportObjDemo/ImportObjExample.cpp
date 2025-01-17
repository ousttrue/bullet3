#include "ImportObjExample.h"
#include <vector>
#include <GLInstancingRenderer.h>
#include <tiny_obj_loader.h>
#include <GLInstanceGraphicsShape.h>
#include "CommonCameraInterface.h"
#include "btBulletDynamicsCommon.h"
#include "Wavefront2GLInstanceGraphicsShape.h"
#include <b3ResourcePath.h>
#include <b3BulletDefaultFileIO.h>
#include "Bullet3Common/b3FileUtils.h"
#include <stb_image.h>
#include <CommonRigidBodyBase.h>
#include <b3ImportMeshUtility.h>

class ImportObjSetup : public CommonRigidBodyBase
{
	std::string m_fileName;

public:
	ImportObjSetup(struct GUIHelperInterface* helper, const char* fileName);
	virtual ~ImportObjSetup();

	void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper) override;

	CameraResetInfo cameraResetInfo() const override
	{
		CameraResetInfo info;
		info.camDist = 18;
		info.pitch = -46;
		info.yaw = 120;
		info.camPosX = -2;
		info.camPosY = -2;
		info.camPosZ = -2;
		info.upAxis = 2;
		return info;
	}
};

ImportObjSetup::ImportObjSetup(struct GUIHelperInterface* helper, const char* fileName)
	: CommonRigidBodyBase(helper)
{
	if (fileName)
	{
		m_fileName = fileName;
	}
	else
	{
		m_fileName = "cube.obj";  //"sponza_closed.obj";//sphere8.obj";
	}
}

ImportObjSetup::~ImportObjSetup()
{
}

int loadAndRegisterMeshFromFile2(const std::string& fileName, CommonRenderInterface* renderer)
{
	int shapeId = -1;

	b3ImportMeshData meshData;
	b3BulletDefaultFileIO fileIO;
	if (b3ImportMeshUtility::loadAndRegisterMeshFromFileInternal(fileName, meshData, &fileIO))
	{
		int textureIndex = -1;

		if (meshData.m_textureImage1)
		{
			textureIndex = renderer->registerTexture(meshData.m_textureImage1, meshData.m_textureWidth, meshData.m_textureHeight);
		}

		shapeId = renderer->registerShape(&meshData.m_gfxShape->m_vertices->at(0).xyzw[0],
										  meshData.m_gfxShape->m_numvertices,
										  &meshData.m_gfxShape->m_indices->at(0),
										  meshData.m_gfxShape->m_numIndices,
										  B3_GL_TRIANGLES,
										  textureIndex);
		delete meshData.m_gfxShape;
		if (!meshData.m_isCached)
		{
			delete meshData.m_textureImage1;
		}
	}
	return shapeId;
}

void ImportObjSetup::initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper)
{
	m_physics = new Physics;
	auto m_dynamicsWorld = m_physics->getDynamicsWorld();
	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);
	m_dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

	btTransform trans;
	trans.setIdentity();
	trans.setRotation(btQuaternion(btVector3(1, 0, 0), SIMD_HALF_PI));
	btVector3 position = trans.getOrigin();
	btQuaternion orn = trans.getRotation();

	btVector3 scaling(1, 1, 1);
	btVector4 color(1, 1, 1, 1);

	int shapeId = loadAndRegisterMeshFromFile2(m_fileName, m_guiHelper->getRenderInterface());
	if (shapeId >= 0)
	{
		//int id =
		m_guiHelper->getRenderInterface()->registerGraphicsInstance(shapeId, position, orn, color, scaling);
	}
}

CommonExampleInterface* ImportObjCreateFunc(struct CommonExampleOptions& options)
{
	return new ImportObjSetup(options.m_guiHelper, options.m_fileName);
}
