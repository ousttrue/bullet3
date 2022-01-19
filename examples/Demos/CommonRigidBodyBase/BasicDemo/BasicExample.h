#pragma once
#include <CommonRigidBodyBase.h>
struct BasicExample : public CommonRigidBodyBase
{
	BasicExample() : CommonRigidBodyBase({}) {}
	BasicExample(struct GUIHelperInterface *p) : CommonRigidBodyBase(p) {}
	CameraResetInfo cameraResetInfo() const override;
	void initWorld(Physics *world) override;
};
