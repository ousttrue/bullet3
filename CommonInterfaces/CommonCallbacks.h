#pragma once
#include <functional>

enum ButtonFlags
{
	ButtonFlagsNone = 0,
	ButtonFlagsAlt = 1,
	ButtonFlagsCtrl = 1 << 1,
};

enum
{
	B3G_ESCAPE = 27,
	B3G_SPACE = 32,
	B3G_F1 = 0xff00,
	B3G_F2,
	B3G_F3,
	B3G_F4,
	B3G_F5,
	B3G_F6,
	B3G_F7,
	B3G_F8,
	B3G_F9,
	B3G_F10,
	B3G_F11,
	B3G_F12,
	B3G_F13,
	B3G_F14,
	B3G_F15,
	B3G_LEFT_ARROW,
	B3G_RIGHT_ARROW,
	B3G_UP_ARROW,
	B3G_DOWN_ARROW,
	B3G_PAGE_UP,
	B3G_PAGE_DOWN,
	B3G_END,
	B3G_HOME,
	B3G_INSERT,
	B3G_DELETE,
	B3G_BACKSPACE,
	B3G_SHIFT,
	B3G_CONTROL,
	B3G_ALT,
	B3G_RETURN,
};

using b3ResizeCallback = std::function<void(float width, float height)>;
using b3RenderCallback = std::function<void()>;
// input callback return true if cosume the event
using b3WheelCallback = std::function<bool(float deltax, float deltay)>;
using b3MouseMoveCallback = std::function<bool(float x, float y)>;
using b3MouseButtonCallback = std::function<bool(int button, int state, float x, float y, ButtonFlags flags)>;
using b3KeyboardCallback = std::function<bool(int keycode, int state)>;
