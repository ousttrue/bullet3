#include "OpenGLExampleBrowser.h"
#include "LinearMath/btQuickprof.h"
#include <OpenGLInclude.h>
#include <SimpleOpenGL2App.h>
#include <functional>
#ifndef NO_OPENGL3
#include <SimpleOpenGL3App.h>
#endif
#include <CommonRenderInterface.h>
#ifdef __APPLE__
#include <MacOpenGLWindow.h>
#else
#ifdef _WIN32
#include <Win32OpenGLWindow.h>
#else
//let's cross the fingers it is Linux/X11
#ifdef BT_USE_EGL
#include <EGLOpenGLWindow.h>
#else
#include <X11OpenGLWindow.h>
#endif  //BT_USE_EGL
#endif  //_WIN32
#endif  //__APPLE__
// #include <Gwen/Renderers/OpenGL_DebugFont.h>
#include "LinearMath/btThreads.h"
#include "Bullet3Common/b3Vector3.h"
#include "assert.h"
#include <stdio.h>
#include <gwenInternalData.h>
#include <gwenUserInterface.h>
#include <b3Clock.h>
#include <ChromeTraceUtil.h>
#include <GwenParameterInterface.h>
#ifndef BT_NO_PROFILE
#include "GwenGUISupport/GwenProfileWindow.h"
#endif
#include <GwenTextureWindow.h>
#include <GraphingTexture.h>
#include <Common2dCanvasInterface.h>
#include <CommonExampleInterface.h>
#include "Bullet3Common/b3CommandLineArgs.h"
#include <SimpleCamera.h>
#include <SimpleOpenGL2Renderer.h>
#include "ExampleEntries.h"
#include "OpenGLGuiHelper.h"
#include "Bullet3Common/b3FileUtils.h"

#include "LinearMath/btIDebugDraw.h"
//quick test for file import, @todo(erwincoumans) make it more general and add other file formats
#include "../Importers/ImportURDFDemo/ImportURDFSetup.h"
#include "../Importers/ImportBullet/SerializeSetup.h"
#include "../SharedMemory/SharedMemoryPublic.h"
#include "Bullet3Common/b3HashMap.h"
#include "EmptyExample.h"
#include <string.h>
#include <stdio.h>
#define MAX_GRAPH_WINDOWS 5

#ifndef USE_OPENGL3
extern bool useShadowMap;
#endif
extern bool gDisableDeactivation;
int gSharedMemoryKey = -1;


struct GL3TexLoader : public MyTextureLoader
{
	b3HashMap<b3HashString, GLint> m_hashMap;

	virtual void LoadTexture(Gwen::Texture* pTexture)
	{
		Gwen::String namestr = pTexture->name.Get();
		const char* n = namestr.c_str();
		GLint* texIdPtr = m_hashMap[n];
		if (texIdPtr)
		{
			pTexture->m_intData = *texIdPtr;
		}
	}
	virtual void FreeTexture(Gwen::Texture* pTexture)
	{
	}
};

struct FileImporterByExtension
{
	std::string m_extension;
	CommonExampleInterface::CreateFunc* m_createFunc;
};

using OnButton = std::function<void()>;
struct MyMenuItemHander : public Gwen::Event::Handler
{
	int m_buttonId;

	OnButton m_onButtonB;
	OnButton m_onButtonD;
	std::function<void(int)> m_onButtonE;

	MyMenuItemHander(int buttonId,
					 const OnButton& onButtonB,
					 const OnButton& onButtonD,
					 const std::function<void(int)>& onButtonE)
		: m_buttonId(buttonId), m_onButtonB(onButtonB), m_onButtonD(onButtonD), m_onButtonE(onButtonE)
	{
	}

