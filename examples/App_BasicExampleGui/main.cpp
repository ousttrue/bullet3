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

	gui.resetCamera(example->cameraResetInfo());
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
		example->stepSimulation(dtSec);
		clock.reset();

		// render
		app.m_instancingRenderer->init();
		app.m_instancingRenderer->updateCamera(app.getUpAxis());
		if (auto world = example->getDynamicsWorld())
		{
				gui.syncPhysicsToGraphics(world);
				gui.render(world);
		}
		// void renderScene()
		// {  // render the scene
		// 	if (!gIsHeadless)
		// 	{  // while the simulation is not running headlessly, render to screen
		// 		CommonRigidBodyBase::renderScene();

		// 		if (m_physics->getDynamicsWorld()->getDebugDrawer())
		// 		{
		// 			m_physics->debugDraw(m_physics->getDynamicsWorld()->getDebugDrawer()->getDebugMode());
		// 		}
		// 	}
		// 	mIsHeadless = gIsHeadless;
		// }		
		// virtual void renderScene()
		// {
		// 	CommonRigidBodyBase::renderScene();
		// 	btSoftRigidDynamicsWorld* softWorld = getSoftDynamicsWorld();

		// 	for (int i = 0; i < softWorld->getSoftBodyArray().size(); i++)
		// 	{
		// 		btSoftBody* psb = (btSoftBody*)softWorld->getSoftBodyArray()[i];
		// 		//if (softWorld->getDebugDrawer() && !(softWorld->getDebugDrawer()->getDebugMode() & (btIDebugDraw::DBG_DrawWireframe)))
		// 		{
		// 			btSoftBodyHelpers::DrawFrame(psb, softWorld->getDebugDrawer());
		// 			btSoftBodyHelpers::Draw(psb, softWorld->getDebugDrawer(), softWorld->getDrawFlags());
		// 		}
		// 	}
		// }

		// void SimpleClothExample::renderScene()
		// {
		// 	CommonRigidBodyBase::renderScene();
		// 	btSoftRigidDynamicsWorld* softWorld = getSoftDynamicsWorld();

		// 	for (int i = 0; i < softWorld->getSoftBodyArray().size(); i++)
		// 	{
		// 		btSoftBody* psb = (btSoftBody*)softWorld->getSoftBodyArray()[i];
		// 		//if (softWorld->getDebugDrawer() && !(softWorld->getDebugDrawer()->getDebugMode() & (btIDebugDraw::DBG_DrawWireframe)))
		// 		{
		// 			btSoftBodyHelpers::DrawFrame(psb, softWorld->getDebugDrawer());
		// 			btSoftBodyHelpers::Draw(psb, softWorld->getDebugDrawer(), softWorld->getDrawFlags());
		// 		}
		// 	}
		// }

		// void NewtonsRopeCradleExample::renderScene()
		// {
		// 	CommonRigidBodyBase::renderScene();
		// 	btSoftRigidDynamicsWorld* softWorld = getSoftDynamicsWorld();

		// 	for (int i = 0; i < softWorld->getSoftBodyArray().size(); i++)
		// 	{
		// 		btSoftBody* psb = (btSoftBody*)softWorld->getSoftBodyArray()[i];
		// 		//if (softWorld->getDebugDrawer() && !(softWorld->getDebugDrawer()->getDebugMode() & (btIDebugDraw::DBG_DrawWireframe)))
		// 		{
		// 			btSoftBodyHelpers::DrawFrame(psb, softWorld->getDebugDrawer());
		// 			btSoftBodyHelpers::Draw(psb, softWorld->getDebugDrawer(), softWorld->getDrawFlags());
		// 		}
		// 	}
		// }

		DrawGridData dg;
		dg.upAxis = app.getUpAxis();
		app.drawGrid(dg);

		app.swapBuffer();
	}

	example->exitPhysics();

	return 0;
}
