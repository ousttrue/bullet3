#ifndef B3G_WINDOW_INTERFACE_H
#define B3G_WINDOW_INTERFACE_H

#include "CommonCallbacks.h"

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
	virtual ~CommonWindowInterface()
	{
	}

	virtual void closeWindow() = 0;

	virtual void runMainLoop() = 0;
	virtual float getTimeInSeconds() = 0;

	virtual bool requestedExit() const = 0;
	virtual void setRequestExit() = 0;

	virtual void startRendering() = 0;

	virtual void endRendering() = 0;

	virtual bool isModifierKeyPressed(int key) = 0;

	virtual void setMouseMoveCallback(b3MouseMoveCallback mouseCallback) = 0;
	virtual b3MouseMoveCallback getMouseMoveCallback() = 0;

	virtual void setMouseButtonCallback(b3MouseButtonCallback mouseCallback) = 0;
	virtual b3MouseButtonCallback getMouseButtonCallback() = 0;

	virtual void setResizeCallback(b3ResizeCallback resizeCallback) = 0;
	virtual b3ResizeCallback getResizeCallback() = 0;

	virtual void setWheelCallback(b3WheelCallback wheelCallback) = 0;
	virtual b3WheelCallback getWheelCallback() = 0;

	virtual void setKeyboardCallback(b3KeyboardCallback keyboardCallback) = 0;
	virtual b3KeyboardCallback getKeyboardCallback() = 0;

	virtual void setRenderCallback(b3RenderCallback renderCallback) = 0;

	virtual void setWindowTitle(const char* title) = 0;

	virtual float getRetinaScale() const = 0;
	virtual void setAllowRetina(bool allow) = 0;

	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;

	virtual int fileOpenDialog(char* fileName, int maxFileNameLength) = 0;
};

#endif  //B3G_WINDOW_INTERFACE_H
