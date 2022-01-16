#include "gwenInternalData.h"
#include "Gwen/Controls/TreeNode.h"
#include "GwenTextureWindow.h"
#include "GraphingTexture.h"
#include <memory>
#include <tuple>
#include <functional>
#include <Common2dCanvasInterface.h>
#include <CommonExampleInterface.h>
#include <Bullet3Common/b3HashMap.h>
#include <GlfwApp.h>

#define MAX_GRAPH_WINDOWS 5

class MyMenuItems : public Gwen::Controls::Base
{
public:
	b3FileOpenCallback m_fileOpenCallback;
	b3QuitCallback m_quitCallback;

	MyMenuItems() : Gwen::Controls::Base(0), m_fileOpenCallback(0)
	{
	}
	void myQuitApp(Gwen::Controls::Base* pControl)
	{
		if (m_quitCallback)
		{
			(m_quitCallback)();
		}
	}
	void fileOpen(Gwen::Controls::Base* pControl)
	{
		if (m_fileOpenCallback)
		{
			(m_fileOpenCallback)();
		}
	}
};

struct MyTestMenuBar : public Gwen::Controls::MenuStrip
{
	Gwen::Controls::MenuItem* m_fileMenu;
	Gwen::Controls::MenuItem* m_viewMenu;
	MyMenuItems* m_menuItems;

	MyTestMenuBar(Gwen::Controls::Base* pParent)
		: Gwen::Controls::MenuStrip(pParent)
	{
		//		Gwen::Controls::MenuStrip* menu = new Gwen::Controls::MenuStrip( pParent );
		{
			m_menuItems = new MyMenuItems();
			m_menuItems->m_fileOpenCallback = 0;
			m_menuItems->m_quitCallback = 0;

			m_fileMenu = AddItem(L"File");

			m_fileMenu->GetMenu()->AddItem(L"Open", m_menuItems, (Gwen::Event::Handler::Function)&MyMenuItems::fileOpen);
			m_fileMenu->GetMenu()->AddItem(L"Quit", m_menuItems, (Gwen::Event::Handler::Function)&MyMenuItems::myQuitApp);
			m_viewMenu = AddItem(L"View");
		}
	}
	virtual ~MyTestMenuBar()
	{
		delete m_menuItems;
	}
};

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
		//  if (m_toggleButtonCallback)
		//  (*m_toggleButtonCallback)(m_buttonId, tog);
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
	GwenInternalData* m_internalData;

	GL3TexLoader* m_myTexLoader;

	MyGraphWindow* m_gw[MAX_GRAPH_WINDOWS];
	GraphingTexture* m_gt[MAX_GRAPH_WINDOWS];
	int m_curNumGraphWindows;

	QuickCanvas(GwenInternalData* internalData, GL3TexLoader* myTexLoader)
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

			MyGraphInput input(m_internalData);
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

struct MyComboBoxHander : public Gwen::Event::Handler
{
	GwenInternalData* m_data;
	int m_buttonId;

	MyComboBoxHander(GwenInternalData* data, int buttonId)
		: m_data(data),
		  m_buttonId(buttonId)
	{
	}

	void onSelect(Gwen::Controls::Base* pControl)
	{
		Gwen::Controls::ComboBox* but = (Gwen::Controls::ComboBox*)pControl;

		Gwen::String str = Gwen::Utility::UnicodeToString(but->GetSelectedItem()->GetText());

		if (m_data->m_comboBoxCallback)
			(m_data->m_comboBoxCallback)(m_buttonId, str.c_str());
	}
};

struct MyButtonHander : public Gwen::Event::Handler
{
	GwenInternalData* m_data;
	int m_buttonId;

	MyButtonHander(GwenInternalData* data, int buttonId)
		: m_data(data),
		  m_buttonId(buttonId)
	{
	}

	void onButtonA(Gwen::Controls::Base* pControl)
	{
		Gwen::Controls::Button* but = (Gwen::Controls::Button*)pControl;
		//		int dep = but->IsDepressed();
		int tog = but->GetToggleState();
		if (m_data->m_toggleButtonCallback)
			(m_data->m_toggleButtonCallback)(m_buttonId, tog);
	}
};

