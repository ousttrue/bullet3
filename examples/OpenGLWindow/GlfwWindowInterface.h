#pragma once
#include <CommonWindowInterface.h>

class GlfwWindowInterface : public CommonWindowInterface
{
	struct GLFWwindow* m_window = nullptr;
	float m_retinaScale = 1.0f;

	GlfwWindowInterface(GLFWwindow* window);
	GlfwWindowInterface(const GlfwWindowInterface*) = delete;
	GlfwWindowInterface& operator=(const GlfwWindowInterface*) = delete;

public:
	~GlfwWindowInterface();
	static GlfwWindowInterface* createWindow(const b3gWindowConstructionInfo& ci);
	void closeWindow() override;
	void runMainLoop() override {}
	float getTimeInSeconds() override;
	bool requestedExit() const override;
	void setRequestExit() override;
	void startRendering() override;
	void endRendering() override;
	bool isModifierKeyPressed(int key) override;
	// render
	b3RenderCallback m_render;
	void setRenderCallback(b3RenderCallback renderCallback) override
	{
		m_render = renderCallback;
	}
	void setWindowTitle(const char* title) override {}
	float getRetinaScale() const override { return m_retinaScale; }
	void setAllowRetina(bool /*allowRetina*/) override{};
	int getWidth() const override;
	int getHeight() const override;
	int fileOpenDialog(char* fileName, int maxFileNameLength) override;
};
