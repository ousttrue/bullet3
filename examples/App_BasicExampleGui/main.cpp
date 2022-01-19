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
	// OpenGLGuiHelper gui(&app, false);
	auto camera = app.m_instancingRenderer->getActiveCamera();
	// CommonExampleOptions options(&gui);
	std::unique_ptr<CommonExampleInterface> example;
	example.reset();

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

	example->initPhysics(camera, {});
	// gui.resetCamera(example->cameraResetInfo());
	// if (m_app->m_renderer && m_app->m_renderer->getActiveCamera())
	// {
	// 	m_app->m_renderer->getActiveCamera()->setCameraDistance(dist);
	// 	m_app->m_renderer->getActiveCamera()->setCameraPitch(pitch);
	// 	m_app->m_renderer->getActiveCamera()->setCameraYaw(yaw);
	// 	m_app->m_renderer->getActiveCamera()->setCameraTargetPosition(targetPos[0], targetPos[1], targetPos[2]);
	// }

	b3Clock clock;

	while (!app.m_window->requestedExit())
	{
		// step simulation
		btScalar dtSec = btScalar(clock.getTimeInSeconds());
		if (dtSec > 0.1)
			dtSec = 0.1;
		// m_guiHelper->getRenderInterface()->removeAllInstances();
		example->stepSimulation(dtSec);
		// m_guiHelper->autogenerateGraphicsObjects(m_dynamicsWorld);

		clock.reset();

		// render
		app.m_instancingRenderer->init();
		app.m_instancingRenderer->updateCamera(app.getUpAxis());
		if (auto world = example->getDynamicsWorld())
		{
			// gui.syncPhysicsToGraphics(world);
			// gui.render(world);
		}

		DrawGridData dg;
		dg.upAxis = app.getUpAxis();
		app.drawGrid(dg);

		app.swapBuffer();
	}

	example->exitPhysics();

	return 0;
}
