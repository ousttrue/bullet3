
#include "BoxStack.h"

#include <CommonGraphicsAppInterface.h>
#include "Bullet3Common/b3AlignedObjectArray.h"
#include <CommonRenderInterface.h>
#include <CommonExampleInterface.h>
#include <CommonGUIHelperInterface.h>
#include "../SharedMemory/PhysicsServerSharedMemory.h"
#include "../SharedMemory/PhysicsClientC_API.h"
#include <CommonParameterInterface.h>
#include "../SharedMemory/SharedMemoryPublic.h"
#include <string>

#include "../RobotSimulator/b3RobotSimulatorClientAPI.h"
#include "../Utils/b3Clock.h"
#include "CommonCameraInterface.h"

class BoxStackExample : public CommonExampleInterface
{
	CommonGraphicsApp* m_app;
	GUIHelperInterface* m_guiHelper;
	b3RobotSimulatorClientAPI m_robotSim;
	int m_options;

public:
	BoxStackExample(GUIHelperInterface* helper, int options)
		: m_app(helper->getAppInterface()),
		  m_guiHelper(helper),
		  m_options(options)
	{
		m_app->setUpAxis(2);
	}

	virtual ~BoxStackExample()
	{
	}

	virtual void physicsDebugDraw(int debugDrawMode)
	{
		m_robotSim.debugDraw(debugDrawMode);
	}

	void initPhysics(CommonCameraInterface* camera) override
	{
		int mode = eCONNECT_EXISTING_EXAMPLE_BROWSER;
		m_robotSim.setGuiHelper(m_guiHelper);
		bool connected = m_robotSim.connect(mode);

		b3Printf("robotSim connected = %d", connected);

		m_robotSim.configureDebugVisualizer(COV_ENABLE_RGB_BUFFER_PREVIEW, 0);
		m_robotSim.configureDebugVisualizer(COV_ENABLE_DEPTH_BUFFER_PREVIEW, 0);
		m_robotSim.configureDebugVisualizer(COV_ENABLE_SEGMENTATION_MARK_PREVIEW, 0);

		b3RobotSimulatorLoadUrdfFileArgs args;
		b3RobotSimulatorChangeDynamicsArgs dynamicsArgs;
		int massRatio = 4;
		int mass = 1;
		for (int i = 0; i < 8; i++)
		{
			args.m_startPosition.setValue(0, 0, i * .06);
			int boxIdx = m_robotSim.loadURDF("cube_small.urdf", args);
			dynamicsArgs.m_mass = mass;
			m_robotSim.changeDynamics(boxIdx, -1, dynamicsArgs);
			mass *= massRatio;
		}

		m_robotSim.loadURDF("plane.urdf");
		m_robotSim.setGravity(btVector3(0, 0, -10));
	}

	void exitPhysics() override
	{
		m_robotSim.disconnect();
	}
	void stepSimulation(float deltaTime) override
	{
		m_robotSim.stepSimulation();
	}

	bool mouseMoveCallback(const CommonCameraInterface* camera, float x, float y) override
	{
		return m_robotSim.mouseMoveCallback(x, y);
	}
	bool mouseButtonCallback(const CommonCameraInterface* camera, int button, int state, float x, float y, ButtonFlags flags) override
	{
		return m_robotSim.mouseButtonCallback(button, state, x, y);
	}
	bool keyboardCallback(int key, int state) override
	{
		return false;
	}

	CameraResetInfo cameraResetInfo() const override
	{
		CameraResetInfo info;
		info.camDist = 1.5;
		info.pitch = -10;
		info.yaw = 18;
		info.camPosX = -0.2f;
		info.camPosY = 0.8f;
		info.camPosZ = 0.3f;
		return info;
	}
};

class CommonExampleInterface* BoxStackExampleCreateFunc(struct CommonExampleOptions& options)
{
	return new BoxStackExample(options.m_guiHelper, options.m_option);
}
