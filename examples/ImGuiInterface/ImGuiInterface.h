#pragma once
#include <CommonGUIInterface.h>
#include <tuple>

class ImGuiInterface : public CommonGUIInterface
{
	float m_retinaScale;
	ImGuiInterface(float retinaScale);

public:
	static std::tuple<ImGuiInterface*, int> Create(struct GlfwApp* app, int width, int height, float reginaScale, class ExampleEntries* gAllExamples, const char* demoNameFromCommandOption,
	const std::function<void()>& onB, const std::function<void()>& onD, const std::function<void(int)>& _onE
	);

	~ImGuiInterface() override;
	struct CommonParameterInterface* CreateCommonParameterInterface() override;
	struct Common2dCanvasInterface* CreateCommon2dCanvasInterface() override;
	// input
	bool OnKeyboard(int key, int state) override;
	bool OnMouseMove(int x, int y) override;
	bool OnMouseButton(int button, int state, int x, int y) override;
	// callback
	void RegisterFileOpen(const std::function<void()>& callback) override;
	void RegisterQuit(const std::function<void()>& callback) override;
	// GUI
	void SetDescription(const char* description) override;
	void ShowMessage(const char* msg) override;
	void SetStatusbarMessage(const char* msg, bool isLeft) override;
	void ShowErrorMessage(const char* msg) override;
	void Render(int w, int h) override;
	void ForceUpdateScrollBars() override;
};
