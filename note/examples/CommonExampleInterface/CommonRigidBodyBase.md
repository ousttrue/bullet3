# CommonRigidBodyBase

普通に `bulletphysics` するサンプル。


1. [btDiscreteDynamicsWorld](https://pybullet.org/Bullet/BulletFull/classbtDiscreteDynamicsWorld.html) を作成
1. シーン(RigidBOdy や SoftBOdy)をセットアップする
1. stepSimulation する


```{digraph} CommonRigidBodyBase
rankdir="BT"
"CommonRigidBodyBase" -> "CommonExampleInterface";
```

## BasicDemo

```{digraph} BasicExample
rankdir="BT"
"BasicExample" -> "CommonRigidBodyBase";
```

```c++
struct CommonRigidBodyBase : public CommonExampleInterface;

struct BasicExample : public CommonRigidBodyBase
{
	BasicExample(struct GUIHelperInterface* helper)
		: CommonRigidBodyBase(helper)
	{
	}
	virtual ~BasicExample() {}
	void initPhysics(CommonCameraInterface *camera) override;
	void renderScene() override;
	void resetCamera()
	{
		float dist = 4;
		float pitch = -35;
		float yaw = 52;
		float info.camPosX = 0;
info.camPosY = 0;
info.camPosZ = 0;

		m_guiHelper->resetCamera(dist, yaw, pitch, targetPos[0], targetPos[1], targetPos[2]);
	}
};
```

##  Constraints
##  DeformableDemo
##  DynamicControlDemo
##  FractureDemo
##  GyroscopicDemo
##  Heightfield
##  InverseDynamics
##  Planar2D
##  Raycast
##  RigidBody
##  RollingFrictionDemo
##  SoftDemo
##  Tutorial
##  ExtendedTutorials
##  Evolution
##  Vehicles
##  Importers
