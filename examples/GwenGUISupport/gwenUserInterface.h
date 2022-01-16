#pragma once
#include <CommonGUIInterface.h>

class GwenUserInterface : public CommonGUIInterface
{
	class GwenInternalData* m_data;
	GwenUserInterface(struct GlfwApp* s_app, int width, int height, float retinaScale);
public:
	~GwenUserInterface() override;
	class GwenInternalData* getInternalData()
	{
		return m_data;
	}
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
