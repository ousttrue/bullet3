#include "OpenGLExampleBrowser.h"
#include <ChromeTraceUtil.h>
#include <SharedMemoryPublic.h>
#include <OpenGLGuiHelper.h>
#include <GlfwApp.h>
#include <CommonCallbacks.h>
#include <CommonParameterInterface.h>
#include <CommonRenderInterface.h>
#include <Common2dCanvasInterface.h>
#include <Bullet3Common/b3CommandLineArgs.h>
#include <Bullet3Common/b3FileUtils.h>
#include <LinearMath/btQuickprof.h>
#include <LinearMath/btIDebugDraw.h>
#include <OpenGLInclude.h>
#include <assert.h>
#include <functional>
#include <string>
#include "CommonCameraInterface.h"
#include "CommonExampleInterface.h"

#define DEMO_SELECTION_COMBOBOX 13

#ifndef USE_OPENGL3
extern bool useShadowMap;
#endif
extern bool gDisableDeactivation;

struct FileImporterByExtension
{
	std::string m_extension;
	CommonExampleInterface::CreateFunc m_createFunc;
};

class OpenGLExampleBrowserInternalData
{
	CommonGUIInterface* m_gwen = nullptr;

	GlfwApp* s_app = 0;

	CommonParameterInterface* s_parameterInterface = 0;
	CommonRenderInterface* s_instancingRenderer = 0;
	OpenGLGuiHelper* s_guiHelper = 0;
#ifndef BT_NO_PROFILE
	MyProfileWindow* s_profWindow = 0;
#endif  //BT_NO_PROFILE

	const char* startFileName = "0_Bullet3Demo.txt";
	char staticPngFileName[1024];

private:
	int sCurrentDemoIndex = -1;
	int sCurrentHightlighted = 0;
	b3AlignedObjectArray<const char*> allNames;
	float gFixedTimeStep = 0;
	bool gAllowRetina = true;

	int gRenderDevice = -1;
	int gWindowBackend = 0;
	class ExampleEntries* gAllExamples = 0;

	bool visualWireframe = false;
	bool renderVisualGeometry = true;
	bool renderGrid = true;
	bool gEnableRenderLoop = true;

	bool renderGui = true;
	bool enable_experimental_opencl = false;

	bool gEnableDefaultKeyboardShortcuts = true;
	bool gEnableDefaultMousePicking = true;

	int gDebugDrawFlags = 0;

	const char* gPngFileName = 0;
	int gPngSkipFrames = 0;

public:
	bool pauseSimulation = false;
	bool singleStepSimulation = false;
	btAlignedObjectArray<FileImporterByExtension> gFileImporterByExtension;
	CommonWindowInterface* s_window = 0;
	CommonExampleInterface* sCurrentDemo = 0;
	bool gDisableDemoSelection = false;
	SharedMemoryInterface* sSharedMem = 0;

private:
	//in case of multi-threading, don't submit messages while the GUI is rendering (causing crashes)
	bool gBlockGuiMessages = false;

public:
	OpenGLExampleBrowserInternalData(ExampleEntries* examples)
		: gAllExamples(examples)
	{
	}

	~OpenGLExampleBrowserInternalData()
	{
		deleteDemo();
		delete s_parameterInterface;
		s_parameterInterface = 0;
		delete s_app->m_2dCanvasInterface;
		s_app->m_2dCanvasInterface = 0;

#ifndef BT_NO_PROFILE
		destroyProfileWindow(m_profWindow);
#endif

		delete m_gwen;
		m_gwen = nullptr;

		delete s_app;
		s_app = 0;

		gFileImporterByExtension.clear();
		gAllExamples = 0;
	}

private:
	void deleteDemo()
	{
		if (sCurrentDemo)
		{
			sCurrentDemo->exitPhysics();
			s_instancingRenderer->removeAllInstances();
			delete sCurrentDemo;
			sCurrentDemo = 0;
			delete s_guiHelper;
			s_guiHelper = 0;

			//  CProfileManager::CleanupMemory();
		}
	}

