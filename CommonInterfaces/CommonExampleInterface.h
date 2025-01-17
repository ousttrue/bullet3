#pragma once
#include "CommonCameraInterface.h"
#include <functional>

struct CommandProcessorCreationInterface
{
	virtual ~CommandProcessorCreationInterface() {}
	virtual class CommandProcessorInterface* createCommandProcessor() = 0;
	virtual void deleteCommandProcessor(CommandProcessorInterface*) = 0;
};

struct CommonExampleOptions
{
	struct GUIHelperInterface* m_guiHelper = nullptr;

	//Those are optional, some examples will use them others don't. Each example should work with them being 0.
	int m_option = 0;
	struct CommonGraphicsApp* m_app = nullptr;
	const char* m_fileName = nullptr;
	class SharedMemoryInterface* m_sharedMem = nullptr;
	CommandProcessorCreationInterface* m_commandProcessorCreation = nullptr;
	bool m_skipGraphicsUpdate = false;
};

class CommonExampleInterface
{
public:
	using CreateFunc = std::function<class CommonExampleInterface*(CommonExampleOptions& options)>;
	virtual ~CommonExampleInterface() {}
	virtual void initPhysics(CommonCameraInterface* camera, struct GUIHelperInterface* m_guiHelper) {}
	virtual void exitPhysics() {}
	virtual void updateGraphics() {}
	virtual void stepSimulation(float deltaTime) {}
	virtual void physicsDebugDraw(int debugFlags) {}  //for now we reuse the flags in Bullet/src/LinearMath/btIDebugDraw.h
	virtual CameraResetInfo cameraResetInfo() const { return {}; }
	virtual bool mouseMoveCallback(const CommonCameraInterface* camera, float x, float y) { return false; }
	virtual bool mouseButtonCallback(const CommonCameraInterface* camera, int button, int state, float x, float y, ButtonFlags flags) { return false; }
	virtual bool keyboardCallback(int key, int state) { return false; }
	virtual void vrControllerMoveCallback(int controllerId, float pos[4], float orientation[4], float analogAxis, float auxAnalogAxes[10]) {}
	virtual void vrControllerButtonCallback(int controllerId, int button, int state, float pos[4], float orientation[4]) {}
	virtual void vrHMDMoveCallback(int controllerId, float pos[4], float orientation[4]) {}
	virtual void vrGenericTrackerMoveCallback(int controllerId, float pos[4], float orientation[4]) {}
	virtual void processCommandLineArgs(int argc, char* argv[]){};
	virtual class btDiscreteDynamicsWorld* getDynamicsWorld() { return nullptr; }
};

class EmptyExample : public CommonExampleInterface
{
public:
	static CommonExampleInterface* CreateFunc(struct CommonExampleOptions& /* unusedOptions*/)
	{
		return new EmptyExample;
	}
};

class ExampleEntries
{
public:
	virtual ~ExampleEntries() {}
	virtual void initExampleEntries() = 0;
	virtual void initOpenCLExampleEntries() = 0;
	virtual int getNumRegisteredExamples() = 0;
	virtual CommonExampleInterface::CreateFunc getExampleCreateFunc(int index) = 0;
	virtual const char* getExampleName(int index) = 0;
	virtual const char* getExampleDescription(int index) = 0;
	virtual int getExampleOption(int index) = 0;
};

CommonExampleInterface* StandaloneExampleCreateFunc(CommonExampleOptions& options);

#ifdef B3_USE_STANDALONE_EXAMPLE
#define B3_STANDALONE_EXAMPLE(ExampleFunc)                                             \
	CommonExampleInterface* StandaloneExampleCreateFunc(CommonExampleOptions& options) \
	{                                                                                  \
		return ExampleFunc(options);                                                   \
	}
#else  //B3_USE_STANDALONE_EXAMPLE
#define B3_STANDALONE_EXAMPLE(ExampleFunc)
#endif  //B3_USE_STANDALONE_EXAMPLE
