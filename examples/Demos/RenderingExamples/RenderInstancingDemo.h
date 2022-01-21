#pragma once
#include <CommonExampleInterface.h>
#include <Bullet3Common/b3AlignedObjectArray.h>
#include <Bullet3Common/b3Vector3.h>

// quick demo showing the right-handed coordinate system and positive rotations around each axis
class RenderInstancingDemo : public CommonExampleInterface
{
	CommonGraphicsApp* m_app;
	float m_x = 0;
	float m_y = 0;
	float m_z = 0;
	b3AlignedObjectArray<int> m_movingInstances;
	enum
	{
		numCubesX = 20,
		numCubesY = 20
	};

public:
	RenderInstancingDemo(CommonGraphicsApp* app);
	CameraResetInfo cameraResetInfo() const override;
	void stepSimulation(float deltaTime) override;
};
