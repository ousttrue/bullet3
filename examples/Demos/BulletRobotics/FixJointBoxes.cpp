#include "FixJointBoxes.h"

#include <CommonGraphicsAppInterface.h>
#include "Bullet3Common/b3AlignedObjectArray.h"
#include <CommonRenderInterface.h>
#include <CommonExampleInterface.h>
#include <CommonGUIHelperInterface.h>
#include <PhysicsServerSharedMemory.h>
#include <PhysicsClientC_API.h>
#include <SharedMemoryPublic.h>
#include <CommonParameterInterface.h>
#include <string>
#include <vector>
#include "../RobotSimulator/b3RobotSimulatorClientAPI.h"
#include "CommonCameraInterface.h"

static btScalar numSolverIterations = 1000;
static btScalar solverId = 0;

class FixJointBoxes : public CommonExampleInterface
{
	GUIHelperInterface* m_guiHelper;
	b3RobotSimulatorClientAPI m_robotSim;
	b3RobotSimulatorSetPhysicsEngineParameters physicsArgs;
	int solver;

	const size_t numCubes;
	std::vector<int> cubeIds;

public:
	FixJointBoxes(GUIHelperInterface* helper, int options)
		: m_guiHelper(helper),
		  numCubes(30),
		  cubeIds(numCubes, 0),
		  solver(solverId)
	{
	}

	virtual ~FixJointBoxes()
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

		{
			b3RobotSimulatorLoadUrdfFileArgs args;
			b3RobotSimulatorChangeDynamicsArgs dynamicsArgs;

			for (int i = 0; i < numCubes; i++)
			{
				args.m_forceOverrideFixedBase = (i == 0);
				args.m_startPosition.setValue(0, i * 0.05, 1);
				cubeIds[i] = m_robotSim.loadURDF("cube_small.urdf", args);

				b3RobotJointInfo jointInfo;

				jointInfo.m_parentFrame[1] = -0.025;
				jointInfo.m_childFrame[1] = 0.025;

				if (i > 0)
				{
					m_robotSim.createConstraint(cubeIds[i], -1, cubeIds[i - 1], -1, &jointInfo);
					m_robotSim.setCollisionFilterGroupMask(cubeIds[i], -1, 0, 0);
				}

				m_robotSim.loadURDF("plane.urdf");
			}
		}

		{
			SliderParams slider("Direct solver", &solverId);
			slider.m_minVal = 0;
			slider.m_maxVal = 1;
			m_guiHelper->getParameterInterface()->registerSliderFloatParameter(slider);
		}
		{
			SliderParams slider("numSolverIterations", &numSolverIterations);
			slider.m_minVal = 50;
			slider.m_maxVal = 1e4;
			m_guiHelper->getParameterInterface()->registerSliderFloatParameter(slider);
		}

		physicsArgs.m_defaultGlobalCFM = 1e-6;
		m_robotSim.setPhysicsEngineParameter(physicsArgs);

		m_robotSim.setGravity(btVector3(0, 0, -10));
		m_robotSim.setNumSolverIterations((int)numSolverIterations);
	}

	void exitPhysics() override
	{
		m_robotSim.disconnect();
	}

	void resetCubePosition()
	{
		for (int i = 0; i < numCubes; i++)
		{
			btVector3 pos(0, i * (btScalar)0.05, 1);
			btQuaternion quar(0, 0, 0, 1);
			m_robotSim.resetBasePositionAndOrientation(cubeIds[i], pos, quar);
		}
	}
	void stepSimulation(float deltaTime) override
	{
		int newSolver = (int)(solverId + 0.5);
		if (newSolver != solver)
		{
			printf("Switching solver, new %d, old %d\n", newSolver, solver);
			solver = newSolver;
			resetCubePosition();
			if (solver)
			{
				physicsArgs.m_constraintSolverType = eConstraintSolverLCP_DANTZIG;
			}
			else
			{
				physicsArgs.m_constraintSolverType = eConstraintSolverLCP_SI;
			}

			m_robotSim.setPhysicsEngineParameter(physicsArgs);
		}
		m_robotSim.setNumSolverIterations((int)numSolverIterations);
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
		info.camDist = 1;
		info.pitch = -20;
		info.yaw = -30;
		info.camPosX = 0;
		info.camPosY = 0.2;
		info.camPosZ = 0.5;
		return info;
	}
};

class CommonExampleInterface* FixJointBoxesCreateFunc(struct CommonExampleOptions& options)
{
	return new FixJointBoxes(options.m_guiHelper, options.m_option);
}
