#pragma once
#include <LinearMath/btAlignedObjectArray.h>
#include <tuple>
#include <functional>
#include <Common2dCanvasInterface.h>
#include <CommonGUIInterface.h>

struct GwenInternalData;

using b3ComboBoxCallback = std::function<void(int combobox, const char* item)>;
using b3ToggleButtonCallback = std::function<void(int button, int state)>;
using b3FileOpenCallback = std::function<void()>;
using b3QuitCallback = std::function<void()>;
namespace Gwen
{
namespace Renderer
{
class Base;
}
namespace Controls
{
class TreeNode;
class TreeControl;
}  // namespace Controls
}  // namespace Gwen


class GwenUserInterface: public CommonGUIInterface
{
	GwenInternalData* m_data;

	GwenUserInterface(struct GlfwApp* s_app, int width, int height, float retinaScale);
public:
	~GwenUserInterface() override;

private:
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

	void registerFileOpenCallback(b3FileOpenCallback callback);

public:
	GwenInternalData* getInternalData()
	{
		return m_data;
	}

private:
	//
	struct GL3TexLoader* m_myTexLoader = nullptr;
	// Gwen::Renderer::Base* m_gwenRenderer = nullptr;
	struct MyMenuItemHander* m_handler2 = nullptr;
	btAlignedObjectArray<Gwen::Controls::TreeNode*> m_nodes;
	btAlignedObjectArray<MyMenuItemHander*> m_handlers;

public:
	static std::tuple<CommonGUIInterface*, int> Create(struct GlfwApp* app, int width, int height, float reginaScale, class ExampleEntries* gAllExamples, const char* demoNameFromCommandOption,
													   const std::function<void()>& onB, const std::function<void()>& onD, const std::function<void(int)>& _onE);
	struct CommonParameterInterface* CreateCommonParameterInterface() override;
	struct Common2dCanvasInterface* CreateCommon2dCanvasInterface() override;
	bool OnKeyboard(int key, int state) override;
	bool OnMouseMove(int x, int y) override;
	bool OnMouseButton(int button, int state, int x, int y) override;
	void SetDescription(const char* description) override;
	void ShowMessage(const char* msg) override;
	void SetStatusbarMessage(const char* msg, bool isLeft = true) override;
	void ShowErrorMessage(const char* msg) override;
	void RegisterFileOpen(const std::function<void()>& callback) override;
	void RegisterQuit(const std::function<void()>& callback) override;
	void Render(int w, int h) override;
	void ForceUpdateScrollBars() override;
};
