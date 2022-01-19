
#include "DynamicTexturedCubeDemo.h"
#include "Bullet3Common/b3Logging.h"
#include <CommonGraphicsAppInterface.h>
#include "Bullet3Common/b3Quaternion.h"
#include "Bullet3Common/b3AlignedObjectArray.h"
#include <CommonRenderInterface.h>
#include <CommonExampleInterface.h>
#include <CommonGUIHelperInterface.h>
// #include "GwenGUISupport/GraphingTexture.h"

#include <Common2dCanvasInterface.h>
#include "../RenderingExamples/TimeSeriesCanvas.h"
#include "../RenderingExamples/TimeSeriesFontData.h"
#include <b3ImportMeshUtility.h>
#include <GLInstanceGraphicsShape.h>
#include "CommonCameraInterface.h"
#include "TinyVRGui.h"
#include <CommonParameterInterface.h>

class DynamicTexturedCubeDemo : public CommonExampleInterface
{
	CommonGraphicsApp* m_app;
	b3AlignedObjectArray<int> m_movingInstances;

	TinyVRGui* m_tinyVrGUI;

	enum
	{
		numCubesX = 1,
		numCubesY = 1
	};

public:
	DynamicTexturedCubeDemo(CommonGraphicsApp* app)
		: m_app(app),
		  m_tinyVrGUI(0)
	{
		m_app->setUpAxis(2);

		{
			b3Vector3 extents = b3MakeVector3(100, 100, 100);
			extents[m_app->getUpAxis()] = 1;

			int xres = 20;
			int yres = 20;

			b3Vector4 color0 = b3MakeVector4(0.1, 0.1, 0.5, 1);
			b3Vector4 color1 = b3MakeVector4(0.6, 0.6, 0.6, 1);
			m_app->registerGrid(xres, yres, color0, color1);
		}

		ComboBoxParams comboParams;
		comboParams.m_comboboxId = 0;
		comboParams.m_numItems = 0;
		comboParams.m_startItem = 0;
		comboParams.m_callback = 0;     //MyComboBoxCallback;
		comboParams.m_userPointer = 0;  //this;

		m_tinyVrGUI = new TinyVRGui(comboParams, m_app->m_renderer);
		m_tinyVrGUI->init();

		m_app->m_renderer->writeTransforms();
	}
	virtual ~DynamicTexturedCubeDemo()
	{
		delete m_tinyVrGUI;
	}

	void initPhysics(CommonCameraInterface* camera) override
	{
	}
	void exitPhysics() override
	{
	}
	void stepSimulation(float deltaTime) override
	{
		static b3Transform tr = b3Transform::getIdentity();
		static b3Scalar t = 0.f;
		t += deltaTime;
		tr.setOrigin(b3MakeVector3(0., 0., 2.) + b3MakeVector3(0., 0., 0.02 * b3Sin(t)));

		m_tinyVrGUI->tick(deltaTime, tr);

		m_app->m_renderer->writeTransforms();
	}

	virtual void physicsDebugDraw()
	{
	}

	CameraResetInfo cameraResetInfo() const override
	{
		CameraResetInfo info;
		info.camDist = 1.15;
		info.pitch = -33.7;
		info.yaw = 396;
		info.camPosX = -0.5;
		info.camPosY = 0.7;
		info.camPosZ = 1.45;
		return info;
	}
};

class CommonExampleInterface* DynamicTexturedCubeDemoCreateFunc(struct CommonExampleOptions& options)
{
	return new DynamicTexturedCubeDemo(options.m_guiHelper->getAppInterface());
}
