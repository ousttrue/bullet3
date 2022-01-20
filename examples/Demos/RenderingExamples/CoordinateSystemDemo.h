#pragma once
#include <CommonExampleInterface.h>
///quick demo showing the right-handed coordinate system and positive rotations around each axis
class CoordinateSystemDemo : public CommonExampleInterface
{
	struct CommonGraphicsApp* m_app;
	float m_x;
	float m_y;
	float m_z;

public:
	CoordinateSystemDemo(CommonGraphicsApp* app);
	CameraResetInfo cameraResetInfo() const override;
	void initPhysics(CommonCameraInterface* camera, struct GUIHelperInterface* m_guiHelper) override {}
	void stepSimulation(float deltaTime) override
	{
		m_x += 0.01f;
		m_y += 0.01f;
		m_z += 0.01f;
	}
	void physicsDebugDraw(int debugDrawFlags) override;

private:
	void drawArc(const btVector3& center, const btVector3& normal, const btVector3& axis, btScalar radiusA, btScalar radiusB, btScalar minAngle, btScalar maxAngle,
				 const btVector3& color, bool drawSect, btScalar stepDegrees = btScalar(10.f));

public:
};
