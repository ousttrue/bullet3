#pragma once
#include <CommonRigidBodyBase.h>
struct BasicExample : public CommonRigidBodyBase
{
	BasicExample() : CommonRigidBodyBase({}) {}
	void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper) override;
	CameraResetInfo cameraResetInfo() const override
	{
		CameraResetInfo info;
		info.camDist = 4;
		info.pitch = -35;
		info.yaw = 52;
		info.camPosX = 0;
		info.camPosY = 0;
		info.camPosZ = 0;
		return info;
	}
};