	bool MyKeyboardCallback(int key, int state)
	{
		//b3Printf("key=%d, state=%d", key, state);
		bool handled = false;

		if (renderGui && m_gwen && !handled)
		{
			handled = m_gwen->OnKeyboard(key, state);
		}

		if (!handled && sCurrentDemo)
		{
			handled = sCurrentDemo->keyboardCallback(key, state);
		}

		//checkout: is it desired to ignore keys, if the demo already handles them?
		//if (handled)
		// return;

		if (gEnableDefaultKeyboardShortcuts)
		{
			if (key == 'a' && state)
			{
				gDebugDrawFlags ^= btIDebugDraw::DBG_DrawAabb;
			}
			if (key == 'c' && state)
			{
				gDebugDrawFlags ^= btIDebugDraw::DBG_DrawContactPoints;
			}
			if (key == 'd' && state)
			{
				gDebugDrawFlags ^= btIDebugDraw::DBG_NoDeactivation;
				gDisableDeactivation = ((gDebugDrawFlags & btIDebugDraw::DBG_NoDeactivation) != 0);
			}
			if (key == 'j' && state)
			{
				gDebugDrawFlags ^= btIDebugDraw::DBG_DrawFrames;
			}

			if (key == 'k' && state)
			{
				gDebugDrawFlags ^= btIDebugDraw::DBG_DrawConstraints;
			}

			if (key == 'l' && state)
			{
				gDebugDrawFlags ^= btIDebugDraw::DBG_DrawConstraintLimits;
			}
			if (key == 'w' && state)
			{
				visualWireframe = !visualWireframe;
				gDebugDrawFlags ^= btIDebugDraw::DBG_DrawWireframe;
			}

			if (key == 'v' && state)
			{
				renderVisualGeometry = !renderVisualGeometry;
			}
			if (key == 'g' && state)
			{
				renderGrid = !renderGrid;
				renderGui = !renderGui;
			}

			if (key == 'i' && state)
			{
				pauseSimulation = !pauseSimulation;
			}
			if (key == 'o' && state)
			{
				singleStepSimulation = true;
			}

			if (key == 'p')
			{
				if (state)
				{
					b3ChromeUtilsStartTimings();
				}
				else
				{
#ifdef _WIN32
					b3ChromeUtilsStopTimingsAndWriteJsonFile("timings");
#else
					b3ChromeUtilsStopTimingsAndWriteJsonFile("/tmp/timings");
#endif
				}
			}

#ifndef NO_OPENGL3
			if (key == 's' && state)
			{
				useShadowMap = !useShadowMap;
			}
#endif
			if (key == B3G_F1)
			{
				static int count = 0;
				if (state)
				{
					b3Printf("F1 pressed %d", count++);

					if (gPngFileName)
					{
						b3Printf("disable image dump");

						gPngFileName = 0;
					}
					else
					{
						gPngFileName = gAllExamples->getExampleName(sCurrentDemoIndex);
						b3Printf("enable image dump %s", gPngFileName);
					}
				}
				else
				{
					b3Printf("F1 released %d", count++);
				}
			}
		}
		if (key == B3G_ESCAPE && s_window)
		{
			s_window->setRequestExit();
		}

		return false;
	}

	bool MyMouseMoveCallback(float x, float y)
	{
		bool handled = false;
		if (sCurrentDemo)
			handled = sCurrentDemo->mouseMoveCallback(s_guiHelper->getRenderInterface()->getActiveCamera(), x, y);
		if (renderGui && !handled && m_gwen)
		{
			handled = m_gwen->OnMouseMove(x, y);
		}
		return handled;
	}

	bool MyMouseButtonCallback(int button, int state, float x, float y, ButtonFlags flags)
	{
		bool handled = false;
		//try picking first
		if (sCurrentDemo)
		{
			handled = sCurrentDemo->mouseButtonCallback(s_guiHelper->getRenderInterface()->getActiveCamera(), button, state, x, y, flags);
		}

		if (renderGui && m_gwen && !handled)
		{
			handled = m_gwen->OnMouseButton(button, state, x, y);
		}

		return handled;
	}