	void onButtonA(Gwen::Controls::Base* pControl)
	{
		//const Gwen::String& name = pControl->GetName();
		Gwen::Controls::TreeNode* node = (Gwen::Controls::TreeNode*)pControl;
		//	Gwen::Controls::Label* l = node->GetButton();

		Gwen::UnicodeString la = node->GetButton()->GetText();  // node->GetButton()->GetName();// GetText();
		Gwen::String laa = Gwen::Utility::UnicodeToString(la);
		//	const char* ha = laa.c_str();

		//printf("selected %s\n", ha);
		//int dep = but->IsDepressed();
		//int tog = but->GetToggleState();
		//		if (m_data->m_toggleButtonCallback)
		//		(*m_data->m_toggleButtonCallback)(m_buttonId, tog);
	}

	void onButtonB(Gwen::Controls::Base* pControl)
	{
		Gwen::Controls::Label* label = (Gwen::Controls::Label*)pControl;
		Gwen::UnicodeString la = label->GetText();  // node->GetButton()->GetName();// GetText();
		Gwen::String laa = Gwen::Utility::UnicodeToString(la);
		//const char* ha = laa.c_str();

		m_onButtonB();
	}

	void onButtonC(Gwen::Controls::Base* pControl)
	{
		/*Gwen::Controls::Label* label = (Gwen::Controls::Label*) pControl;
		Gwen::UnicodeString la = label->GetText();// node->GetButton()->GetName();// GetText();
		Gwen::String laa = Gwen::Utility::UnicodeToString(la);
		const char* ha = laa.c_str();


		printf("onButtonC ! %s\n", ha);
		*/
	}
	void onButtonD(Gwen::Controls::Base* pControl)
	{
		/*		Gwen::Controls::Label* label = (Gwen::Controls::Label*) pControl;
		Gwen::UnicodeString la = label->GetText();// node->GetButton()->GetName();// GetText();
		Gwen::String laa = Gwen::Utility::UnicodeToString(la);
		const char* ha = laa.c_str();
		*/

		//	printf("onKeyReturn ! \n");
		m_onButtonD();
	}

	void onButtonE(Gwen::Controls::Base* pControl)
	{
		//	printf("select %d\n",m_buttonId);
		m_onButtonE(m_buttonId);
	}

	void onButtonF(Gwen::Controls::Base* pControl)
	{
		//printf("selection changed!\n");
	}

	void onButtonG(Gwen::Controls::Base* pControl)
	{
		//printf("onButtonG !\n");
	}
};

struct QuickCanvas : public Common2dCanvasInterface
{
	GL3TexLoader* m_myTexLoader;

	MyGraphWindow* m_gw[MAX_GRAPH_WINDOWS];
	GraphingTexture* m_gt[MAX_GRAPH_WINDOWS];
	int m_curNumGraphWindows;

	QuickCanvas(GL3TexLoader* myTexLoader)
		: m_myTexLoader(myTexLoader),
		  m_curNumGraphWindows(0)
	{
		for (int i = 0; i < MAX_GRAPH_WINDOWS; i++)
		{
			m_gw[i] = 0;
			m_gt[i] = 0;
		}
	}
	virtual ~QuickCanvas() {}
	virtual int createCanvas(const char* canvasName, int width, int height, int xPos, int yPos);
	virtual void destroyCanvas(int canvasId)
	{
		btAssert(canvasId >= 0);
		delete m_gt[canvasId];
		m_gt[canvasId] = 0;
		destroyTextureWindow(m_gw[canvasId]);
		m_gw[canvasId] = 0;
		m_curNumGraphWindows--;
	}
	virtual void setPixel(int canvasId, int x, int y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		btAssert(canvasId >= 0);
		btAssert(canvasId < m_curNumGraphWindows);
		m_gt[canvasId]->setPixel(x, y, red, green, blue, alpha);
	}

	virtual void getPixel(int canvasId, int x, int y, unsigned char& red, unsigned char& green, unsigned char& blue, unsigned char& alpha)
	{
		btAssert(canvasId >= 0);
		btAssert(canvasId < m_curNumGraphWindows);
		m_gt[canvasId]->getPixel(x, y, red, green, blue, alpha);
	}

