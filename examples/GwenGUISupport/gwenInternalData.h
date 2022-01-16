#pragma once

#include "GwenOpenGL3CoreRenderer.h"
#include <GLPrimitiveRenderer.h>
#include "Gwen/Platform.h"
#include "Gwen/Controls/TreeControl.h"
#include "Gwen/Controls/RadioButtonController.h"
#include "Gwen/Controls/VerticalSlider.h"
#include "Gwen/Controls/HorizontalSlider.h"
#include "Gwen/Controls/GroupBox.h"
#include "Gwen/Controls/CheckBox.h"
#include "Gwen/Controls/StatusBar.h"
#include "Gwen/Controls/Button.h"
#include "Gwen/Controls/ComboBox.h"
#include "Gwen/Controls/MenuStrip.h"
#include "Gwen/Controls/Slider.h"
#include "Gwen/Controls/Property/Text.h"
#include "Gwen/Controls/SplitterBar.h"
#include "Bullet3Common/b3AlignedObjectArray.h"
#include "Gwen/Gwen.h"
#include "Gwen/Align.h"
#include "Gwen/Utility.h"
#include "Gwen/Controls/WindowControl.h"
#include "Gwen/Controls/TabControl.h"
#include "Gwen/Controls/ListBox.h"
#include "Gwen/Skins/Simple.h"
#include <Gwen/Controls/ImagePanel.h>
#include <Gwen/Controls/ColorPicker.h>
#include <Gwen/Controls/TreeNode.h>
//#include "Gwen/Skins/TexturedBase.h"
// #include "gwenUserInterface.h"
#include <LinearMath/btAlignedObjectArray.h>

using b3ComboBoxCallback = std::function<void(int combobox, const char* item)>;
using b3ToggleButtonCallback = std::function<void(int button, int state)>;
using b3FileOpenCallback = std::function<void()>;
using b3QuitCallback = std::function<void()>;

class GwenInternalData
{
public:
	struct GL3TexLoader* m_myTexLoader = nullptr;
	struct MyMenuItemHander* m_handler2 = nullptr;
	btAlignedObjectArray<Gwen::Controls::TreeNode*> m_nodes;
	btAlignedObjectArray<MyMenuItemHander*> m_nodeHandlers;

	//struct sth_stash;
	//class GwenOpenGL3CoreRenderer*	pRenderer;
	Gwen::Renderer::Base* pRenderer;
	Gwen::Skin::Simple skin;
	Gwen::Controls::Canvas* pCanvas;
	//GLPrimitiveRenderer* m_primRenderer;
	Gwen::Controls::TabButton* m_demoPage;
	Gwen::Controls::TabButton* m_explorerPage;
	Gwen::Controls::TreeControl* m_explorerTreeCtrl;
	Gwen::Controls::MenuItem* m_viewMenu;
	class MyMenuItems* m_menuItems;
	Gwen::Controls::ListBox* m_TextOutput;
	Gwen::Controls::Label* m_exampleInfoGroupBox;
	Gwen::Controls::ListBox* m_exampleInfoTextOutput;
	struct MyTestMenuBar* m_menubar;
	Gwen::Controls::StatusBar* m_bar;
	Gwen::Controls::ScrollControl* m_windowRight;
	Gwen::Controls::TabControl* m_tab;

	int m_curYposition;

	Gwen::Controls::Label* m_rightStatusBar;
	Gwen::Controls::Label* m_leftStatusBar;
	b3AlignedObjectArray<class Gwen::Event::Handler*> m_handlers;

public:
	b3ToggleButtonCallback m_toggleButtonCallback;
	b3ComboBoxCallback m_comboBoxCallback;

	GwenInternalData(struct GlfwApp* s_app, int width, int height, float retinaScale);
	~GwenInternalData();
	int setup(class ExampleEntries* gAllExamples, const char* demoNameFromCommandOption,
			  const std::function<void()>& onB, const std::function<void()>& onD, const std::function<void(int)>& _onE);
	void setFocus();
	void resize(int width, int height);
	bool keyboardCallback(int key, int state);

	void setToggleButtonCallback(b3ToggleButtonCallback callback);
	b3ToggleButtonCallback getToggleButtonCallback();

	void registerToggleButton2(int buttonId, const char* name);

	void setComboBoxCallback(b3ComboBoxCallback callback);
	b3ComboBoxCallback getComboBoxCallback();
	void registerComboBox2(int buttonId, int numItems, const char** items, int startItem = 0);

	void textOutput(const char* msg);
	void setExampleDescription(const char* msg);

	void setStatusBarMessage(const char* message, bool isLeft)
	{
		Gwen::UnicodeString msg = Gwen::Utility::StringToUnicode(message);
		if (isLeft)
		{
			m_leftStatusBar->SetText(msg);
		}
		else
		{
			m_rightStatusBar->SetText(msg);
		}
	}
	void registerFileOpen(const std::function<void()>& callback);
	void registerQuit(const std::function<void()>& callback);
	void forceUpdateScrollBars();
	void render(int width, int height);
	struct Common2dCanvasInterface* createCommon2dCanvasInterface();
	bool onMouseMove(int x, int y);
	bool onKeyboard(int bulletKey, int state);
	bool onMouseButton(int button, int state, int x, int y);
};