	void OpenGLExampleBrowserVisualizerFlagCallback(int flag, bool enable)
	{
		if (flag == COV_ENABLE_Y_AXIS_UP)
		{
			//either Y = up or Z
			int upAxis = enable ? 1 : 2;
			s_app->m_instancingRenderer->getActiveCamera()->setCameraUpAxis(upAxis);
		}

		if (flag == COV_ENABLE_RENDERING)
		{
			gEnableRenderLoop = (enable != 0);
		}

		if (flag == COV_ENABLE_SINGLE_STEP_RENDERING)
		{
			if (enable)
			{
				gEnableRenderLoop = false;
				singleStepSimulation = true;
			}
			else
			{
				gEnableRenderLoop = true;
				singleStepSimulation = false;
			}
		}

		if (flag == COV_ENABLE_SHADOWS)
		{
			useShadowMap = enable;
		}
		if (flag == COV_ENABLE_GUI)
		{
			renderGui = enable;
			renderGrid = enable;
		}

		if (flag == COV_ENABLE_KEYBOARD_SHORTCUTS)
		{
			gEnableDefaultKeyboardShortcuts = enable;
		}
		if (flag == COV_ENABLE_MOUSE_PICKING)
		{
			gEnableDefaultMousePicking = enable;
		}

		if (flag == COV_ENABLE_WIREFRAME)
		{
			visualWireframe = enable;
			if (visualWireframe)
			{
				gDebugDrawFlags |= btIDebugDraw::DBG_DrawWireframe;
			}
			else
			{
				gDebugDrawFlags &= ~btIDebugDraw::DBG_DrawWireframe;
			}
		}
	}

	void openFileDemo(const char* filename)
	{
		deleteDemo();

		s_guiHelper = new OpenGLGuiHelper(s_app, false);
		s_guiHelper->setVisualizerFlagCallback(std::bind(&OpenGLExampleBrowserInternalData::OpenGLExampleBrowserVisualizerFlagCallback,
														 this, std::placeholders::_1, std::placeholders::_2));

		s_parameterInterface->removeAllParameters();

		CommonExampleOptions options(s_guiHelper, 1);
		options.m_fileName = filename;
		char fullPath[1024];
		sprintf(fullPath, "%s", filename);
		b3FileUtils::toLower(fullPath);

		for (int i = 0; i < gFileImporterByExtension.size(); i++)
		{
			if (strstr(fullPath, gFileImporterByExtension[i].m_extension.c_str()))
			{
				sCurrentDemo = gFileImporterByExtension[i].m_createFunc(options);
			}
		}

		if (sCurrentDemo)
		{
			sCurrentDemo->initPhysics(s_guiHelper->getRenderInterface()->getActiveCamera(), s_guiHelper);
			s_guiHelper->getRenderInterface()->getActiveCamera()->resetCamera(sCurrentDemo->cameraResetInfo());
		}
	}

	void selectDemo(int demoIndex)
	{
		bool resetCamera = (sCurrentDemoIndex != demoIndex);
		sCurrentDemoIndex = demoIndex;
		sCurrentHightlighted = demoIndex;
		int numDemos = gAllExamples->getNumRegisteredExamples();

		if (demoIndex > numDemos)
		{
			demoIndex = 0;
		}
		deleteDemo();

		auto func = gAllExamples->getExampleCreateFunc(demoIndex);
		if (func)
		{
			if (s_parameterInterface)
			{
				s_parameterInterface->removeAllParameters();
			}
			int option = gAllExamples->getExampleOption(demoIndex);
			s_guiHelper = new OpenGLGuiHelper(s_app, false);
			s_guiHelper->setVisualizerFlagCallback(std::bind(&OpenGLExampleBrowserInternalData::OpenGLExampleBrowserVisualizerFlagCallback,
															 this, std::placeholders::_1, std::placeholders::_2));

			CommonExampleOptions options(s_guiHelper, option);
			options.m_sharedMem = sSharedMem;
			sCurrentDemo = (func)(options);
			if (sCurrentDemo)
			{
				b3Printf("Selected demo: %s", gAllExamples->getExampleName(demoIndex));
				m_gwen->SetDescription(gAllExamples->getExampleDescription(demoIndex));

				sCurrentDemo->initPhysics(s_guiHelper->getRenderInterface()->getActiveCamera(), s_guiHelper);
				if (resetCamera)
				{
					auto info = sCurrentDemo->cameraResetInfo();
					s_guiHelper->getRenderInterface()->getActiveCamera()->resetCamera(info);
					s_app->m_instancingRenderer->updateCamera();
				}
			}
		}
	}

