#include <CommonExampleInterface.h>
#include <CommonGUIHelperInterface.h>
#include <GlfwApp.h>
#include <b3Clock.h>
#include <OpenGLGuiHelper.h>
#include <stdio.h>
#include <BasicDemo/BasicExample.h>
#include <memory>

int main(int argc, char* argv[])
{
	GlfwApp app("Bullet Standalone Example", 1024, 768, true);
	OpenGLGuiHelper gui(&app, false);
	auto camera = app.m_instancingRenderer->getActiveCamera();
	// auto example = std::make_unique<BasicExample>(&gui);
	auto example = std::make_unique<BasicExample>();

	auto prevMouseButtonCallback = app.m_window->getMouseButtonCallback();
	app.m_window->setMouseButtonCallback([&example, &prevMouseButtonCallback, camera](int button, int state, float x, float y, ButtonFlags flags)
										 {
											 bool handled = example->mouseButtonCallback(camera, button, state, x, y, flags);
											 if (!handled)
											 {
												 if (prevMouseButtonCallback)
													 prevMouseButtonCallback(button, state, x, y, flags);
											 } });

	auto prevMouseMoveCallback = app.m_window->getMouseMoveCallback();
	app.m_window->setMouseMoveCallback([&example, &prevMouseMoveCallback, camera](float x, float y)
									   {
										   bool handled = example->mouseMoveCallback(camera, x, y);
										   if (!handled)
										   {
											   if (prevMouseMoveCallback)
												   prevMouseMoveCallback(x, y);
										   } });

	example->processCommandLineArgs(argc, argv);

	example->initPhysics(camera, &gui);
	// m_guiHelper->createPhysicsDebugDrawer(m_dynamicsWorld);
	// if (m_dynamicsWorld->getDebugDrawer())
	// 	m_dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawContactPoints);
	gui.resetCamera(example->cameraResetInfo());

	b3Clock clock;

	while (!app.m_window->requestedExit())
	{
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
		// m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);
		app.m_instancingRenderer->init();
		app.m_instancingRenderer->updateCamera(app.getUpAxis());
		if (auto world = example->getDynamicsWorld())
		{
			gui.syncPhysicsToGraphics(world);
			gui.render(world);
		}

		DrawGridData dg;
		dg.upAxis = app.getUpAxis();
		app.drawGrid(dg);

		app.swapBuffer();
	}

	return 0;
}
