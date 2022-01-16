#include "ImGuiInterface.h"

ImGuiInterface::ImGuiInterface(float retinaScale)
{
}

std::tuple<ImGuiInterface*, int> ImGuiInterface::Create(struct GlfwApp* app, int width, int height, float reginaScale, class ExampleEntries* gAllExamples, const char* demoNameFromCommandOption,
														const std::function<void()>& onB, const std::function<void()>& onD, const std::function<void(int)>& _onE)
{
	auto p = new ImGuiInterface(reginaScale);

	return {p, 0};
}

ImGuiInterface::~ImGuiInterface() {}

struct CommonParameterInterface* ImGuiInterface::CreateCommonParameterInterface()
{
	return nullptr;
}

struct Common2dCanvasInterface* ImGuiInterface::CreateCommon2dCanvasInterface()
{
	return nullptr;
	;
}

bool ImGuiInterface::OnKeyboard(int key, int state)
{
	return false;
}

bool ImGuiInterface::OnMouseMove(int x, int y)
{
	return false;
	;
}

bool ImGuiInterface::OnMouseButton(int button, int state, int x, int y)
{
	return false;
	;
}

void ImGuiInterface::RegisterFileOpen(const std::function<void()>& callback)
{
}

void ImGuiInterface::RegisterQuit(const std::function<void()>& callback)
{
}

void ImGuiInterface::SetDescription(const char* description) {}

void ImGuiInterface::ShowMessage(const char* msg) {}

void ImGuiInterface::SetStatusbarMessage(const char* msg, bool isLeft) {}

void ImGuiInterface::ShowErrorMessage(const char* msg) {}

void ImGuiInterface::Render(int w, int h) {}

void ImGuiInterface::ForceUpdateScrollBars() {}