//
// GwenInternalData
//
GwenInternalData::GwenInternalData(GlfwApp* s_app, int width, int height, float retinaScale)
{
	m_myTexLoader.reset(new GL3TexLoader);

	auto fontstash = s_app->getFontStash();

	// m_gui = new GwenInternalData;
	// init(width, height, m_gwenRenderer, retinaScale);
	// void GwenInternalData::init(int width, int height, Gwen::Renderer::Base* renderer, float retinaScale)

	m_curYposition = 20;
	//m_primRenderer = new GLPrimitiveRenderer(width,height);
	pRenderer.reset(new GwenOpenGL3CoreRenderer(s_app->m_primRenderer, fontstash, width, height, retinaScale, m_myTexLoader.get()));

	skin.SetRender(pRenderer.get());

	pCanvas.reset(new Gwen::Controls::Canvas(&skin));
	pCanvas->SetSize(width, height);
	pCanvas->SetDrawBackground(false);
	pCanvas->SetBackgroundColor(Gwen::Color(150, 170, 170, 255));

	m_menubar.reset(new MyTestMenuBar(pCanvas.get()));
	m_viewMenu = m_menubar->m_viewMenu;
	m_menuItems = m_menubar->m_menuItems;

	m_bar.reset(new Gwen::Controls::StatusBar(pCanvas.get()));
	m_rightStatusBar.reset(new Gwen::Controls::Label(m_bar.get()));

	m_rightStatusBar->SetWidth(width / 2);
	//m_rightStatusBar->SetText( L"Label Added to Right" );
	m_bar->AddControl(m_rightStatusBar.get(), true);

	m_TextOutput.reset(new Gwen::Controls::ListBox(pCanvas.get()));

	m_TextOutput->Dock(Gwen::Pos::Bottom);
	m_TextOutput->SetHeight(100);
	m_leftStatusBar.reset(new Gwen::Controls::Label(m_bar.get()));

	//m_leftStatusBar->SetText( L"Label Added to Left" );
	m_leftStatusBar->SetWidth(width / 2);
	m_bar->AddControl(m_leftStatusBar.get(), false);

	//Gwen::KeyboardFocus
	/*Gwen::Controls::GroupBox* box = new Gwen::Controls::GroupBox(pCanvas);
	box->SetText("text");
	box->SetName("name");
	box->SetHeight(500);
	*/

	m_windowRight.reset(new Gwen::Controls::ScrollControl(pCanvas.get()));
	m_windowRight->Dock(Gwen::Pos::Right);
	m_windowRight->SetWidth(250);
	m_windowRight->SetHeight(250);
	m_windowRight->SetScroll(false, true);

	//windowLeft->SetSkin(
	m_tab.reset(new Gwen::Controls::TabControl(m_windowRight.get()));

	//tab->SetHeight(300);
	m_tab->SetWidth(240);
	m_tab->SetHeight(13250);
	//tab->Dock(Gwen::Pos::Left);
	m_tab->Dock(Gwen::Pos::Fill);
	//tab->SetMargin( Gwen::Margin( 2, 2, 2, 2 ) );

	Gwen::UnicodeString str1(L"Params");
	m_demoPage = m_tab->AddPage(str1);

	//	Gwen::UnicodeString str2(L"OpenCL");
	//	tab->AddPage(str2);
	//Gwen::UnicodeString str3(L"page3");
	//	tab->AddPage(str3);

	//but->onPress.Add(handler, &MyHander::onButtonA);

	//box->Dock(Gwen::Pos::Left);

	/*Gwen::Controls::WindowControl* windowBottom = new Gwen::Controls::WindowControl(pCanvas);
	windowBottom->SetHeight(100);
	windowBottom->Dock(Gwen::Pos::Bottom);
	windowBottom->SetTitle("bottom");
	*/
	//	Gwen::Controls::Property::Text* prop = new Gwen::Controls::Property::Text(pCanvas);
	//prop->Dock(Gwen::Pos::Bottom);
	/*Gwen::Controls::SplitterBar* split = new Gwen::Controls::SplitterBar(pCanvas);
	split->Dock(Gwen::Pos::Center);
	split->SetHeight(300);
	split->SetWidth(300);
	*/
	/*


	*/

	Gwen::Controls::ScrollControl* windowLeft = new Gwen::Controls::ScrollControl(pCanvas.get());
	windowLeft->Dock(Gwen::Pos::Left);
	//	windowLeft->SetTitle("title");
	windowLeft->SetScroll(false, false);
	windowLeft->SetWidth(250);
	windowLeft->SetPos(50, 50);
	windowLeft->SetHeight(500);
	//windowLeft->SetClosable(false);
	//	windowLeft->SetShouldDrawBackground(true);
	windowLeft->SetTabable(true);

	Gwen::Controls::TabControl* explorerTab = new Gwen::Controls::TabControl(windowLeft);

	//tab->SetHeight(300);
	//	explorerTab->SetWidth(230);
	explorerTab->SetHeight(250);
	//tab->Dock(Gwen::Pos::Left);
	explorerTab->Dock(Gwen::Pos::Fill);

	//m_exampleInfoTextOutput->SetBounds(2, 10, 236, 400);

	//windowRight

	Gwen::UnicodeString explorerStr1(L"Explorer");
	m_explorerPage = explorerTab->AddPage(explorerStr1);
	Gwen::UnicodeString shapesStr1(L"Test");
	Gwen::Controls::TabButton* shapes = explorerTab->AddPage(shapesStr1);

	///todo(erwincoumans) figure out why the HSV color picker is extremely slow
	//Gwen::Controls::HSVColorPicker* color = new Gwen::Controls::HSVColorPicker(shapes->GetPage());
	Gwen::Controls::ColorPicker* color = new Gwen::Controls::ColorPicker(shapes->GetPage());
	color->SetKeyboardInputEnabled(true);

	Gwen::Controls::TreeControl* ctrl = new Gwen::Controls::TreeControl(m_explorerPage->GetPage());
	m_explorerTreeCtrl = ctrl;
	ctrl->SetKeyboardInputEnabled(true);
	ctrl->Focus();
	ctrl->SetBounds(2, 10, 236, 300);

	m_exampleInfoGroupBox = new Gwen::Controls::Label(m_explorerPage->GetPage());
	m_exampleInfoGroupBox->SetPos(2, 314);
	m_exampleInfoGroupBox->SetHeight(15);
	m_exampleInfoGroupBox->SetWidth(234);
	m_exampleInfoGroupBox->SetText("Example Description");

	m_exampleInfoTextOutput = new Gwen::Controls::ListBox(m_explorerPage->GetPage());

	//m_exampleInfoTextOutput->Dock( Gwen::Pos::Bottom );
	m_exampleInfoTextOutput->SetPos(2, 332);
	m_exampleInfoTextOutput->SetHeight(150);
	m_exampleInfoTextOutput->SetWidth(233);

	setFocus();
}

