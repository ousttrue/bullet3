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
	CommonExampleOptions options(&gui);
	std::unique_ptr<CommonExampleInterface> example;
	example.reset(BasicExampleCreateFunc(options));

	auto prevMouseButtonCallback = app.m_window->getMouseButtonCallback();
	app.m_window->setMouseButtonCallback([&example, &prevMouseButtonCallback](int button, int state, float x, float y)
										 {
											 bool handled = example->mouseButtonCallback(button, state, x, y);
											 if (!handled)
											 {
												 if (prevMouseButtonCallback)
													 prevMouseButtonCallback(button, state, x, y);
											 } });

	auto prevMouseMoveCallback = app.m_window->getMouseMoveCallback();
	app.m_window->setMouseMoveCallback([&example, &prevMouseMoveCallback](float x, float y)
									   {
										   bool handled = example->mouseMoveCallback(x, y);
										   if (!handled)
										   {
											   if (prevMouseMoveCallback)
												   prevMouseMoveCallback(x, y);
										   } });

	example->processCommandLineArgs(argc, argv);

	example->initPhysics();
	example->resetCamera();

	b3Clock clock;

	while (!app.m_window->requestedExit())
	{
		app.m_instancingRenderer->init();
		app.m_instancingRenderer->updateCamera(app.getUpAxis());

		btScalar dtSec = btScalar(clock.getTimeInSeconds());
		if (dtSec > 0.1)
			dtSec = 0.1;

		example->stepSimulation(dtSec);
		clock.reset();

		example->renderScene();

		DrawGridData dg;
		dg.upAxis = app.getUpAxis();
		app.drawGrid(dg);

		app.swapBuffer();
	}

	example->exitPhysics();

	return 0;
}
