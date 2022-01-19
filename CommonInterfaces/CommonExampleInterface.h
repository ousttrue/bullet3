#pragma once
#include "CommonCameraInterface.h"
#include <functional>

enum ButtonFlags
{
	ButtonFlagsNone = 0,
	ButtonFlagsAlt = 1,
	ButtonFlagsCtrl = 1 << 1,
};

struct CommandProcessorCreationInterface
{
	virtual ~CommandProcessorCreationInterface() {}
	virtual class CommandProcessorInterface* createCommandProcessor() = 0;
	virtual void deleteCommandProcessor(CommandProcessorInterface*) = 0;
};

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
	using CreateFunc = std::function<class CommonExampleInterface*(CommonExampleOptions& options)>;
	virtual ~CommonExampleInterface() {}
	virtual void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper) = 0;
	virtual void exitPhysics(){}
	virtual void updateGraphics() {}
	virtual void stepSimulation(float deltaTime) = 0;
	virtual void physicsDebugDraw(int debugFlags, struct GUIHelperInterface *m_guiHelper=0) {}  //for now we reuse the flags in Bullet/src/LinearMath/btIDebugDraw.h
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
	void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper) override {}
	void exitPhysics() override {}
	void stepSimulation(float deltaTime) override {}
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
