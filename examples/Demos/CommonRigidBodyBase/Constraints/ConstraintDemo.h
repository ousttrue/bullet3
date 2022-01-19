#pragma once
#include <CommonRigidBodyBase.h>

// AllConstraintDemo shows how to create a constraint, like Hinge or btGenericD6constraint
class AllConstraintDemo : public CommonRigidBodyBase
{
	//keep track of variables to delete memory at the end
	void setupEmptyDynamicsWorld();
	// for cone-twist motor driving
	float m_Time = 0;
	class btConeTwistConstraint* m_ctc = nullptr;

public:
	AllConstraintDemo() : CommonRigidBodyBase({}) {}
	CameraResetInfo cameraResetInfo() const override;
	void initWorld(Physics* physics) override;
	bool keyboardCallback(int key, int state) override;
};