	virtual void refreshImageData(int canvasId)
	{
		m_gt[canvasId]->uploadImageData();
	}
};

struct OpenGLExampleBrowserInternalData
{
	Gwen::Renderer::Base* m_gwenRenderer = nullptr;
	btAlignedObjectArray<Gwen::Controls::TreeNode*> m_nodes;
	GwenUserInterface* m_gui = nullptr;
	GL3TexLoader* m_myTexLoader = nullptr;
	struct MyMenuItemHander* m_handler2 = nullptr;
	btAlignedObjectArray<MyMenuItemHander*> m_handlers;

	CommonGraphicsApp* s_app = 0;

	CommonWindowInterface* s_window = 0;
	CommonParameterInterface* s_parameterInterface = 0;
	CommonRenderInterface* s_instancingRenderer = 0;
	OpenGLGuiHelper* s_guiHelper = 0;
#ifndef BT_NO_PROFILE
	MyProfileWindow* s_profWindow = 0;
#endif  //BT_NO_PROFILE
	SharedMemoryInterface* sSharedMem = 0;

#define DEMO_SELECTION_COMBOBOX 13
	const char* startFileName = "0_Bullet3Demo.txt";
	char staticPngFileName[1024];
	//GwenUserInterface* gui  = 0;
	GwenUserInterface* gui2 = 0;
	int sCurrentDemoIndex = -1;
	int sCurrentHightlighted = 0;
	CommonExampleInterface* sCurrentDemo = 0;
	b3AlignedObjectArray<const char*> allNames;
	float gFixedTimeStep = 0;
	bool gAllowRetina = true;
	bool gDisableDemoSelection = false;
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
	bool pauseSimulation = false;
	bool singleStepSimulation = false;
	int midiBaseIndex = 176;

	///some quick test variable for the OpenCL examples

	int gPreferredOpenCLDeviceIndex = -1;
	int gPreferredOpenCLPlatformIndex = -1;
	int gGpuArraySizeX = 45;
	int gGpuArraySizeY = 55;
	int gGpuArraySizeZ = 45;

	const char* gPngFileName = 0;
	int gPngSkipFrames = 0;
	b3KeyboardCallback prevKeyboardCallback = 0;
	b3MouseMoveCallback prevMouseMoveCallback = 0;
	b3MouseButtonCallback prevMouseButtonCallback = 0;

	btAlignedObjectArray<FileImporterByExtension> gFileImporterByExtension;

	//in case of multi-threading, don't submit messages while the GUI is rendering (causing crashes)
	bool gBlockGuiMessages = false;

	OpenGLExampleBrowserInternalData(ExampleEntries* examples)
		: gAllExamples(examples)
	{
	}

	~OpenGLExampleBrowserInternalData()
	{
		deleteDemo();
		for (int i = 0; i < m_nodes.size(); i++)
		{
			delete m_nodes[i];
		}
		delete m_handler2;
		for (int i = 0; i < m_handlers.size(); i++)
		{
			delete m_handlers[i];
		}
		m_handlers.clear();
		m_nodes.clear();
		delete s_parameterInterface;
		s_parameterInterface = 0;
		delete s_app->m_2dCanvasInterface;
		s_app->m_2dCanvasInterface = 0;

#ifndef BT_NO_PROFILE
		destroyProfileWindow(m_profWindow);
#endif

		m_gui->exit();

		delete m_gui;
		delete m_gwenRenderer;
		delete m_myTexLoader;

		delete s_app;
		s_app = 0;

		gFileImporterByExtension.clear();
		gAllExamples = 0;
	}

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