	void saveCurrentSettings(int currentEntry, const char* startFileName)
	{
		FILE* f = fopen(startFileName, "w");
		if (f)
		{
			fprintf(f, "--start_demo_name=%s\n", gAllExamples->getExampleName(sCurrentDemoIndex));
			float red, green, blue;
			s_app->getBackgroundColor(&red, &green, &blue);
			fprintf(f, "--background_color_red= %f\n", red);
			fprintf(f, "--background_color_green= %f\n", green);
			fprintf(f, "--background_color_blue= %f\n", blue);
			fprintf(f, "--fixed_timestep= %f\n", gFixedTimeStep);
			if (!gAllowRetina)
			{
				fprintf(f, "--disable_retina");
			}

			if (enable_experimental_opencl)
			{
				fprintf(f, "--enable_experimental_opencl\n");
			}

			fclose(f);
		}
	}

	void loadCurrentSettings(const char* startFileName, b3CommandLineArgs& args)
	{
		//int currentEntry= 0;
		FILE* f = fopen(startFileName, "r");
		if (f)
		{
			char oneline[1024];
			char* argv[] = {0, &oneline[0]};

			while (fgets(oneline, 1024, f) != NULL)
			{
				char* pos;
				if ((pos = strchr(oneline, '\n')) != NULL)
					*pos = '\0';
				args.addArgs(2, argv);
			}
			fclose(f);
		}
	}

	void MyComboBoxCallback(int comboId, const char* item)
	{
		//printf("comboId = %d, item = %s\n",comboId, item);
		if (comboId == DEMO_SELECTION_COMBOBOX)
		{
			//find selected item
			for (int i = 0; i < allNames.size(); i++)
			{
				if (strcmp(item, allNames[i]) == 0)
				{
					selectDemo(i);
					saveCurrentSettings(sCurrentDemoIndex, startFileName);
					break;
				}
			}
		}
	}

	void MyGuiPrintf(const char* msg)
	{
		printf("b3Printf: %s\n", msg);
		if (!gDisableDemoSelection && !gBlockGuiMessages)
		{
			m_gwen->ShowMessage(msg);
		}
	}

	void MyStatusBarPrintf(const char* msg)
	{
		printf("b3Printf: %s\n", msg);
		if (!gDisableDemoSelection && !gBlockGuiMessages)
		{
			m_gwen->SetStatusbarMessage(msg);
		}
	}

	void MyStatusBarError(const char* msg)
	{
		printf("Warning: %s\n", msg);
		if (!gDisableDemoSelection && !gBlockGuiMessages)
		{
			m_gwen->ShowErrorMessage(msg);
		}
		btAssert(0);
	}

	void quitCallback()
	{
		s_window->setRequestExit();
	}

	void fileOpenCallback()
	{
		char filename[1024];
		int len = s_window->fileOpenDialog(filename, 1024);
		if (len)
		{
			//todo(erwincoumans) check if it is actually URDF
			//printf("file open:%s\n", filename);
			openFileDemo(filename);
		}
	}

	void OnButtonB()
	{
		if (!gDisableDemoSelection)
		{
			selectDemo(sCurrentHightlighted);
			saveCurrentSettings(sCurrentDemoIndex, startFileName);
		}
	}

	void OnButtonD()
	{
		if (!gDisableDemoSelection)
		{
			selectDemo(sCurrentHightlighted);
			saveCurrentSettings(sCurrentDemoIndex, startFileName);
		}
	}

