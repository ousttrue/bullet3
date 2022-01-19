#pragma once

#include <CommonRigidBodyBase.h>
struct ConstraintPhysicsSetup : public CommonRigidBodyBase
{
	ConstraintPhysicsSetup() : CommonRigidBodyBase({}) {}
	CameraResetInfo cameraResetInfo() const override;
	void initWorld(Physics *world) override;
	void stepSimulation(float deltaTime) override;
};
