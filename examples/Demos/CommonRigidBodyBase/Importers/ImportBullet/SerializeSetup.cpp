#include "SerializeSetup.h"
#include "../Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.h"
#include "CommonCameraInterface.h"

#include <CommonRigidBodyBase.h>

class SerializeSetup : public CommonRigidBodyBase
{
	char m_fileName[1024];

public:
	SerializeSetup(struct GUIHelperInterface* helper, const char* fileName);
	virtual ~SerializeSetup();

	void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper) override;
	virtual void stepSimulation(float deltaTime);
	virtual void setFileName(const char* fileName)
	{
		memcpy(m_fileName, fileName, strlen(fileName) + 1);
	}
	CameraResetInfo cameraResetInfo() const override
	{
		CameraResetInfo info;
		info.camDist = 9.5;
		info.pitch = -20;
		info.yaw = -2.8;
		info.camPosX = -0.2;
		info.camPosY = -1.4;
		info.camPosZ = 3.5;
		return info;
	}
};

SerializeSetup::SerializeSetup(struct GUIHelperInterface* helper, const char* fileName)
	: CommonRigidBodyBase(helper)
{
	if (fileName)
	{
		setFileName(fileName);
	}
	else
	{
		setFileName("spider.bullet");
	}
}
SerializeSetup::~SerializeSetup()
{
}

void SerializeSetup::initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper)
{
	m_physics = new Physics;
	auto m_dynamicsWorld = m_physics->getDynamicsWorld();
	m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);
	m_dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawContactPoints);
	btBulletWorldImporter* importer = new btBulletWorldImporter(m_dynamicsWorld);

	const char* prefix[] = {"", "./", "./data/", "../data/", "../../data/", "../../../data/", "../../../../data/"};
	int numPrefixes = sizeof(prefix) / sizeof(const char*);
	char relativeFileName[1024];
	FILE* f = 0;

	for (int i = 0; !f && i < numPrefixes; i++)
	{
		sprintf(relativeFileName, "%s%s", prefix[i], m_fileName);
		f = fopen(relativeFileName, "rb");
		if (f)
		{
			break;
		}
	}
	if (f)
	{
		fclose(f);
	}

	importer->loadFile(relativeFileName);

	//for now, guess the up axis from gravity
	if (m_dynamicsWorld->getGravity()[1] == 0.f)
	{
		m_guiHelper->setUpAxis(2);
	}
	else
	{
		m_guiHelper->setUpAxis(1);
	}

	//example code to export the dynamics world to a .bullet file

	btDefaultSerializer* serializer = new btDefaultSerializer();
	m_dynamicsWorld->serialize(serializer);

	FILE* file = fopen("SerializeSetupTestFile.bullet", "wb");
	fwrite(serializer->getBufferPointer(), serializer->getCurrentBufferSize(), 1, file);
	fclose(file);

	m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
}

void SerializeSetup::stepSimulation(float deltaTime)
{
	CommonRigidBodyBase::stepSimulation(deltaTime);
}

class CommonExampleInterface* SerializeBulletCreateFunc(struct CommonExampleOptions& options)
{
	return new SerializeSetup(options.m_guiHelper, options.m_fileName);
}
