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
#include <list>
#include <memory>

using b3ComboBoxCallback = std::function<void(int combobox, const char* item)>;
using b3ToggleButtonCallback = std::function<void(int button, int state)>;
using b3FileOpenCallback = std::function<void()>;
using b3QuitCallback = std::function<void()>;

class GwenInternalData
{
	std::unique_ptr<struct MyMenuItemHander> m_handler2;
	std::unique_ptr<struct GL3TexLoader> m_myTexLoader;
	std::unique_ptr<Gwen::Renderer::Base> pRenderer;
	Gwen::Skin::Simple skin;

public:
	std::unique_ptr<Gwen::Controls::Canvas> pCanvas;
	GLPrimitiveRenderer* m_primRenderer;
	class OpenGL2RenderCallbacks* m_renderCallbacks;
	Gwen::Controls::TabButton* m_demoPage;
	Gwen::Controls::TabButton* m_explorerPage;
	Gwen::Controls::TreeControl* m_explorerTreeCtrl;
	Gwen::Controls::MenuItem* m_viewMenu;
	class MyMenuItems* m_menuItems;
	Gwen::Controls::Label* m_exampleInfoGroupBox;
	Gwen::Controls::ListBox* m_exampleInfoTextOutput;

private:
	std::unique_ptr<Gwen::Controls::ListBox> m_TextOutput;
	std::unique_ptr<struct MyTestMenuBar> m_menubar;
	std::unique_ptr<Gwen::Controls::StatusBar> m_bar;
	std::unique_ptr<Gwen::Controls::Label> m_rightStatusBar;
	std::unique_ptr<Gwen::Controls::Label> m_leftStatusBar;
	std::unique_ptr<Gwen::Controls::ScrollControl> m_windowRight;
	std::unique_ptr<Gwen::Controls::TabControl> m_tab;

	// std::list<std::unique_ptr<Gwen::Controls::TreeNode>> m_nodes;
	std::list<std::unique_ptr<MyMenuItemHander>> m_nodeHandlers;
public:
	std::list<std::unique_ptr<class Gwen::Event::Handler>> m_handlers;
	int m_curYposition;

public:
	b3ToggleButtonCallback m_toggleButtonCallback;
	b3ComboBoxCallback m_comboBoxCallback;

	GwenInternalData(int width, int height, float retinaScale);
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