			//		CProfileManager::CleanupMemory();
		}
	}

	void MyKeyboardCallback(int key, int state)
	{
		//b3Printf("key=%d, state=%d", key, state);
		bool handled = false;
		if (renderGui)
		{
			if (gui2 && !handled)
			{
				handled = gui2->keyboardCallback(key, state);
			}
		}

		if (!handled && sCurrentDemo)
		{
			handled = sCurrentDemo->keyboardCallback(key, state);
		}

		//checkout: is it desired to ignore keys, if the demo already handles them?
		//if (handled)
		//	return;

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

		if (prevKeyboardCallback)
			prevKeyboardCallback(key, state);
	}

	void MyMouseMoveCallback(float x, float y)
	{
		bool handled = false;
		if (sCurrentDemo)
			handled = sCurrentDemo->mouseMoveCallback(x, y);
		if (renderGui)
		{
			if (!handled && gui2)
				handled = gui2->mouseMoveCallback(x, y);
		}
		if (!handled)
		{
			if (prevMouseMoveCallback)
				prevMouseMoveCallback(x, y);
		}
	}

	void MyMouseButtonCallback(int button, int state, float x, float y)
	{
		bool handled = false;
		//try picking first
		if (sCurrentDemo)
			handled = sCurrentDemo->mouseButtonCallback(button, state, x, y);

		if (renderGui)
		{
			if (!handled && gui2)
				handled = gui2->mouseButtonCallback(button, state, x, y);
		}
		if (!handled)
		{
			if (prevMouseButtonCallback)
				prevMouseButtonCallback(button, state, x, y);
		}
		//	b3DefaultMouseButtonCallback(button,state,x,y);
	}

	void OpenGLExampleBrowserVisualizerFlagCallback(int flag, bool enable)
	{
		if (flag == COV_ENABLE_Y_AXIS_UP)
		{
			//either Y = up or Z
			int upAxis = enable ? 1 : 2;
			s_app->setUpAxis(upAxis);
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
			sCurrentDemo->initPhysics();
			sCurrentDemo->resetCamera();
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

		CommonExampleInterface::CreateFunc* func = gAllExamples->getExampleCreateFunc(demoIndex);
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
			sCurrentDemo = (*func)(options);
			if (sCurrentDemo)
			{
				if (gui2)
				{
					gui2->setStatusBarMessage("Status: OK", false);
				}
				b3Printf("Selected demo: %s", gAllExamples->getExampleName(demoIndex));
				if (gui2)
				{
					gui2->setExampleDescription(gAllExamples->getExampleDescription(demoIndex));
				}

				sCurrentDemo->initPhysics();
				if (resetCamera)
				{
					sCurrentDemo->resetCamera();
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
			fprintf(f, "--mouse_move_multiplier=%f\n", s_app->getMouseMoveMultiplier());
			fprintf(f, "--mouse_wheel_multiplier=%f\n", s_app->getMouseWheelMultiplier());
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
			gui2->textOutput(msg);
			gui2->forceUpdateScrollBars();
		}
	}

	void MyStatusBarPrintf(const char* msg)
	{
		printf("b3Printf: %s\n", msg);
		if (!gDisableDemoSelection && !gBlockGuiMessages)
		{
			bool isLeft = true;
			gui2->setStatusBarMessage(msg, isLeft);
		}
	}

	void MyStatusBarError(const char* msg)
	{
		printf("Warning: %s\n", msg);
		if (!gDisableDemoSelection && !gBlockGuiMessages)
		{
			bool isLeft = false;
			gui2->setStatusBarMessage(msg, isLeft);
			gui2->textOutput(msg);
			gui2->forceUpdateScrollBars();
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
		gui2->setExampleDescription(gAllExamples->getExampleDescription(sCurrentHightlighted));
	}

	void init(int argc, char** argv)
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

		SimpleOpenGL3App* simpleApp = 0;
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
		simpleApp = new SimpleOpenGL3App(title, width, height, gAllowRetina, gWindowBackend, gRenderDevice);
		s_app = simpleApp;

		char* gVideoFileName = 0;
		args.GetCmdLineArgument("mp4", gVideoFileName);
		int gVideoFps = 0;
		args.GetCmdLineArgument("mp4fps", gVideoFps);
		if (gVideoFps)
		{
			simpleApp->setMp4Fps(gVideoFps);
		}

#ifndef NO_OPENGL3
		if (gVideoFileName)
			simpleApp->dumpFramesToVideo(gVideoFileName);
#endif

		s_instancingRenderer = s_app->m_renderer;
		s_window = s_app->m_window;

		width = s_window->getWidth();
		height = s_window->getHeight();

		prevMouseMoveCallback = s_window->getMouseMoveCallback();
		s_window->setMouseMoveCallback(std::bind(&OpenGLExampleBrowserInternalData::MyMouseMoveCallback,
												 this, std::placeholders::_1, std::placeholders::_2));

		prevMouseButtonCallback = s_window->getMouseButtonCallback();
		s_window->setMouseButtonCallback(std::bind(&OpenGLExampleBrowserInternalData::MyMouseButtonCallback,
												   this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
		prevKeyboardCallback = s_window->getKeyboardCallback();
		s_window->setKeyboardCallback(std::bind(&OpenGLExampleBrowserInternalData::MyKeyboardCallback,
												this, std::placeholders::_1, std::placeholders::_2));

		s_app->m_renderer->getActiveCamera()->setCameraDistance(13);
		s_app->m_renderer->getActiveCamera()->setCameraPitch(0);
		s_app->m_renderer->getActiveCamera()->setCameraTargetPosition(0, 0, 0);

		float mouseMoveMult = s_app->getMouseMoveMultiplier();
		if (args.GetCmdLineArgument("mouse_move_multiplier", mouseMoveMult))
		{
			s_app->setMouseMoveMultiplier(mouseMoveMult);
		}

		float mouseWheelMult = s_app->getMouseWheelMultiplier();
		if (args.GetCmdLineArgument("mouse_wheel_multiplier", mouseWheelMult))
		{
			s_app->setMouseWheelMultiplier(mouseWheelMult);
		}

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

		{
			m_myTexLoader = new GL3TexLoader;

			sth_stash* fontstash = simpleApp->getFontStash();
			m_gwenRenderer = new GwenOpenGL3CoreRenderer(simpleApp->m_primRenderer, fontstash, width, height, s_window->getRetinaScale(), m_myTexLoader);

			gui2 = new GwenUserInterface;

			m_gui = gui2;

			gui2->init(width, height, m_gwenRenderer, s_window->getRetinaScale());
		}

		if (gui2)
		{
			//	gui->getInternalData()->m_explorerPage
			Gwen::Controls::TreeControl* tree = gui2->getInternalData()->m_explorerTreeCtrl;

			//gui->getInternalData()->pRenderer->setTextureLoader(myTexLoader);

#ifndef BT_NO_PROFILE
			s_profWindow = setupProfileWindow(gui2->getInternalData());
			m_internalData->m_profWindow = s_profWindow;
			profileWindowSetVisible(s_profWindow, false);
#endif  //BT_NO_PROFILE
			gui2->setFocus();

			s_parameterInterface = s_app->m_parameterInterface = new GwenParameterInterface(gui2->getInternalData());
			s_app->m_2dCanvasInterface = new QuickCanvas(m_myTexLoader);

			///add some demos to the gAllExamples

			int numDemos = gAllExamples->getNumRegisteredExamples();

			//char nodeText[1024];
			//int curDemo = 0;
			int selectedDemo = 0;
			Gwen::Controls::TreeNode* curNode = tree;
			m_handler2 = new MyMenuItemHander(-1,
											  std::bind(&OpenGLExampleBrowserInternalData::OnButtonB, this),
											  std::bind(&OpenGLExampleBrowserInternalData::OnButtonD, this),
											  std::bind(&OpenGLExampleBrowserInternalData::OnButtonE, this, std::placeholders::_1));

			char* demoNameFromCommandOption = 0;
			args.GetCmdLineArgument("start_demo_name", demoNameFromCommandOption);
			if (demoNameFromCommandOption)
			{
				selectedDemo = -1;
			}

			tree->onReturnKeyDown.Add(m_handler2, &MyMenuItemHander::onButtonD);
			int firstAvailableDemoIndex = -1;
			Gwen::Controls::TreeNode* firstNode = 0;

			for (int d = 0; d < numDemos; d++)
			{
				//		sprintf(nodeText, "Node %d", i);
				Gwen::UnicodeString nodeUText = Gwen::Utility::StringToUnicode(gAllExamples->getExampleName(d));
				if (gAllExamples->getExampleCreateFunc(d))  //was test for gAllExamples[d].m_menuLevel==1
				{
					Gwen::Controls::TreeNode* pNode = curNode->AddNode(nodeUText);

					if (firstAvailableDemoIndex < 0)
					{
						firstAvailableDemoIndex = d;
						firstNode = pNode;
					}

					if (d == selectedDemo)
					{
						firstAvailableDemoIndex = d;
						firstNode = pNode;
						//pNode->SetSelected(true);
						//tree->ExpandAll();
						//	tree->ForceUpdateScrollBars();
						//tree->OnKeyLeft(true);
						//	tree->OnKeyRight(true);

						//tree->ExpandAll();

						//	selectDemo(d);
					}

					if (demoNameFromCommandOption)
					{
						const char* demoName = gAllExamples->getExampleName(d);
						int res = strcmp(demoName, demoNameFromCommandOption);
						if (res == 0)
						{
							firstAvailableDemoIndex = d;
							firstNode = pNode;
						}
					}

#if 1
					MyMenuItemHander* handler = new MyMenuItemHander(d,
																	 std::bind(&OpenGLExampleBrowserInternalData::OnButtonB, this),
																	 std::bind(&OpenGLExampleBrowserInternalData::OnButtonD, this),
																	 std::bind(&OpenGLExampleBrowserInternalData::OnButtonE, this, std::placeholders::_1));
					m_handlers.push_back(handler);

					pNode->onNamePress.Add(handler, &MyMenuItemHander::onButtonA);
					pNode->GetButton()->onDoubleClick.Add(handler, &MyMenuItemHander::onButtonB);
					pNode->GetButton()->onDown.Add(handler, &MyMenuItemHander::onButtonC);
					pNode->onSelect.Add(handler, &MyMenuItemHander::onButtonE);
					pNode->onReturnKeyDown.Add(handler, &MyMenuItemHander::onButtonG);
					pNode->onSelectChange.Add(handler, &MyMenuItemHander::onButtonF);

#endif
					//			pNode->onKeyReturn.Add(handler, &MyMenuItemHander::onButtonD);
					//			pNode->GetButton()->onKeyboardReturn.Add(handler, &MyMenuItemHander::onButtonD);
					//		pNode->onNamePress.Add(handler, &MyMenuItemHander::onButtonD);
					//			pNode->onKeyboardPressed.Add(handler, &MyMenuItemHander::onButtonD);
					//			pNode->OnKeyPress
				}
				else
				{
					curNode = tree->AddNode(nodeUText);
					m_nodes.push_back(curNode);
				}
			}

			if (sCurrentDemo == 0)
			{
				if (firstAvailableDemoIndex >= 0)
				{
					firstNode->SetSelected(true);
					while (firstNode != tree)
					{
						firstNode->ExpandAll();
						firstNode = (Gwen::Controls::TreeNode*)firstNode->GetParent();
					}

					selectDemo(firstAvailableDemoIndex);
				}
			}
			free(demoNameFromCommandOption);
			demoNameFromCommandOption = 0;

			btAssert(sCurrentDemo != 0);
			if (sCurrentDemo == 0)
			{
				printf("Error, no demo/example\n");
				exit(0);
			}

			gui2->registerFileOpenCallback(std::bind(&OpenGLExampleBrowserInternalData::fileOpenCallback, this));
			gui2->registerQuitCallback(std::bind(&OpenGLExampleBrowserInternalData::quitCallback, this));
		}
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
		dg.upAxis = s_app->getUpAxis();

		{
			BT_PROFILE("Update Camera and Light");

			s_instancingRenderer->updateCamera(dg.upAxis);
		}

		static int frameCount = 0;
		frameCount++;

		if (0)
		{
			BT_PROFILE("Draw frame counter");
			char bla[1024];
			sprintf(bla, "Frame %d", frameCount);
			s_app->drawText(bla, 10, 10);
		}

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
				sCurrentDemo->renderScene();
			}
			else
			{
				B3_PROFILE("physicsDebugDraw");
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				sCurrentDemo->physicsDebugDraw(gDebugDrawFlags);
			}
		}

		{
			if (gui2 && s_guiHelper && s_guiHelper->getRenderInterface() && s_guiHelper->getRenderInterface()->getActiveCamera())
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
				gui2->setStatusBarMessage(msg, true);
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

				if (m_gui)
				{
					gBlockGuiMessages = true;
					m_gui->draw(s_instancingRenderer->getScreenWidth(), s_instancingRenderer->getScreenHeight());

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

		if (gui2)
		{
			B3_PROFILE("forceUpdateScrollBars");
			gui2->forceUpdateScrollBars();
		}
	}
};
static OpenGLExampleBrowserInternalData* s_internalData = nullptr;

int QuickCanvas::createCanvas(const char* canvasName, int width, int height, int xPos, int yPos)
{
	if (m_curNumGraphWindows < MAX_GRAPH_WINDOWS)
	{
		//find a slot
		int slot = m_curNumGraphWindows;
		btAssert(slot < MAX_GRAPH_WINDOWS);
		if (slot >= MAX_GRAPH_WINDOWS)
			return 0;  //don't crash

		m_curNumGraphWindows++;

		MyGraphInput input(s_internalData->gui2->getInternalData());
		input.m_width = width;
		input.m_height = height;
		input.m_xPos = xPos;
		input.m_yPos = yPos;
		input.m_name = canvasName;
		input.m_texName = canvasName;
		m_gt[slot] = new GraphingTexture;
		m_gt[slot]->create(width, height);
		int texId = m_gt[slot]->getTextureId();
		m_myTexLoader->m_hashMap.insert(canvasName, texId);
		m_gw[slot] = setupTextureWindow(input);

		return slot;
	}
	return -1;
}

void OpenGLExampleBrowser::registerFileImporter(const char* extension, CommonExampleInterface::CreateFunc* createFunc)
{
	FileImporterByExtension fi;
	fi.m_extension = extension;
	fi.m_createFunc = createFunc;
	s_internalData->gFileImporterByExtension.push_back(fi);
}

OpenGLExampleBrowser::OpenGLExampleBrowser(class ExampleEntries* examples)
{
	s_internalData = new OpenGLExampleBrowserInternalData(examples);
}

OpenGLExampleBrowser::~OpenGLExampleBrowser()
{
	delete s_internalData;
	s_internalData = nullptr;
}

bool OpenGLExampleBrowser::init(int argc, char* argv[])
{
	s_internalData->init(argc, argv);
	return true;
}

CommonExampleInterface* OpenGLExampleBrowser::getCurrentExample()
{
	btAssert(s_internalData->sCurrentDemo);
	return s_internalData->sCurrentDemo;
}

bool OpenGLExampleBrowser::requestedExit()
{
	return s_internalData->s_window->requestedExit();
}

void OpenGLExampleBrowser::updateGraphics()
{
	if (s_internalData->sCurrentDemo)
	{
		if (!s_internalData->pauseSimulation || s_internalData->singleStepSimulation)
		{
			//B3_PROFILE("sCurrentDemo->updateGraphics");
			s_internalData->sCurrentDemo->updateGraphics();
		}
	}
}

void OpenGLExampleBrowser::update(float deltaTime)
{
	s_internalData->update(deltaTime);
}

void OpenGLExampleBrowser::setSharedMemoryInterface(class SharedMemoryInterface* sharedMem)
{
	s_internalData->gDisableDemoSelection = true;
	s_internalData->sSharedMem = sharedMem;
}
