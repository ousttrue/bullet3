
#include "R2D2GraspExample.h"

#include <CommonGraphicsAppInterface.h>
#include "Bullet3Common/b3Quaternion.h"
#include "Bullet3Common/b3AlignedObjectArray.h"
#include <CommonRenderInterface.h>
#include <CommonExampleInterface.h>
#include <CommonGUIHelperInterface.h>
#include "../SharedMemory/PhysicsServerSharedMemory.h"
#include "../SharedMemory/PhysicsClientC_API.h"
#include <string>

#include "../RobotSimulator/b3RobotSimulatorClientAPI.h"
#include "../Utils/b3Clock.h"

///quick demo showing the right-handed coordinate system and positive rotations around each axis
class R2D2GraspExample : public CommonExampleInterface
{
	CommonGraphicsApp* m_app;
	GUIHelperInterface* m_guiHelper;
	b3RobotSimulatorClientAPI m_robotSim;
	int m_options;
	int m_r2d2Index;

	b3AlignedObjectArray<int> m_movingInstances;
	enum
	{
		numCubesX = 20,
		numCubesY = 20
	};

public:
	R2D2GraspExample(GUIHelperInterface* helper, int options)
		: m_app(helper->getAppInterface()),
		  m_guiHelper(helper),
		  m_options(options),
		  m_r2d2Index(-1)
	{
	}
	virtual ~R2D2GraspExample()
	{
	}

	void initPhysics(CommonCameraInterface *camera, struct GUIHelperInterface *m_guiHelper) override
	{
		int mode = eCONNECT_EXISTING_EXAMPLE_BROWSER;
		m_robotSim.setGuiHelper(m_guiHelper);
		bool connected = m_robotSim.connect(mode);
		m_robotSim.configureDebugVisualizer(COV_ENABLE_RGB_BUFFER_PREVIEW, 0);
		m_robotSim.configureDebugVisualizer(COV_ENABLE_DEPTH_BUFFER_PREVIEW, 0);
		m_robotSim.configureDebugVisualizer(COV_ENABLE_SEGMENTATION_MARK_PREVIEW, 0);

		b3Printf("robotSim connected = %d", connected);

		if ((m_options & eROBOTIC_LEARN_GRASP) != 0)
		{
			{
				b3RobotSimulatorLoadUrdfFileArgs args;
				args.m_startPosition.setValue(0, 0, .5);
				m_r2d2Index = m_robotSim.loadURDF("r2d2.urdf", args);

				if (m_r2d2Index >= 0)
				{
					int numJoints = m_robotSim.getNumJoints(m_r2d2Index);
					b3Printf("numJoints = %d", numJoints);

					for (int i = 0; i < numJoints; i++)
					{
						b3JointInfo jointInfo;
						m_robotSim.getJointInfo(m_r2d2Index, i, &jointInfo);
						b3Printf("joint[%d].m_jointName=%s", i, jointInfo.m_jointName);
					}
					int wheelJointIndices[4] = {2, 3, 6, 7};
					int wheelTargetVelocities[4] = {-10, -10, -10, -10};
					for (int i = 0; i < 4; i++)
					{
						b3RobotSimulatorJointMotorArgs controlArgs(CONTROL_MODE_VELOCITY);
						controlArgs.m_targetVelocity = wheelTargetVelocities[i];
						controlArgs.m_maxTorqueValue = 1e30;
						m_robotSim.setJointMotorControl(m_r2d2Index, wheelJointIndices[i], controlArgs);
					}
				}
			}
			{
				b3RobotSimulatorLoadFileResults results;
				m_robotSim.loadSDF("kiva_shelf/model.sdf", results);
			}
			{
				m_robotSim.loadURDF("plane.urdf");
			}

			m_robotSim.setGravity(btVector3(0, 0, -10));
		}

		if ((m_options & eROBOTIC_LEARN_COMPLIANT_CONTACT) != 0)
		{
			b3RobotSimulatorLoadUrdfFileArgs args;
			b3RobotSimulatorLoadFileResults results;
			{
				args.m_startPosition.setValue(0, 0, 2.5);
				args.m_startOrientation.setEulerZYX(0, 0.2, 0);
				m_r2d2Index = m_robotSim.loadURDF("cube_soft.urdf", args);
			}
			{
				args.m_startPosition.setValue(0, 2, 2.5);
				args.m_startOrientation.setEulerZYX(0, 0.2, 0);
				m_robotSim.loadURDF("cube_no_friction.urdf", args);
			}
			{
				args.m_startPosition.setValue(0, 0, 0);
				args.m_startOrientation.setEulerZYX(0, 0.2, 0);
				args.m_forceOverrideFixedBase = true;
				m_robotSim.loadURDF("plane.urdf", args);
			}

			m_robotSim.setGravity(btVector3(0, 0, -10));
		}

		if ((m_options & eROBOTIC_LEARN_ROLLING_FRICTION) != 0)
		{
			b3RobotSimulatorLoadUrdfFileArgs args;
			b3RobotSimulatorLoadFileResults results;
			{
				args.m_startPosition.setValue(0, 0, 2.5);
				args.m_startOrientation.setEulerZYX(0, 0, 0);
				args.m_useMultiBody = true;
				m_robotSim.loadURDF("sphere2_rolling_friction.urdf", args);
			}
			{
				args.m_startPosition.setValue(0, 2, 2.5);
				args.m_startOrientation.setEulerZYX(0, 0, 0);
				args.m_useMultiBody = true;
				m_robotSim.loadURDF("sphere2.urdf", args);
			}
			{
				args.m_startPosition.setValue(0, 0, 0);
				args.m_startOrientation.setEulerZYX(0, 0.2, 0);
				args.m_useMultiBody = true;
				args.m_forceOverrideFixedBase = true;
				m_robotSim.loadURDF("plane.urdf", args);
			}

			m_robotSim.setGravity(btVector3(0, 0, -10));
		}
	}
	void exitPhysics() override
	{
		m_robotSim.disconnect();
	}
	void stepSimulation(float deltaTime) override
	{
		m_robotSim.stepSimulation();
	}

	CameraResetInfo cameraResetInfo() const override
	{
		CameraResetInfo info;
		info.camDist = 3;
		info.pitch = -30;
		info.yaw = -75;
		info.camPosX = -0.2;
		info.camPosY = 0.8;
		info.camPosZ = 0.3;
		info.upAxis = 2;
		return info;
	}
};

class CommonExampleInterface* R2D2GraspExampleCreateFunc(struct CommonExampleOptions& options)
{
	return new R2D2GraspExample(options.m_guiHelper, options.m_option);
}
