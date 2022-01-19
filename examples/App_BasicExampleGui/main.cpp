#include <CommonExampleInterface.h>
#include <CommonGUIHelperInterface.h>
#include <GlfwApp.h>
#include <b3Clock.h>
#include <OpenGLGuiHelper.h>
#include <stdio.h>
#include <BasicDemo/BasicExample.h>
#include <RollingFrictionDemo/RollingFrictionDemo.h>
#include <memory>

int main(int argc, char* argv[])
{
	GlfwApp app;
	auto window = app.createWindow({1024, 768, "Bullet Standalone Example"});
	if (!window)
	{
		return 1;
	}

	OpenGLGuiHelper gui(&app, false);
	auto camera = app.m_instancingRenderer->getActiveCamera();
	auto example = std::make_unique<RollingFrictionDemo>();

	auto prevMouseButtonCallback = window->getMouseButtonCallback();
	window->setMouseButtonCallback([&example, &prevMouseButtonCallback, camera](int button, int state, float x, float y, ButtonFlags flags)
								   {
											 bool handled = example->mouseButtonCallback(camera, button, state, x, y, flags);
											 if (!handled)
											 {
												 if (prevMouseButtonCallback)
													 prevMouseButtonCallback(button, state, x, y, flags);
											 } });

	auto prevMouseMoveCallback = window->getMouseMoveCallback();
	window->setMouseMoveCallback([&example, &prevMouseMoveCallback, camera](float x, float y)
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
	camera->resetCamera(example->cameraResetInfo());

	// if (0)
	// {
	// 	btSerializer* s = new btDefaultSerializer;
	// 	m_dynamicsWorld->serialize(s);
	// 	char resourcePath[1024];
	// 	if (b3ResourcePath::findResourcePath("slope.bullet", resourcePath, 1024, 0))
	// 	{
	// 		FILE* f = fopen(resourcePath, "wb");
	// 		fwrite(s->getBufferPointer(), s->getCurrentBufferSize(), 1, f);
	// 		fclose(f);
	// 	}
	// }

	b3Clock clock;

	while (!window->requestedExit())
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
		app.m_instancingRenderer->updateCamera();
		if (auto world = example->getDynamicsWorld())
		{
			gui.syncPhysicsToGraphics(world);
			gui.render(world);
		}

		DrawGridData dg;
		dg.upAxis = camera->getCameraUpAxis();
		app.drawGrid(dg);

		app.swapBuffer();
	}

	return 0;
}
