#pragma once
#include <LinearMath/btAlignedObjectArray.h>
#include <tuple>
#include <functional>
#include "Common2dCanvasInterface.h"

namespace Gwen
{
namespace Renderer
{
struct Base;
}
namespace Controls
{
struct TreeNode;
struct TreeControl;
}  // namespace Controls
}  // namespace Gwen

class GwenImpl
{
	struct GL3TexLoader* m_myTexLoader = nullptr;
	Gwen::Renderer::Base* m_gwenRenderer = nullptr;
	struct GwenUserInterface* m_gui = nullptr;
	struct MyMenuItemHander* m_handler2 = nullptr;
	btAlignedObjectArray<Gwen::Controls::TreeNode*> m_nodes;
	btAlignedObjectArray<MyMenuItemHander*> m_handlers;

	GwenImpl(struct SimpleOpenGL3App* s_app, int width, int height, float retinaScale);

public:
	~GwenImpl();
	static std::tuple<GwenImpl*, int> Create(struct SimpleOpenGL3App* app, int width, int height, float reginaScale, class ExampleEntries* gAllExamples, const char* demoNameFromCommandOption,
											 const std::function<void()>& onB, const std::function<void()>& onD, const std::function<void(int)>& _onE);
	struct CommonParameterInterface* CreateCommonParameterInterface();
	struct Common2dCanvasInterface* CreateCommon2dCanvasInterface();
	bool OnKeyboard(int key, int state);
	bool OnMouseMove(int x, int y);
	bool OnMouseButton(int button, int state, int x, int y);
	void SetDescription(const char* description);
	void ShowMessage(const char* msg);
	void SetStatusbarMessage(const char* msg);
	void ShowErrorMessage(const char* msg);
	void RegisterFileOpen(const std::function<void()>& callback);
	void RegisterQuit(const std::function<void()>& callback);
	void Render(int w, int h);
	void ForceUpdateScrollBars();
};
