#ifndef COMMON_CALLBACKS_H
#define COMMON_CALLBACKS_H
#include <functional>

using b3WheelCallback = std::function<void(float deltax, float deltay)>;
using b3ResizeCallback = std::function<void(float width, float height)>;
using b3MouseMoveCallback = std::function<void(float x, float y)>;
using b3MouseButtonCallback = std::function<void(int button, int state, float x, float y)>;
using b3KeyboardCallback = std::function<void(int keycode, int state)>;
using b3RenderCallback = std::function<void()>;

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

#endif