GwenInternalData::~GwenInternalData()
{
}

int GwenInternalData::setup(ExampleEntries* gAllExamples, const char* demoNameFromCommandOption,
							const std::function<void()>& onB, const std::function<void()>& onD, const std::function<void(int)>& onE)
{
	auto tree = m_explorerTreeCtrl;
	m_handler2.reset(new MyMenuItemHander(-1, onB, onD, onE));

	auto curNode = (Gwen::Controls::TreeNode*)tree;
	tree->onReturnKeyDown.Add(m_handler2.get(), &MyMenuItemHander::onButtonD);

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
			std::unique_ptr<MyMenuItemHander> handler;
			handler.reset(new MyMenuItemHander(d,
											   onB,
											   onD,
											   onE));

			pNode->onNamePress.Add(handler.get(), &MyMenuItemHander::onButtonA);
			pNode->GetButton()->onDoubleClick.Add(handler.get(), &MyMenuItemHander::onButtonB);
			pNode->GetButton()->onDown.Add(handler.get(), &MyMenuItemHander::onButtonC);
			pNode->onSelect.Add(handler.get(), &MyMenuItemHander::onButtonE);
			pNode->onReturnKeyDown.Add(handler.get(), &MyMenuItemHander::onButtonG);
			pNode->onSelectChange.Add(handler.get(), &MyMenuItemHander::onButtonF);

			m_nodeHandlers.push_back(std::move(handler));

#endif
			//   pNode->onKeyReturn.Add(handler, &MyMenuItemHander::onButtonD);
			//   pNode->GetButton()->onKeyboardReturn.Add(handler, &MyMenuItemHander::onButtonD);
			//  pNode->onNamePress.Add(handler, &MyMenuItemHander::onButtonD);
			//   pNode->onKeyboardPressed.Add(handler, &MyMenuItemHander::onButtonD);
			//   pNode->OnKeyPress
		}
		else
		{
			std::unique_ptr<Gwen::Controls::TreeNode> curNode;
			curNode.reset(tree->AddNode(nodeUText));
			m_nodes.push_back(std::move(curNode));
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

	return firstAvailableDemoIndex;
}

