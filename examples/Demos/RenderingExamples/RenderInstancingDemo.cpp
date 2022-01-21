#include "RenderInstancingDemo.h"
#include <CommonGraphicsAppInterface.h>
#include <Bullet3Common/b3Quaternion.h>

RenderInstancingDemo::RenderInstancingDemo(CommonGraphicsApp* app)
	: m_app(app),
	  m_x(0),
	  m_y(0),
	  m_z(0)
{
	{
		b3Vector3 extents = b3MakeVector3(100, 100, 100);
		extents[cameraResetInfo().upAxis] = 1;

		int xres = 20;
		int yres = 20;

		b3Vector4 color0 = b3MakeVector4(0.1, 0.1, 0.1, 1);
		b3Vector4 color1 = b3MakeVector4(0.6, 0.6, 0.6, 1);
		m_app->registerGrid(xres, yres, color0, color1);
	}

	{
		int boxId = m_app->registerCubeShape(0.1, 0.1, 0.1);

		for (int i = -numCubesX / 2; i < numCubesX / 2; i++)
		{
			for (int j = -numCubesY / 2; j < numCubesY / 2; j++)
			{
				b3Vector3 pos = b3MakeVector3(i, j, j);
				pos[cameraResetInfo().upAxis] = 1;
				b3Quaternion orn(0, 0, 0, 1);
				b3Vector4 color = b3MakeVector4(0.3, 0.3, 0.3, 1);
				b3Vector3 scaling = b3MakeVector3(1, 1, 1);
				int instanceId = m_app->m_renderer->registerGraphicsInstance(boxId, pos, orn, color, scaling);
				m_movingInstances.push_back(instanceId);
			}
		}
	}

	m_app->m_renderer->writeTransforms();
}

CameraResetInfo RenderInstancingDemo::cameraResetInfo() const
{
	CameraResetInfo info;
	info.camDist = 13;
	info.pitch = -13;
	info.yaw = 50;
	info.camPosX = -1;
	info.camPosY = 0;
	info.camPosZ = -0.3;
	info.upAxis = 2;
	return info;
}

void RenderInstancingDemo::stepSimulation(float deltaTime)
{
	m_x += 0.01f;
	m_y += 0.01f;
	m_z += 0.01f;
	int index = 0;
	for (int i = -numCubesX / 2; i < numCubesX / 2; i++)
	{
		for (int j = -numCubesY / 2; j < numCubesY / 2; j++)
		{
			b3Vector3 pos = b3MakeVector3(i, j, j);
			pos[cameraResetInfo().upAxis] = 1 + 1 * b3Sin(m_x + i - j);
			float orn[4] = {0, 0, 0, 1};
			m_app->m_renderer->writeSingleInstanceTransformToCPU(pos, orn, m_movingInstances[index++]);
		}
	}
	m_app->m_renderer->writeTransforms();
}
