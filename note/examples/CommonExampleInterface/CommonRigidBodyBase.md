## CommonRigidBodyBase

```{digraph} CommonRigidBodyBase
rankdir="BT"
"CommonRigidBodyBase" -> "CommonExampleInterface";
```

### BasicDemo

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
	virtual void initPhysics();
	virtual void renderScene();
	void resetCamera()
	{
		float dist = 4;
		float pitch = -35;
		float yaw = 52;
		float targetPos[3] = {0, 0, 0};
		m_guiHelper->resetCamera(dist, yaw, pitch, targetPos[0], targetPos[1], targetPos[2]);
	}
};
```