	void OnButtonE(int buttonId)
	{
		sCurrentHightlighted = buttonId;
	}

public:
	void init(const CommonGUIInterface::Factory& factory, int argc, char** argv)
	{
		b3CommandLineArgs args(argc, argv);

		loadCurrentSettings(startFileName, args);
		if (args.CheckCmdLineFlag("nogui"))
		{
			renderGrid = false;
			renderGui = false;
		}
		if (args.CheckCmdLineFlag("tracing"))
		{
			b3ChromeUtilsStartTimings();
		}
		args.GetCmdLineArgument("fixed_timestep", gFixedTimeStep);
		args.GetCmdLineArgument("png_skip_frames", gPngSkipFrames);
		///The OpenCL rigid body pipeline is experimental and
		///most OpenCL drivers and OpenCL compilers have issues with our kernels.
		///If you have a high-end desktop GPU such as AMD 7970 or better, or NVIDIA GTX 680 with up-to-date drivers
		///you could give it a try
		///Note that several old OpenCL physics examples still have to be ported over to this new Example Browser
		if (args.CheckCmdLineFlag("enable_experimental_opencl"))
		{
			enable_experimental_opencl = true;
			gAllExamples->initOpenCLExampleEntries();
		}

		if (args.CheckCmdLineFlag("disable_retina"))
		{
			gAllowRetina = false;
		}

		int width = 1024;
		int height = 768;

		if (args.CheckCmdLineFlag("width"))
		{
			args.GetCmdLineArgument("width", width);
		}
		if (args.CheckCmdLineFlag("height"))
		{
			args.GetCmdLineArgument("height", height);
		}

		args.GetCmdLineArgument("render_device", gRenderDevice);
		args.GetCmdLineArgument("window_backend", gWindowBackend);
		const char* appTitle = "Bullet Physics ExampleBrowser";
#if defined(_DEBUG) || defined(DEBUG)
		const char* optMode = "Debug build (slow)";
#else
		const char* optMode = "Release build";
#endif

#ifdef B3_USE_GLFW
		const char* glContext = "[glfw]";
#else
		const char* glContext = "[btgl]";
#endif

		char title[1024];
		sprintf(title, "%s using OpenGL3+ %s %s", appTitle, glContext, optMode);
		s_app = new GlfwApp();
		auto window = s_app->createWindow({width, height, title});

		char* gVideoFileName = 0;
		args.GetCmdLineArgument("mp4", gVideoFileName);
		int gVideoFps = 0;
		args.GetCmdLineArgument("mp4fps", gVideoFps);
		if (gVideoFps)
		{
			s_app->setMp4Fps(gVideoFps);
		}

#ifndef NO_OPENGL3
		if (gVideoFileName)
			s_app->dumpFramesToVideo(gVideoFileName);
#endif

		s_instancingRenderer = s_app->m_renderer;
		s_window = s_app->m_window;

		width = s_window->getWidth();
		height = s_window->getHeight();

		auto camera = s_app->m_renderer->getActiveCamera();
		camera->setCameraDistance(13);
		camera->setCameraPitch(0);
		camera->setCameraTargetPosition(0, 0, 0);

		s_window->mouseMoveCallback.push_back(std::bind(&OpenGLExampleBrowserInternalData::MyMouseMoveCallback,
														this, std::placeholders::_1, std::placeholders::_2));
		s_window->mouseMoveCallback.push_back(std::bind(&CommonCameraInterface::mouseMoveCallback, camera, std::placeholders::_1, std::placeholders::_2));

		s_window->mouseButtonCallback.push_back(std::bind(&OpenGLExampleBrowserInternalData::MyMouseButtonCallback,
														  this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
		s_window->mouseButtonCallback.push_back(std::bind(&CommonCameraInterface::mouseButtonCallback, camera, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));

		s_window->keyboardCallback.push_front(std::bind(&OpenGLExampleBrowserInternalData::MyKeyboardCallback,
														this, std::placeholders::_1, std::placeholders::_2));

		args.GetCmdLineArgument("shared_memory_key", gSharedMemoryKey);

		float red, green, blue;
		s_app->getBackgroundColor(&red, &green, &blue);
		args.GetCmdLineArgument("background_color_red", red);
		args.GetCmdLineArgument("background_color_green", green);
		args.GetCmdLineArgument("background_color_blue", blue);
		s_app->setBackgroundColor(red, green, blue);

		b3SetCustomWarningMessageFunc(std::bind(&OpenGLExampleBrowserInternalData::MyGuiPrintf, this, std::placeholders::_1));
		b3SetCustomPrintfFunc(std::bind(&OpenGLExampleBrowserInternalData::MyGuiPrintf, this, std::placeholders::_1));
		b3SetCustomErrorMessageFunc(std::bind(&OpenGLExampleBrowserInternalData::MyStatusBarError, this, std::placeholders::_1));

		assert(glGetError() == GL_NO_ERROR);

		char* demoNameFromCommandOption = 0;
		args.GetCmdLineArgument("start_demo_name", demoNameFromCommandOption);

		int demo_index;
		std::tie(m_gwen, demo_index) = factory(s_app, width, height, s_window->getRetinaScale(),
											   gAllExamples, demoNameFromCommandOption,
											   std::bind(&OpenGLExampleBrowserInternalData::OnButtonB, this),
											   std::bind(&OpenGLExampleBrowserInternalData::OnButtonD, this),
											   std::bind(&OpenGLExampleBrowserInternalData::OnButtonE, this, std::placeholders::_1));
		s_parameterInterface = s_app->m_parameterInterface = m_gwen->CreateCommonParameterInterface();
		s_app->m_2dCanvasInterface = m_gwen->CreateCommon2dCanvasInterface();
		m_gwen->RegisterFileOpen(std::bind(&OpenGLExampleBrowserInternalData::fileOpenCallback, this));
		m_gwen->RegisterQuit(std::bind(&OpenGLExampleBrowserInternalData::quitCallback, this));

		// auto demo_index = setup_gui(width, height, demoNameFromCommandOption);
		if (demo_index >= 0)
		{
			selectDemo(demo_index);
		}
		free(demoNameFromCommandOption);
		demoNameFromCommandOption = 0;
	}

	void update(float deltaTime)
	{
		b3ChromeUtilsEnableProfiling();

		if (!gEnableRenderLoop && !singleStepSimulation)
		{
			B3_PROFILE("updateGraphics");
			sCurrentDemo->updateGraphics();
			return;
		}

		B3_PROFILE("OpenGLExampleBrowser::update");
		//assert(glGetError() == GL_NO_ERROR);
		{
			B3_PROFILE("s_instancingRenderer");
			s_instancingRenderer->init();
		}
		DrawGridData dg;
		dg.upAxis = s_instancingRenderer->getActiveCamera()->getCameraUpAxis();

		{
			BT_PROFILE("Update Camera and Light");

			s_instancingRenderer->updateCamera();
		}

		static int frameCount = 0;
		frameCount++;

		if (gPngFileName)
		{
			static int skip = 0;
			skip--;
			if (skip < 0)
			{
				skip = gPngSkipFrames;
				//printf("gPngFileName=%s\n",gPngFileName);
				static int s_frameCount = 0;

				sprintf(staticPngFileName, "%s%d.png", gPngFileName, s_frameCount++);
				//b3Printf("Made screenshot %s",staticPngFileName);
				s_app->dumpNextFrameToPng(staticPngFileName);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}
		}

		if (sCurrentDemo)
		{
			if (!pauseSimulation || singleStepSimulation)
			{
				//printf("---------------------------------------------------\n");
				//printf("Framecount = %d\n",frameCount);
				B3_PROFILE("sCurrentDemo->stepSimulation");

				if (gFixedTimeStep > 0)
				{
					sCurrentDemo->stepSimulation(gFixedTimeStep);
				}
				else
				{
					sCurrentDemo->stepSimulation(deltaTime);  //1./60.f);
				}
			}

			if (renderGrid)
			{
				BT_PROFILE("Draw Grid");
				//glPolygonOffset(3.0, 3);
				//glEnable(GL_POLYGON_OFFSET_FILL);
				s_app->drawGrid(dg);
			}
			if (renderVisualGeometry && ((gDebugDrawFlags & btIDebugDraw::DBG_DrawWireframe) == 0))
			{
				if (visualWireframe)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				BT_PROFILE("Render Scene");
				if (auto world = sCurrentDemo->getDynamicsWorld())
				{
					s_guiHelper->syncPhysicsToGraphics(world);
					s_guiHelper->render(world);
				}
			}
			else
			{
				B3_PROFILE("physicsDebugDraw");
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				sCurrentDemo->physicsDebugDraw(gDebugDrawFlags);
			}
		}

		{
			if (s_guiHelper && s_guiHelper->getRenderInterface() && s_guiHelper->getRenderInterface()->getActiveCamera())
			{
				B3_PROFILE("setStatusBarMessage");
				char msg[1024];
				float camDist = s_guiHelper->getRenderInterface()->getActiveCamera()->getCameraDistance();
				float pitch = s_guiHelper->getRenderInterface()->getActiveCamera()->getCameraPitch();
				float yaw = s_guiHelper->getRenderInterface()->getActiveCamera()->getCameraYaw();
				float camTarget[3];
				float camPos[3];
				s_guiHelper->getRenderInterface()->getActiveCamera()->getCameraPosition(camPos);
				s_guiHelper->getRenderInterface()->getActiveCamera()->getCameraTargetPosition(camTarget);
				sprintf(msg, "camTargetPos=%2.2f,%2.2f,%2.2f, dist=%2.2f, pitch=%2.2f, yaw=%2.2f", camTarget[0], camTarget[1], camTarget[2], camDist, pitch, yaw);

				if (m_gwen)
				{
					m_gwen->SetStatusbarMessage(msg);
				}
			}
		}

		static int toggle = 1;
		if (renderGui)
		{
			B3_PROFILE("renderGui");
#ifndef BT_NO_PROFILE

			if (!pauseSimulation || singleStepSimulation)
			{
				if (isProfileWindowVisible(s_profWindow))
				{
					processProfileData(s_profWindow, false);
				}
			}
#endif  //#ifndef BT_NO_PROFILE

			{
				B3_PROFILE("updateOpenGL");

				if (m_gwen)
				{
					gBlockGuiMessages = true;
					auto camera = s_instancingRenderer->getActiveCamera();
					m_gwen->Render(camera->getScreenWidth(), camera->getScreenHeight());
					gBlockGuiMessages = false;
				}
			}
		}

		singleStepSimulation = false;

		toggle = 1 - toggle;
		{
			BT_PROFILE("Sync Parameters");
			if (s_parameterInterface)
			{
				s_parameterInterface->syncParameters();
			}
		}
		{
			BT_PROFILE("Swap Buffers");
			s_app->swapBuffer();
		}

		if (m_gwen)
		{
			B3_PROFILE("forceUpdateScrollBars");
			m_gwen->ForceUpdateScrollBars();
		}
	}
};

void OpenGLExampleBrowser::registerFileImporter(const char* extension, const CommonExampleInterface::CreateFunc& createFunc)
{
	FileImporterByExtension fi;
	fi.m_extension = extension;
	fi.m_createFunc = createFunc;
	m_internalData->gFileImporterByExtension.push_back(fi);
}

OpenGLExampleBrowser::OpenGLExampleBrowser(class ExampleEntries* examples)
{
	m_internalData = new OpenGLExampleBrowserInternalData(examples);
}

OpenGLExampleBrowser::~OpenGLExampleBrowser()
{
	delete m_internalData;
	m_internalData = nullptr;
}

bool OpenGLExampleBrowser::init(int argc, char* argv[], const CommonGUIInterface::Factory& factory)
{
	m_internalData->init(factory, argc, argv);
	return true;
}

CommonExampleInterface* OpenGLExampleBrowser::getCurrentExample()
{
	btAssert(m_internalData->sCurrentDemo);
	return m_internalData->sCurrentDemo;
}

bool OpenGLExampleBrowser::requestedExit()
{
	return m_internalData->s_window->requestedExit();
}

void OpenGLExampleBrowser::updateGraphics()
{
	if (m_internalData->sCurrentDemo)
	{
		if (!m_internalData->pauseSimulation || m_internalData->singleStepSimulation)
		{
			//B3_PROFILE("sCurrentDemo->updateGraphics");
			m_internalData->sCurrentDemo->updateGraphics();
		}
	}
}

void OpenGLExampleBrowser::update(float deltaTime)
{
	m_internalData->update(deltaTime);
}

void OpenGLExampleBrowser::setSharedMemoryInterface(class SharedMemoryInterface* sharedMem)
{
	m_internalData->gDisableDemoSelection = true;
	m_internalData->sSharedMem = sharedMem;
}
