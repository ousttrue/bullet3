#pragma once
#include <CommonRigidBodyBase.h>

struct Dof6Spring2Setup : public CommonRigidBodyBase
{
	Dof6Spring2Setup();
	~Dof6Spring2Setup() override;
	CameraResetInfo cameraResetInfo() const override;
    Physics* createPhysics()override;
	void initWorld(Physics* physics) override;
	void stepSimulation(float deltaTime) override;

private:
	struct Dof6Spring2SetupInternalData* m_data;
	void animate();
};