void GwenInternalData::resize(int width, int height)
{
	pCanvas->SetSize(width, height);
}

void GwenInternalData::textOutput(const char* message)
{
	Gwen::UnicodeString msg = Gwen::Utility::StringToUnicode(message);
	m_TextOutput->AddItem(msg);
	m_TextOutput->Scroller()->ScrollToBottom();
}

void GwenInternalData::setExampleDescription(const char* message)
{
	//Gwen apparently doesn't have text/word wrap, so do rudimentary brute-force implementation here.

	std::string wrapmessage = message;
	int startPos = 0;

	std::string lastFit = "";
	bool hasSpace = false;
	std::string lastFitSpace = "";
	int spacePos = 0;

	m_exampleInfoTextOutput->Clear();
	int fixedWidth = m_exampleInfoTextOutput->GetBounds().w - 25;
	int wrapLen = int(wrapmessage.length());
	for (int endPos = 0; endPos <= wrapLen; endPos++)
	{
		std::string sub = wrapmessage.substr(startPos, (endPos - startPos));
		Gwen::Point pt = pRenderer->MeasureText(pCanvas->GetSkin()->GetDefaultFont(), sub);

		if (pt.x <= fixedWidth)
		{
			lastFit = sub;

			if (message[endPos] == ' ' || message[endPos] == '.' || message[endPos] == ',')
			{
				hasSpace = true;
				lastFitSpace = sub;
				spacePos = endPos;
			}
		}
		else
		{
			//submit and
			if (hasSpace)
			{
				endPos = spacePos + 1;
				hasSpace = false;
				lastFit = lastFitSpace;
				startPos = endPos;
			}
			else
			{
				startPos = endPos - 1;
			}
			Gwen::UnicodeString msg = Gwen::Utility::StringToUnicode(lastFit);

			m_exampleInfoTextOutput->AddItem(msg);
			m_exampleInfoTextOutput->Scroller()->ScrollToBottom();
		}
	}

	if (lastFit.length())
	{
		Gwen::UnicodeString msg = Gwen::Utility::StringToUnicode(lastFit);
		m_exampleInfoTextOutput->AddItem(msg);
		m_exampleInfoTextOutput->Scroller()->ScrollToBottom();
	}
}

void GwenInternalData::registerFileOpen(const std::function<void()>& callback)
{
	m_menuItems->m_fileOpenCallback = callback;
}
void GwenInternalData::registerQuit(const std::function<void()>& callback)
{
	m_menuItems->m_quitCallback = callback;
}

void GwenInternalData::forceUpdateScrollBars()
{
	b3Assert(m_explorerTreeCtrl);
	if (m_explorerTreeCtrl)
	{
		m_explorerTreeCtrl->ForceUpdateScrollBars();
	}
}

void GwenInternalData::setFocus()
{
	b3Assert(m_explorerTreeCtrl);
	if (m_explorerTreeCtrl)
	{
		m_explorerTreeCtrl->Focus();
	}
}

b3ToggleButtonCallback GwenInternalData::getToggleButtonCallback()
{
	return m_toggleButtonCallback;
}

void GwenInternalData::setToggleButtonCallback(b3ToggleButtonCallback callback)
{
	m_toggleButtonCallback = callback;
}

void GwenInternalData::registerToggleButton2(int buttonId, const char* name)
{
	assert(m_demoPage);

	Gwen::Controls::Button* but = new Gwen::Controls::Button(m_demoPage->GetPage());

	///some heuristic to find the button location
	int ypos = m_curYposition;
	but->SetPos(10, ypos);
	but->SetWidth(200);
	//but->SetBounds( 200, 30, 300, 200 );

	std::unique_ptr<MyButtonHander> handler;
	handler.reset(new MyButtonHander(this, buttonId));
	m_handlers.push_back(std::move(handler));
	m_curYposition += 22;
	but->onToggle.Add(handler.get(), &MyButtonHander::onButtonA);
	but->SetIsToggle(true);
	but->SetToggleState(false);
	but->SetText(name);
}

void GwenInternalData::setComboBoxCallback(b3ComboBoxCallback callback)
{
	m_comboBoxCallback = callback;
}

