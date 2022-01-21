#pragma once
#include <functional>

struct CommonGUIInterface
{
	using Factory = std::function<std::tuple<CommonGUIInterface*, int>(int width, int height, float reginaScale, class ExampleEntries* gAllExamples, const char* demoNameFromCommandOption,
																	   const std::function<void()>& onB, const std::function<void()>& onD, const std::function<void(int)>& _onE)>;
	virtual ~CommonGUIInterface() {}
	virtual struct CommonParameterInterface* CreateCommonParameterInterface() = 0;
	virtual struct Common2dCanvasInterface* CreateCommon2dCanvasInterface() = 0;
	// input
	virtual bool OnKeyboard(int key, int state) = 0;
	virtual bool OnMouseMove(int x, int y) = 0;
	virtual bool OnMouseButton(int button, int state, int x, int y) = 0;
	// callback
	virtual void RegisterFileOpen(const std::function<void()>& callback) = 0;
	virtual void RegisterQuit(const std::function<void()>& callback) = 0;
	// GUI
	virtual void SetDescription(const char* description) = 0;
	virtual void ShowMessage(const char* msg) = 0;
	virtual void SetStatusbarMessage(const char* msg, bool isLeft = true) = 0;
	virtual void ShowErrorMessage(const char* msg) = 0;
	virtual void Render(int w, int h) = 0;
	virtual void ForceUpdateScrollBars() = 0;
};
