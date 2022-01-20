
#include "JointLimit.h"

#include <CommonGraphicsAppInterface.h>
#include "Bullet3Common/b3AlignedObjectArray.h"
#include <CommonRenderInterface.h>
#include <CommonExampleInterface.h>
#include <CommonGUIHelperInterface.h>
#include "../SharedMemory/PhysicsServerSharedMemory.h"
#include "../SharedMemory/PhysicsClientC_API.h"
#include "../SharedMemory/SharedMemoryPublic.h"
#include <CommonParameterInterface.h>
#include <string>

#include "../RobotSimulator/b3RobotSimulatorClientAPI.h"
#include "../Utils/b3Clock.h"
#include "CommonCameraInterface.h"

class JointLimit : public CommonExampleInterface
{
	GUIHelperInterface* m_guiHelper;
	b3RobotSimulatorClientAPI m_robotSim;

public:
	JointLimit(GUIHelperInterface* helper, int options)
		: m_guiHelper(helper)
	{
	}

	~JointLimit() override
	{
	}

	void physicsDebugDraw(int debugDrawMode) override
	{
		m_robotSim.debugDraw(debugDrawMode);
	}

	void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper) override
	{
		int mode = eCONNECT_EXISTING_EXAMPLE_BROWSER;
		m_robotSim.setGuiHelper(m_guiHelper);
		bool connected = m_robotSim.connect(mode);

		b3Printf("robotSim connected = %d", connected);

		m_robotSim.configureDebugVisualizer(COV_ENABLE_RGB_BUFFER_PREVIEW, 0);
		m_robotSim.configureDebugVisualizer(COV_ENABLE_DEPTH_BUFFER_PREVIEW, 0);
		m_robotSim.configureDebugVisualizer(COV_ENABLE_SEGMENTATION_MARK_PREVIEW, 0);

		b3RobotSimulatorSetPhysicsEngineParameters physicsArgs;
		physicsArgs.m_constraintSolverType = eConstraintSolverLCP_DANTZIG;

		physicsArgs.m_defaultGlobalCFM = 1e-6;

		m_robotSim.setNumSolverIterations(10);

		b3RobotSimulatorLoadUrdfFileArgs loadArgs;
		int humanoid = m_robotSim.loadURDF("test_joints_MB.urdf", loadArgs);

		b3RobotSimulatorChangeDynamicsArgs dynamicsArgs;
		dynamicsArgs.m_linearDamping = 0;
		dynamicsArgs.m_angularDamping = 0;
		m_robotSim.changeDynamics(humanoid, -1, dynamicsArgs);

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

	CameraResetInfo cameraResetInfo() const override
	{
		CameraResetInfo info;
		info.camDist = 3;
		info.pitch = -10;
		info.yaw = 18;
		info.camPosX = 0.6;
		info.camPosY = 0.8;
		info.camPosZ = 0.3;
		return info;
	}
};

class CommonExampleInterface* JointLimitCreateFunc(struct CommonExampleOptions& options)
{
	return new JointLimit(options.m_guiHelper, options.m_option);
}
