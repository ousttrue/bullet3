#include <CommonExampleInterface.h>
#include <CommonGUIHelperInterface.h>
#include <GlfwApp.h>
#include <b3Clock.h>
#include <OpenGLGuiHelper.h>
#include <stdio.h>
//
#include <BasicDemo/BasicExample.h>
#include <RollingFrictionDemo/RollingFrictionDemo.h>
#include "../RenderingExamples/CoordinateSystemDemo.h"
//
#include <functional>
#include <memory>
#include "CommonCameraInterface.h"

int main(int argc, char* argv[])
{
	GlfwApp app;

	auto window = app.createWindow({1024, 768, "Bullet Standalone Example"});
	if (!window)
	{
		return 1;
	}

	auto example = std::make_unique<CoordinateSystemDemo>(&app);
	auto renderer = app.getRenderer();
	auto camera = renderer->getActiveCamera();
	window->mouseButtonCallback.push_back(
		[&example, camera](int button, int state, float x, float y, ButtonFlags flags)
		{
			return example->mouseButtonCallback(camera, button, state, x, y, flags);
		});
	window->mouseButtonCallback.push_back(
		std::bind(&CommonCameraInterface::mouseButtonCallback, camera, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));

	window->mouseMoveCallback.push_back(
		[&example, camera](float x, float y)
		{
			return example->mouseMoveCallback(camera, x, y);
		});
	window->mouseMoveCallback.push_back(
		std::bind(&CommonCameraInterface::mouseMoveCallback, camera, std::placeholders::_1, std::placeholders::_2));

	example->processCommandLineArgs(argc, argv);

	example->initPhysics(camera, {});
	// m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);
	// if (m_dynamicsWorld->getDebugDrawer())
	// 	m_dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawContactPoints);
	// int mode = btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawConstraints + btIDebugDraw::DBG_DrawConstraintLimits;
	// m_dynamicsWorld->getDebugDrawer()->setDebugMode(mode);

	camera->resetCamera(example->cameraResetInfo());

	b3Clock clock;
	OpenGLGuiHelper gui(&app, false);
	while (!window->requestedExit())
	{
		window->startRendering();
		// m_guiHelper->getRenderInterface()->removeAllInstances();

		// step simulation
		btScalar dtSec = btScalar(clock.getTimeInSeconds());
		if (dtSec > 0.1)
		{
			dtSec = 0.1;
		}
		clock.reset();
		example->stepSimulation(dtSec);

		// render
		renderer->init();
		renderer->updateCamera();
		if (auto world = example->getDynamicsWorld())
		{
			gui.syncPhysicsToGraphics(world);
			gui.render(world);
		}
		example->physicsDebugDraw(0);

		DrawGridData dg;
		dg.upAxis = camera->getCameraUpAxis();
		app.drawGrid(dg);

		window->endRendering();
	}

	return 0;
}
