# Demos

各デモは以下のようなインターフェースを持つ。

```c++
struct CommonExampleOptions
{
	struct GUIHelperInterface* m_guiHelper;

	//Those are optional, some examples will use them others don't. Each example should work with them being 0.
	int m_option;
	const char* m_fileName;
	class SharedMemoryInterface* m_sharedMem;
	CommandProcessorCreationInterface* m_commandProcessorCreation;
	bool m_skipGraphicsUpdate;

	CommonExampleOptions(struct GUIHelperInterface* helper, int option = 0)
		: m_guiHelper(helper),
		  m_option(option),
		  m_fileName(0),
		  m_sharedMem(0),
		  m_commandProcessorCreation(0),
		  m_skipGraphicsUpdate(false)
	{
	}
};

class CommonExampleInterface
{
public:
	typedef class CommonExampleInterface*(CreateFunc)(CommonExampleOptions& options);

	virtual ~CommonExampleInterface()
	{
	}

	virtual void initPhysics() = 0;
	virtual void exitPhysics() = 0;
	virtual void updateGraphics() {}
	virtual void stepSimulation(float deltaTime) = 0;
	virtual void renderScene() = 0;
	virtual void physicsDebugDraw(int debugFlags) = 0;  //for now we reuse the flags in Bullet/src/LinearMath/btIDebugDraw.h
	//reset camera is only called when switching demo. this way you can restart (initPhysics) and watch in a specific location easier
	virtual void resetCamera(){};
	virtual bool mouseMoveCallback(float x, float y) = 0;
	virtual bool mouseButtonCallback(int button, int state, float x, float y) = 0;
	virtual bool keyboardCallback(int key, int state) = 0;

	virtual void vrControllerMoveCallback(int controllerId, float pos[4], float orientation[4], float analogAxis, float auxAnalogAxes[10]) {}
	virtual void vrControllerButtonCallback(int controllerId, int button, int state, float pos[4], float orientation[4]) {}
	virtual void vrHMDMoveCallback(int controllerId, float pos[4], float orientation[4]) {}
	virtual void vrGenericTrackerMoveCallback(int controllerId, float pos[4], float orientation[4]) {}

	virtual void processCommandLineArgs(int argc, char* argv[]){};
};

// BasicDemo の例
class CommonExampleInterface* BasicExampleCreateFunc(struct CommonExampleOptions& options);
```

```{toctree}
CommonRigidBodyBase
```
