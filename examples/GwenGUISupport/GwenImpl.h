#pragma once
#include <LinearMath/btAlignedObjectArray.h>
#include <tuple>
#include <functional>
#include <Common2dCanvasInterface.h>
#include <CommonGUIInterface.h>

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

class GwenImpl : public CommonGUIInterface
{
	struct GL3TexLoader* m_myTexLoader = nullptr;
	Gwen::Renderer::Base* m_gwenRenderer = nullptr;
	struct GwenUserInterface* m_gui = nullptr;
	struct MyMenuItemHander* m_handler2 = nullptr;
	btAlignedObjectArray<Gwen::Controls::TreeNode*> m_nodes;
	btAlignedObjectArray<MyMenuItemHander*> m_handlers;

	GwenImpl(struct GlfwApp* s_app, int width, int height, float retinaScale);

public:
	~GwenImpl() override;
	static std::tuple<GwenImpl*, int> Create(struct GlfwApp* app, int width, int height, float reginaScale, class ExampleEntries* gAllExamples, const char* demoNameFromCommandOption,
											 const std::function<void()>& onB, const std::function<void()>& onD, const std::function<void(int)>& _onE);
	struct CommonParameterInterface* CreateCommonParameterInterface() override;
	struct Common2dCanvasInterface* CreateCommon2dCanvasInterface() override;
	bool OnKeyboard(int key, int state) override;
	bool OnMouseMove(int x, int y) override;
	bool OnMouseButton(int button, int state, int x, int y) override;
	void SetDescription(const char* description) override;
	void ShowMessage(const char* msg) override;
	void SetStatusbarMessage(const char* msg) override;
	void ShowErrorMessage(const char* msg) override;
	void RegisterFileOpen(const std::function<void()>& callback) override;
	void RegisterQuit(const std::function<void()>& callback) override;
	void Render(int w, int h) override;
	void ForceUpdateScrollBars() override;
};
