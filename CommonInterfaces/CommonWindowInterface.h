#pragma once
#include "CommonCallbacks.h"
#include <list>

struct b3gWindowConstructionInfo
{
	int m_width = 1024;
	int m_height = 768;
	const char* m_title;
	bool m_fullscreen = false;
	int m_colorBitsPerPixel = 32;
	void* m_windowHandle = nullptr;
	int m_openglVersion = 3;
	int m_renderDevice = -1;
};

class CommonWindowInterface
{
public:
	virtual ~CommonWindowInterface() {}
	std::list<b3ResizeCallback> resizeCallback;
	std::list<b3MouseMoveCallback> mouseMoveCallback;
	std::list<b3MouseButtonCallback> mouseButtonCallback;
	std::list<b3WheelCallback> wheelCallback;
	std::list<b3KeyboardCallback> keyboardCallback;

	virtual void closeWindow() = 0;
	virtual void runMainLoop() = 0;
	virtual float getTimeInSeconds() = 0;
	virtual bool requestedExit() const = 0;
	virtual void setRequestExit() = 0;
	virtual void startRendering() = 0;
	virtual void endRendering() = 0;
	virtual bool isModifierKeyPressed(int key) = 0;
	virtual void setRenderCallback(b3RenderCallback renderCallback) = 0;
	virtual void setWindowTitle(const char* title) = 0;
	virtual float getRetinaScale() const = 0;
	virtual void setAllowRetina(bool allow) = 0;
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
	virtual int fileOpenDialog(char* fileName, int maxFileNameLength) = 0;
};
