#include "GlfwWindowInterface.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <assert.h>
#include <glad/gl.h>

static void window_close_callback(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void SimpleResizeCallback(GLFWwindow* window, int width, int height)
{
	auto gApp = (GlfwWindowInterface*)glfwGetWindowUserPointer(window);
	for (auto& callback : gApp->resizeCallback)
	{
		callback(width, height);
	}
}

static void SimpleKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto gApp = (GlfwWindowInterface*)glfwGetWindowUserPointer(window);
	for (auto& callback : gApp->keyboardCallback)
	{
		if (callback(key, action))
		{
			break;
		}
	}
}

int s_x = 0;
int s_y = 0;
static void SimpleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	auto gApp = (GlfwWindowInterface*)glfwGetWindowUserPointer(window);
	switch (button)
	{
		case GLFW_MOUSE_BUTTON_LEFT:
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			button = 2;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			button = 1;
			break;
	}

	ButtonFlags flags = {};
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT))
	{
		flags = (ButtonFlags)(flags | ButtonFlagsAlt);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
	{
		flags = (ButtonFlags)(flags | ButtonFlagsCtrl);
	}

	for (auto& callback : gApp->mouseButtonCallback)
	{
		if (callback(button, action, s_x, s_y, flags))
		{
			break;
		}
	}
}

static void SimpleMouseMoveCallback(GLFWwindow* window, double x, double y)
{
	auto gApp = (GlfwWindowInterface*)glfwGetWindowUserPointer(window);
	s_x = x;
	s_y = y;

	for (auto& callback : gApp->mouseMoveCallback)
	{
		if (callback(x, y))
		{
			break;
		}
	}
}

static void SimpleWheelCallback(GLFWwindow* window, double deltax, double deltay)
{
	auto gApp = (GlfwWindowInterface*)glfwGetWindowUserPointer(window);
	for (auto& callback : gApp->wheelCallback)
	{
		if (callback(deltax, deltay))
		{
			break;
		}
	}
}

GlfwWindowInterface::GlfwWindowInterface(GLFWwindow* window)
	: m_window(window)
{
	assert(m_window);
}

GlfwWindowInterface::~GlfwWindowInterface()
{
	glfwDestroyWindow(m_window);
}

GlfwWindowInterface* GlfwWindowInterface::createWindow(const b3gWindowConstructionInfo& ci)
{
	auto window = glfwCreateWindow(ci.m_width, ci.m_height, ci.m_title, NULL, NULL);
	if (!window)
	{
		return nullptr;
	}

	auto p = new GlfwWindowInterface(window);
	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwSetWindowUserPointer(window, p);
	glfwSetWindowSizeCallback(window, SimpleResizeCallback);
	glfwSetCursorPosCallback(window, SimpleMouseMoveCallback);
	glfwSetMouseButtonCallback(window, SimpleMouseButtonCallback);
	glfwSetKeyCallback(window, SimpleKeyboardCallback);
	glfwSetScrollCallback(window, SimpleWheelCallback);
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);
	return p;
}

void GlfwWindowInterface::closeWindow()
{
    setRequestExit();
}

float GlfwWindowInterface::getTimeInSeconds()
{
	return glfwGetTime();
}

bool GlfwWindowInterface::requestedExit() const
{
	if (!m_window)
	{
		return true;
	}
	if (glfwWindowShouldClose(m_window))
	{
		return true;
	}
	return false;
	;
}

void GlfwWindowInterface::setRequestExit()
{
	glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

void GlfwWindowInterface::startRendering()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glfwPollEvents();
}

void GlfwWindowInterface::endRendering()
{
	glfwSwapBuffers(m_window);
}

bool GlfwWindowInterface::isModifierKeyPressed(int key)
{
	return glfwGetKey(m_window, GLFW_KEY_LEFT_ALT);
}

int GlfwWindowInterface::getWidth() const
{
	int width;
	int height;
	glfwGetWindowSize(m_window, &width, &height);
	return width;
}

int GlfwWindowInterface::getHeight() const
{
	int width;
	int height;
	glfwGetWindowSize(m_window, &width, &height);
	return height;
}

int GlfwWindowInterface::fileOpenDialog(char* fileName, int maxFileNameLength)
{
	return 0;
}
