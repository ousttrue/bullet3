#pragma once
#include <CommonRigidBodyBase.h>
struct TestHingeTorque : public CommonRigidBodyBase
{
	bool m_once = true;
	btAlignedObjectArray<btJointFeedback *> m_jointFeedback;

	TestHingeTorque() : CommonRigidBodyBase({}) {}
	virtual ~TestHingeTorque();
	CameraResetInfo cameraResetInfo() const override;
	void initWorld(Physics *physics) override;
	void stepSimulation(float deltaTime) override;
};
