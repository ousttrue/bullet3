#include <LinearMath/btHashMap.h>
#include <Bullet3Common/b3HashMap.h>
#include "CommonGUIInterface.h"
#include "Gwen/Controls/TreeNode.h"
#include "GwenImpl.h"
#include "GwenOpenGL3CoreRenderer.h"
#include <Common2dCanvasInterface.h>
#include <CommonExampleInterface.h>
#include <OpenGLInclude.h>
#include <gwenInternalData.h>
#include <gwenUserInterface.h>
#include <GwenParameterInterface.h>
#include <GwenTextureWindow.h>
#include <GraphingTexture.h>
#include <GlfwApp.h>

#define MAX_GRAPH_WINDOWS 5

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
		// Gwen::Controls::Label* l = node->GetButton();

		Gwen::UnicodeString la = node->GetButton()->GetText();  // node->GetButton()->GetName();// GetText();
		Gwen::String laa = Gwen::Utility::UnicodeToString(la);
		// const char* ha = laa.c_str();

		//printf("selected %s\n", ha);
		//int dep = but->IsDepressed();
		//int tog = but->GetToggleState();
		//  if (m_data->m_toggleButtonCallback)
		//  (*m_data->m_toggleButtonCallback)(m_buttonId, tog);
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
		/*  Gwen::Controls::Label* label = (Gwen::Controls::Label*) pControl;
        Gwen::UnicodeString la = label->GetText();// node->GetButton()->GetName();// GetText();
        Gwen::String laa = Gwen::Utility::UnicodeToString(la);
        const char* ha = laa.c_str();
        */

		// printf("onKeyReturn ! \n");
		m_onButtonD();
	}

	void onButtonE(Gwen::Controls::Base* pControl)
	{
		// printf("select %d\n",m_buttonId);
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
	GwenUserInterface* m_internalData;

	GL3TexLoader* m_myTexLoader;

	MyGraphWindow* m_gw[MAX_GRAPH_WINDOWS];
	GraphingTexture* m_gt[MAX_GRAPH_WINDOWS];
	int m_curNumGraphWindows;

	QuickCanvas(GwenUserInterface* internalData, GL3TexLoader* myTexLoader)
		: m_internalData(internalData),
		  m_myTexLoader(myTexLoader),
		  m_curNumGraphWindows(0)
	{
		for (int i = 0; i < MAX_GRAPH_WINDOWS; i++)
		{
			m_gw[i] = 0;
			m_gt[i] = 0;
		}
	}
	virtual ~QuickCanvas() {}
	virtual int createCanvas(const char* canvasName, int width, int height, int xPos, int yPos)
	{
		if (m_curNumGraphWindows < MAX_GRAPH_WINDOWS)
		{
			//find a slot
			int slot = m_curNumGraphWindows;
			btAssert(slot < MAX_GRAPH_WINDOWS);
			if (slot >= MAX_GRAPH_WINDOWS)
				return 0;  //don't crash

			m_curNumGraphWindows++;

			MyGraphInput input(m_internalData->getInternalData());
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

///
/// GwenImpl
///
GwenImpl::GwenImpl(GlfwApp* s_app, int width, int height, float retinaScale)
{
	m_myTexLoader = new GL3TexLoader;

	auto fontstash = s_app->getFontStash();
	m_gwenRenderer = new GwenOpenGL3CoreRenderer(s_app->m_primRenderer, fontstash, width, height, retinaScale, m_myTexLoader);
	m_gui = new GwenUserInterface;
	m_gui->init(width, height, m_gwenRenderer, retinaScale);
}

GwenImpl::~GwenImpl()
{
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
	m_gui->exit();
	delete m_gui;
	delete m_gwenRenderer;
	delete m_myTexLoader;
}

std::tuple<CommonGUIInterface*, int> GwenImpl::Create(GlfwApp* app, int width, int height, float reginaScale, ExampleEntries* gAllExamples, const char* demoNameFromCommandOption,
											const std::function<void()>& onB, const std::function<void()>& onD, const std::function<void(int)>& _onE)
{
	auto m_gwen = new GwenImpl(app, width, height, reginaScale);

	auto onE = [_onE, m_gwen, gAllExamples](int id)
	{
		_onE(id);
		auto desc = gAllExamples->getExampleDescription(id);
		m_gwen->SetDescription(desc);
	};

	// m_gwen->Setup(onB, onD, onE);
	// gui->getInternalData()->m_explorerPage
	auto tree = m_gwen->m_gui->getInternalData()->m_explorerTreeCtrl;

	//gui->getInternalData()->pRenderer->setTextureLoader(myTexLoader);

	// #ifndef BT_NO_PROFILE
	// 	s_profWindow = setupProfileWindow(m_gui->getInternalData());
	// 	m_internalData->m_profWindow = s_profWindow;
	// 	profileWindowSetVisible(s_profWindow, false);
	// #endif  //BT_NO_PROFILE
	m_gwen->m_gui->setFocus();

	///add some demos to the gAllExamples

	//char nodeText[1024];
	//int curDemo = 0;
	m_gwen->m_handler2 = new MyMenuItemHander(-1, onB, onD, _onE);

	auto curNode = (Gwen::Controls::TreeNode*)tree;
	tree->onReturnKeyDown.Add(m_gwen->m_handler2, &MyMenuItemHander::onButtonD);

	int numDemos = gAllExamples->getNumRegisteredExamples();
	int selectedDemo = 0;
	if (demoNameFromCommandOption)
	{
		selectedDemo = -1;
	}
	int firstAvailableDemoIndex = -1;
	Gwen::Controls::TreeNode* firstNode = 0;
	for (int d = 0; d < numDemos; d++)
	{
		if (demoNameFromCommandOption)
		{
			const char* demoName = gAllExamples->getExampleName(d);
			int res = strcmp(demoName, demoNameFromCommandOption);
			if (res == 0)
			{
				selectedDemo = d;
			}
		}
		// bool selected = m_gwen->AddDemo(d, gAllExamples->getExampleName(d), gAllExamples->getExampleCreateFunc(d), selectedDemo,
		// 								onB, onD, onE);
		bool isSelected = false;

		//  sprintf(nodeText, "Node %d", i);
		Gwen::UnicodeString nodeUText = Gwen::Utility::StringToUnicode(gAllExamples->getExampleName(d));
		auto f = gAllExamples->getExampleCreateFunc(d);
		if (f)  //was test for gAllExamples[d].m_menuLevel==1
		{
			Gwen::Controls::TreeNode* pNode = curNode->AddNode(nodeUText);

			if (!firstNode)
			{
				firstNode = pNode;
				isSelected = true;
			}

			if (d == selectedDemo)
			{
				firstNode = pNode;
				isSelected = true;
				//pNode->SetSelected(true);
				//tree->ExpandAll();
				// tree->ForceUpdateScrollBars();
				//tree->OnKeyLeft(true);
				// tree->OnKeyRight(true);

				//tree->ExpandAll();

				// selectDemo(d);
			}

#if 1
			MyMenuItemHander* handler = new MyMenuItemHander(d,
															 onB,
															 onD,
															 onE);
			m_gwen->m_handlers.push_back(handler);

			pNode->onNamePress.Add(handler, &MyMenuItemHander::onButtonA);
			pNode->GetButton()->onDoubleClick.Add(handler, &MyMenuItemHander::onButtonB);
			pNode->GetButton()->onDown.Add(handler, &MyMenuItemHander::onButtonC);
			pNode->onSelect.Add(handler, &MyMenuItemHander::onButtonE);
			pNode->onReturnKeyDown.Add(handler, &MyMenuItemHander::onButtonG);
			pNode->onSelectChange.Add(handler, &MyMenuItemHander::onButtonF);

#endif
			//   pNode->onKeyReturn.Add(handler, &MyMenuItemHander::onButtonD);
			//   pNode->GetButton()->onKeyboardReturn.Add(handler, &MyMenuItemHander::onButtonD);
			//  pNode->onNamePress.Add(handler, &MyMenuItemHander::onButtonD);
			//   pNode->onKeyboardPressed.Add(handler, &MyMenuItemHander::onButtonD);
			//   pNode->OnKeyPress
		}
		else
		{
			curNode = tree->AddNode(nodeUText);
			m_gwen->m_nodes.push_back(curNode);
		}

		if (isSelected)
		{
			firstAvailableDemoIndex = d;
		}
	}

	// if (sCurrentDemo == 0)
	{
		if (firstAvailableDemoIndex >= 0)
		{
			// m_gwen->ExpandSelected();
			firstNode->SetSelected(true);
			while (firstNode != tree)
			{
				firstNode->ExpandAll();
				firstNode = (Gwen::Controls::TreeNode*)firstNode->GetParent();
			}
		}
	}

	// btAssert(sCurrentDemo != 0);
	// if (sCurrentDemo == 0)
	// {
	// 	printf("Error, no demo/example\n");
	// 	exit(0);
	// }

	return {m_gwen, firstAvailableDemoIndex};
}

CommonParameterInterface* GwenImpl::CreateCommonParameterInterface()
{
	return new GwenParameterInterface(m_gui->getInternalData());
}
Common2dCanvasInterface* GwenImpl::CreateCommon2dCanvasInterface()
{
	return new QuickCanvas(m_gui, m_myTexLoader);
}
bool GwenImpl::OnKeyboard(int key, int state)
{
	return m_gui->keyboardCallback(key, state);
}
bool GwenImpl::OnMouseMove(int x, int y)
{
	return m_gui->mouseMoveCallback(x, y);
}
bool GwenImpl::OnMouseButton(int button, int state, int x, int y)
{
	return m_gui->mouseButtonCallback(button, state, x, y);
}
void GwenImpl::SetDescription(const char* description)
{
	m_gui->setStatusBarMessage("Status: OK", false);
	m_gui->setExampleDescription(description);
}
void GwenImpl::ShowMessage(const char* msg)
{
	m_gui->textOutput(msg);
	m_gui->forceUpdateScrollBars();
}
void GwenImpl::SetStatusbarMessage(const char* msg)
{
	bool isLeft = true;
	m_gui->setStatusBarMessage(msg, isLeft);
}
void GwenImpl::ShowErrorMessage(const char* msg)
{
	bool isLeft = false;
	m_gui->setStatusBarMessage(msg, isLeft);
	m_gui->textOutput(msg);
	m_gui->forceUpdateScrollBars();
}
void GwenImpl::RegisterFileOpen(const std::function<void()>& callback)
{
	m_gui->registerFileOpenCallback(callback);
}
void GwenImpl::RegisterQuit(const std::function<void()>& callback)
{
	m_gui->registerQuitCallback(callback);
}
void GwenImpl::Render(int w, int h)
{
	m_gui->draw(w, h);
}
void GwenImpl::ForceUpdateScrollBars()
{
	m_gui->forceUpdateScrollBars();
}
