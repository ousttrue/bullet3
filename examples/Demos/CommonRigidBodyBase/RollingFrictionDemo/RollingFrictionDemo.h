#pragma once
#include <CommonRigidBodyBase.h>

/// The RollingFrictionDemo shows the use of rolling friction.
/// Spheres will come to a rest on a sloped plane using a constraint. Damping cannot achieve the same.
/// Generally it is best to leave the rolling friction coefficient zero (or close to zero).
class RollingFrictionDemo : public CommonRigidBodyBase
{
public:
	RollingFrictionDemo() : CommonRigidBodyBase({}) {}
	CameraResetInfo cameraResetInfo() const override;
	void initWorld(Physics *physics) override;
};