b3ComboBoxCallback GwenInternalData::getComboBoxCallback()
{
	return m_comboBoxCallback;
}

void GwenInternalData::registerComboBox2(int comboboxId, int numItems, const char** items, int startItem)
{
	Gwen::Controls::ComboBox* combobox = new Gwen::Controls::ComboBox(m_demoPage->GetPage());
	std::unique_ptr<MyComboBoxHander> handler;
	handler.reset(new MyComboBoxHander(this, comboboxId));
	m_handlers.push_back(std::move(handler));

	combobox->onSelection.Add(handler.get(), &MyComboBoxHander::onSelect);
	int ypos = m_curYposition;
	combobox->SetPos(10, ypos);
	combobox->SetWidth(100);
	//box->SetPos(120,130);
	for (int i = 0; i < numItems; i++)
	{
		Gwen::Controls::MenuItem* item = combobox->AddItem(Gwen::Utility::StringToUnicode(items[i]));
		if (i == startItem)
			combobox->OnItemSelected(item);
	}

	m_curYposition += 22;
}

void GwenInternalData::render(int width, int height)
{
	//	printf("width = %d, height=%d\n", width,height);
	if (pCanvas)
	{
		pCanvas->SetSize(width, height);
		//m_primRenderer->setScreenSize(width,height);
		pRenderer->Resize(width, height);
		pCanvas->RenderCanvas();
		//restoreOpenGLState();
	}
}

Common2dCanvasInterface* GwenInternalData::createCommon2dCanvasInterface()
{
	return new QuickCanvas(this, m_myTexLoader.get());
}

bool GwenInternalData::onMouseMove(int x, int y)
{
	bool handled = false;

	static int m_lastmousepos[2] = {0, 0};
	static bool isInitialized = false;
	if (pCanvas)
	{
		if (!isInitialized)
		{
			isInitialized = true;
			m_lastmousepos[0] = x + 1;
			m_lastmousepos[1] = y + 1;
		}
		handled = pCanvas->InputMouseMoved(x, y, m_lastmousepos[0], m_lastmousepos[1]);
	}
	return handled;
}

bool GwenInternalData::onKeyboard(int bulletKey, int state)
{
	int gwenKey = -1;
	if (pCanvas)
	{
		//convert 'Bullet' keys into 'Gwen' keys
		switch (bulletKey)
		{
			case B3G_RETURN:
			{
				gwenKey = Gwen::Key::Return;
				break;
			}
			case B3G_LEFT_ARROW:
			{
				gwenKey = Gwen::Key::Left;
				break;
			}
			case B3G_RIGHT_ARROW:
			{
				gwenKey = Gwen::Key::Right;
				break;
			}
			case B3G_UP_ARROW:
			{
				gwenKey = Gwen::Key::Up;
				break;
			}
			case B3G_DOWN_ARROW:
			{
				gwenKey = Gwen::Key::Down;
				break;
			}
			case B3G_BACKSPACE:
			{
				gwenKey = Gwen::Key::Backspace;
				break;
			}
			case B3G_DELETE:
			{
				gwenKey = Gwen::Key::Delete;
				break;
			}
			case B3G_HOME:
			{
				gwenKey = Gwen::Key::Home;
				break;
			}
			case B3G_END:
			{
				gwenKey = Gwen::Key::End;
				break;
			}
			case B3G_SHIFT:
			{
				gwenKey = Gwen::Key::Shift;
				break;
			}
			case B3G_CONTROL:
			{
				gwenKey = Gwen::Key::Control;
				break;
			}

			default:
			{
			}
		};

		if (gwenKey >= 0)
		{
			return pCanvas->InputKey(gwenKey, state == 1);
		}
		else
		{
			if (bulletKey < 256 && state)
			{
				Gwen::UnicodeChar c = (Gwen::UnicodeChar)bulletKey;
				return pCanvas->InputCharacter(c);
			}
		}
	}
	return false;
}

bool GwenInternalData::onMouseButton(int button, int state, int x, int y)
{
	bool handled = false;
	if (pCanvas)
	{
		handled = pCanvas->InputMouseMoved(x, y, x, y);

		if (button >= 0)
		{
			handled = pCanvas->InputMouseButton(button, (bool)state);
			if (handled)
			{
				//if (!state)
				//	return false;
			}
		}
	}
	return handled;
}
