#ifndef SORA_KEY_INFO_H
#define SORA_KEY_INFO_H

// simplely port the hge definition
// because we want cross-platform

#include "SoraPlatform.h"

#ifdef USE_HGE_KEYMAP 

#define SORA_KEY_LBUTTON	0x01
#define SORA_KEY_RBUTTON	0x02
#define SORA_KEY_MBUTTON	0x04

#define SORA_KEY_ESCAPE		0x1B
#define SORA_KEY_BACKSPACE	0x08
#define SORA_KEY_TAB		0x09
#define SORA_KEY_ENTER		0x0D
#define SORA_KEY_SPACE		0x20

#define SORA_KEY_SHIFT		0x10
#define SORA_KEY_CTRL		0x11
#define SORA_KEY_ALT		0x12

#define SORA_KEY_LWIN		0x5B
#define SORA_KEY_RWIN		0x5C
#define SORA_KEY_APPS		0x5D

#define SORA_KEY_PAUSE		0x13
#define SORA_KEY_CAPSLOCK	0x14
#define SORA_KEY_NUMLOCK	0x90
#define SORA_KEY_SCROLLLOCK	0x91

#define SORA_KEY_PGUP		0x21
#define SORA_KEY_PGDN		0x22
#define SORA_KEY_HOME		0x24
#define SORA_KEY_END		0x23
#define SORA_KEY_INSERT		0x2D
#define SORA_KEY_DELETE		0x2E

#define SORA_KEY_LEFT		0x25
#define SORA_KEY_UP			0x26
#define SORA_KEY_RIGHT		0x27
#define SORA_KEY_DOWN		0x28

#define SORA_KEY_0			0x30
#define SORA_KEY_1			0x31
#define SORA_KEY_2			0x32
#define SORA_KEY_3			0x33
#define SORA_KEY_4			0x34
#define SORA_KEY_5			0x35
#define SORA_KEY_6			0x36
#define SORA_KEY_7			0x37
#define SORA_KEY_8			0x38
#define SORA_KEY_9			0x39

#define SORA_KEY_A			0x41
#define SORA_KEY_B			0x42
#define SORA_KEY_C			0x43
#define SORA_KEY_D			0x44
#define SORA_KEY_E			0x45
#define SORA_KEY_F			0x46
#define SORA_KEY_G			0x47
#define SORA_KEY_H			0x48
#define SORA_KEY_I			0x49
#define SORA_KEY_J			0x4A
#define SORA_KEY_K			0x4B
#define SORA_KEY_L			0x4C
#define SORA_KEY_M			0x4D
#define SORA_KEY_N			0x4E
#define SORA_KEY_O			0x4F
#define SORA_KEY_P			0x50
#define SORA_KEY_Q			0x51
#define SORA_KEY_R			0x52
#define SORA_KEY_S			0x53
#define SORA_KEY_T			0x54
#define SORA_KEY_U			0x55
#define SORA_KEY_V			0x56
#define SORA_KEY_W			0x57
#define SORA_KEY_X			0x58
#define SORA_KEY_Y			0x59
#define SORA_KEY_Z			0x5A

#define SORA_KEY_GRAVE		0xC0
#define SORA_KEY_MINUS		0xBD
#define SORA_KEY_EQUALS		0xBB
#define SORA_KEY_BACKSLASH	0xDC
#define SORA_KEY_LBRACKET	0xDB
#define SORA_KEY_RBRACKET	0xDD
#define SORA_KEY_SEMICOLON	0xBA
#define SORA_KEY_APOSTROPHE	0xDE
#define SORA_KEY_COMMA		0xBC
#define SORA_KEY_PERIOD		0xBE
#define SORA_KEY_SLASH		0xBF

#define SORA_KEY_NUMPAD0	0x60
#define SORA_KEY_NUMPAD1	0x61
#define SORA_KEY_NUMPAD2	0x62
#define SORA_KEY_NUMPAD3	0x63
#define SORA_KEY_NUMPAD4	0x64
#define SORA_KEY_NUMPAD5	0x65
#define SORA_KEY_NUMPAD6	0x66
#define SORA_KEY_NUMPAD7	0x67
#define SORA_KEY_NUMPAD8	0x68
#define SORA_KEY_NUMPAD9	0x69

#define SORA_KEY_MULTIPLY	0x6A
#define SORA_KEY_DIVIDE		0x6F
#define SORA_KEY_ADD		0x6B
#define SORA_KEY_SUBTRACT	0x6D
#define SORA_KEY_DECIMAL	0x6E

#define SORA_KEY_F1			0x70
#define SORA_KEY_F2			0x71
#define SORA_KEY_F3			0x72
#define SORA_KEY_F4			0x73
#define SORA_KEY_F5			0x74
#define SORA_KEY_F6			0x75
#define SORA_KEY_F7			0x76
#define SORA_KEY_F8			0x77
#define SORA_KEY_F9			0x78
#define SORA_KEY_F10		0x79
#define SORA_KEY_F11		0x7A
#define SORA_KEY_F12		0x7B


#define SORA_INPUT_KEYDOWN			1
#define SORA_INPUT_KEYUP			2
#define SORA_INPUT_MBUTTONDOWN		3
#define SORA_INPUT_MBUTTONUP		4
#define SORA_INPUT_MOUSEMOVE		5
#define SORA_INPUT_MOUSEWHEEL		6

#define SORA_INPUT_FLAG_SHIFT		1
#define SORA_INPUT_FLAG_CTRL		2
#define SORA_INPUT_FLAG_ALT			4
#define SORA_INPUT_FLAG_CAPSLOCK	8
#define SORA_INPUT_FLAG_SCROLLLOCK	16
#define SORA_INPUT_FLAG_NUMLOCK		32
#define SORA_INPUT_FLAG_REPEAT		64

#define SORA_DIKEY_PRESSED	0x0
#define SORA_DIKEY_UP		0x10
#define SORA_DIKEY_DOWN		0x20

#define SORA_JOY_LEFT		0x40
#define SORA_JOY_RIGHT		0x50
#define SORA_JOY_UP			0x60
#define SORA_JOY_DOWN		0x70

#endif

#ifdef USE_GLFW_KEYMAP

#define GLFW_KEY_UNKNOWN      -1
#define GLFW_KEY_SPACE        32
#define GLFW_KEY_SPECIAL      256
#define GLFW_KEY_ESC          (GLFW_KEY_SPECIAL+1)
#define GLFW_KEY_F1           (GLFW_KEY_SPECIAL+2)
#define GLFW_KEY_F2           (GLFW_KEY_SPECIAL+3)
#define GLFW_KEY_F3           (GLFW_KEY_SPECIAL+4)
#define GLFW_KEY_F4           (GLFW_KEY_SPECIAL+5)
#define GLFW_KEY_F5           (GLFW_KEY_SPECIAL+6)
#define GLFW_KEY_F6           (GLFW_KEY_SPECIAL+7)
#define GLFW_KEY_F7           (GLFW_KEY_SPECIAL+8)
#define GLFW_KEY_F8           (GLFW_KEY_SPECIAL+9)
#define GLFW_KEY_F9           (GLFW_KEY_SPECIAL+10)
#define GLFW_KEY_F10          (GLFW_KEY_SPECIAL+11)
#define GLFW_KEY_F11          (GLFW_KEY_SPECIAL+12)
#define GLFW_KEY_F12          (GLFW_KEY_SPECIAL+13)
#define GLFW_KEY_F13          (GLFW_KEY_SPECIAL+14)
#define GLFW_KEY_F14          (GLFW_KEY_SPECIAL+15)
#define GLFW_KEY_F15          (GLFW_KEY_SPECIAL+16)
#define GLFW_KEY_F16          (GLFW_KEY_SPECIAL+17)
#define GLFW_KEY_F17          (GLFW_KEY_SPECIAL+18)
#define GLFW_KEY_F18          (GLFW_KEY_SPECIAL+19)
#define GLFW_KEY_F19          (GLFW_KEY_SPECIAL+20)
#define GLFW_KEY_F20          (GLFW_KEY_SPECIAL+21)
#define GLFW_KEY_F21          (GLFW_KEY_SPECIAL+22)
#define GLFW_KEY_F22          (GLFW_KEY_SPECIAL+23)
#define GLFW_KEY_F23          (GLFW_KEY_SPECIAL+24)
#define GLFW_KEY_F24          (GLFW_KEY_SPECIAL+25)
#define GLFW_KEY_F25          (GLFW_KEY_SPECIAL+26)
#define GLFW_KEY_UP           (GLFW_KEY_SPECIAL+27)
#define GLFW_KEY_DOWN         (GLFW_KEY_SPECIAL+28)
#define GLFW_KEY_LEFT         (GLFW_KEY_SPECIAL+29)
#define GLFW_KEY_RIGHT        (GLFW_KEY_SPECIAL+30)
#define GLFW_KEY_LSHIFT       (GLFW_KEY_SPECIAL+31)
#define GLFW_KEY_RSHIFT       (GLFW_KEY_SPECIAL+32)
#define GLFW_KEY_LCTRL        (GLFW_KEY_SPECIAL+33)
#define GLFW_KEY_RCTRL        (GLFW_KEY_SPECIAL+34)
#define GLFW_KEY_LALT         (GLFW_KEY_SPECIAL+35)
#define GLFW_KEY_RALT         (GLFW_KEY_SPECIAL+36)
#define GLFW_KEY_TAB          (GLFW_KEY_SPECIAL+37)
#define GLFW_KEY_ENTER        (GLFW_KEY_SPECIAL+38)
#define GLFW_KEY_BACKSPACE    (GLFW_KEY_SPECIAL+39)
#define GLFW_KEY_INSERT       (GLFW_KEY_SPECIAL+40)
#define GLFW_KEY_DEL          (GLFW_KEY_SPECIAL+41)
#define GLFW_KEY_PAGEUP       (GLFW_KEY_SPECIAL+42)
#define GLFW_KEY_PAGEDOWN     (GLFW_KEY_SPECIAL+43)
#define GLFW_KEY_HOME         (GLFW_KEY_SPECIAL+44)
#define GLFW_KEY_END          (GLFW_KEY_SPECIAL+45)
#define GLFW_KEY_KP_0         (GLFW_KEY_SPECIAL+46)
#define GLFW_KEY_KP_1         (GLFW_KEY_SPECIAL+47)
#define GLFW_KEY_KP_2         (GLFW_KEY_SPECIAL+48)
#define GLFW_KEY_KP_3         (GLFW_KEY_SPECIAL+49)
#define GLFW_KEY_KP_4         (GLFW_KEY_SPECIAL+50)
#define GLFW_KEY_KP_5         (GLFW_KEY_SPECIAL+51)
#define GLFW_KEY_KP_6         (GLFW_KEY_SPECIAL+52)
#define GLFW_KEY_KP_7         (GLFW_KEY_SPECIAL+53)
#define GLFW_KEY_KP_8         (GLFW_KEY_SPECIAL+54)
#define GLFW_KEY_KP_9         (GLFW_KEY_SPECIAL+55)
#define GLFW_KEY_KP_DIVIDE    (GLFW_KEY_SPECIAL+56)
#define GLFW_KEY_KP_MULTIPLY  (GLFW_KEY_SPECIAL+57)
#define GLFW_KEY_KP_SUBTRACT  (GLFW_KEY_SPECIAL+58)
#define GLFW_KEY_KP_ADD       (GLFW_KEY_SPECIAL+59)
#define GLFW_KEY_KP_DECIMAL   (GLFW_KEY_SPECIAL+60)
#define GLFW_KEY_KP_EQUAL     (GLFW_KEY_SPECIAL+61)
#define GLFW_KEY_KP_ENTER     (GLFW_KEY_SPECIAL+62)
#define GLFW_KEY_KP_NUM_LOCK  (GLFW_KEY_SPECIAL+63)
#define GLFW_KEY_CAPS_LOCK    (GLFW_KEY_SPECIAL+64)
#define GLFW_KEY_SCROLL_LOCK  (GLFW_KEY_SPECIAL+65)
#define GLFW_KEY_PAUSE        (GLFW_KEY_SPECIAL+66)
#define GLFW_KEY_LSUPER       (GLFW_KEY_SPECIAL+67)
#define GLFW_KEY_RSUPER       (GLFW_KEY_SPECIAL+68)
#define GLFW_KEY_MENU         (GLFW_KEY_SPECIAL+69)
#define GLFW_KEY_LAST         GLFW_KEY_MENU

/* Mouse button definitions */
#define GLFW_MOUSE_BUTTON_1      0
#define GLFW_MOUSE_BUTTON_2      1
#define GLFW_MOUSE_BUTTON_3      2
#define GLFW_MOUSE_BUTTON_4      3
#define GLFW_MOUSE_BUTTON_5      4
#define GLFW_MOUSE_BUTTON_6      5
#define GLFW_MOUSE_BUTTON_7      6
#define GLFW_MOUSE_BUTTON_8      7
#define GLFW_MOUSE_BUTTON_LAST   GLFW_MOUSE_BUTTON_8

/* Mouse button aliases */
#define GLFW_MOUSE_BUTTON_LEFT   GLFW_MOUSE_BUTTON_1
#define GLFW_MOUSE_BUTTON_RIGHT  GLFW_MOUSE_BUTTON_2
#define GLFW_MOUSE_BUTTON_MIDDLE GLFW_MOUSE_BUTTON_3


/* Joystick identifiers */
#define GLFW_JOYSTICK_1          0
#define GLFW_JOYSTICK_2          1
#define GLFW_JOYSTICK_3          2
#define GLFW_JOYSTICK_4          3
#define GLFW_JOYSTICK_5          4
#define GLFW_JOYSTICK_6          5
#define GLFW_JOYSTICK_7          6
#define GLFW_JOYSTICK_8          7
#define GLFW_JOYSTICK_9          8
#define GLFW_JOYSTICK_10         9
#define GLFW_JOYSTICK_11         10
#define GLFW_JOYSTICK_12         11
#define GLFW_JOYSTICK_13         12
#define GLFW_JOYSTICK_14         13
#define GLFW_JOYSTICK_15         14
#define GLFW_JOYSTICK_16         15
#define GLFW_JOYSTICK_LAST       GLFW_JOYSTICK_16


/* Joystick identifiers */
#define GLFW_JOYSTICK_1          0
#define GLFW_JOYSTICK_2          1
#define GLFW_JOYSTICK_3          2
#define GLFW_JOYSTICK_4          3
#define GLFW_JOYSTICK_5          4
#define GLFW_JOYSTICK_6          5
#define GLFW_JOYSTICK_7          6
#define GLFW_JOYSTICK_8          7
#define GLFW_JOYSTICK_9          8
#define GLFW_JOYSTICK_10         9
#define GLFW_JOYSTICK_11         10
#define GLFW_JOYSTICK_12         11
#define GLFW_JOYSTICK_13         12
#define GLFW_JOYSTICK_14         13
#define GLFW_JOYSTICK_15         14
#define GLFW_JOYSTICK_16         15
#define GLFW_JOYSTICK_LAST       GLFW_JOYSTICK_16

#define SORA_KEY_LBUTTON	GLFW_MOUSE_BUTTON_LEFT
#define SORA_KEY_RBUTTON	GLFW_MOUSE_BUTTON_RIGHT
#define SORA_KEY_MBUTTON	GLFW_MOUSE_BUTTON_MIDDLE

#define SORA_KEY_ESCAPE		GLFW_KEY_ESC
#define SORA_KEY_BACKSPACE	GLFW_KEY_BACKSPACE
#define SORA_KEY_TAB		GLFW_KEY_TAB
#define SORA_KEY_ENTER		GLFW_KEY_ENTER
#define SORA_KEY_SPACE		GLFW_KEY_SPACE

#define SORA_KEY_SHIFT		GLFW_KEY_LSHIFT
#define SORA_KEY_CTRL		GLFW_KEY_LCTRL
#define SORA_KEY_ALT		GLFW_KEY_LALT

#define SORA_KEY_LWIN		0x5B
#define SORA_KEY_RWIN		0x5C
#define SORA_KEY_APPS		0x5D

#define SORA_KEY_PAUSE		GLFW_KEY_PAUSE
#define SORA_KEY_CAPSLOCK	GLFW_KEY_CAPS_LOCK
#define SORA_KEY_NUMLOCK	GLFW_KEY_KP_NUM_LOCK
#define SORA_KEY_SCROLLLOCK	GLFW_KEY_SCROLL_LOCK

#define SORA_KEY_PGUP		GLFW_KEY_PAGEUP
#define SORA_KEY_PGDN		GLFW_KEY_PAGEDOWN
#define SORA_KEY_HOME		GLFW_KEY_HOME
#define SORA_KEY_END		GLFW_KEY_END
#define SORA_KEY_INSERT		GLFW_KEY_INSERT
#define SORA_KEY_DELETE		GLFW_KEY_DEL

#define SORA_KEY_LEFT		GLFW_KEY_LEFT
#define SORA_KEY_UP			GLFW_KEY_UP
#define SORA_KEY_RIGHT		GLFW_KEY_RIGHT
#define SORA_KEY_DOWN		GLFW_KEY_DOWN	

#define SORA_KEY_0			0x30
#define SORA_KEY_1			0x31
#define SORA_KEY_2			0x32
#define SORA_KEY_3			0x33
#define SORA_KEY_4			0x34
#define SORA_KEY_5			0x35
#define SORA_KEY_6			0x36
#define SORA_KEY_7			0x37
#define SORA_KEY_8			0x38
#define SORA_KEY_9			0x39

#define SORA_KEY_A			0x41
#define SORA_KEY_B			0x42
#define SORA_KEY_C			0x43
#define SORA_KEY_D			0x44
#define SORA_KEY_E			0x45
#define SORA_KEY_F			0x46
#define SORA_KEY_G			0x47
#define SORA_KEY_H			0x48
#define SORA_KEY_I			0x49
#define SORA_KEY_J			0x4A
#define SORA_KEY_K			0x4B
#define SORA_KEY_L			0x4C
#define SORA_KEY_M			0x4D
#define SORA_KEY_N			0x4E
#define SORA_KEY_O			0x4F
#define SORA_KEY_P			0x50
#define SORA_KEY_Q			0x51
#define SORA_KEY_R			0x52
#define SORA_KEY_S			0x53
#define SORA_KEY_T			0x54
#define SORA_KEY_U			0x55
#define SORA_KEY_V			0x56
#define SORA_KEY_W			0x57
#define SORA_KEY_X			0x58
#define SORA_KEY_Y			0x59
#define SORA_KEY_Z			0x5A

#define SORA_KEY_GRAVE		0xC0
#define SORA_KEY_MINUS		0xBD
#define SORA_KEY_EQUALS		0xBB
#define SORA_KEY_BACKSLASH	0xDC
#define SORA_KEY_LBRACKET	0xDB
#define SORA_KEY_RBRACKET	0xDD
#define SORA_KEY_SEMICOLON	0xBA
#define SORA_KEY_APOSTROPHE	0xDE
#define SORA_KEY_COMMA		0xBC
#define SORA_KEY_PERIOD		0xBE
#define SORA_KEY_SLASH		0xBF

#define SORA_KEY_NUMPAD0	GLFW_KEY_KP_0
#define SORA_KEY_NUMPAD1	GLFW_KEY_KP_1
#define SORA_KEY_NUMPAD2	GLFW_KEY_KP_2
#define SORA_KEY_NUMPAD3	GLFW_KEY_KP_3
#define SORA_KEY_NUMPAD4	GLFW_KEY_KP_4
#define SORA_KEY_NUMPAD5	GLFW_KEY_KP_5
#define SORA_KEY_NUMPAD6	GLFW_KEY_KP_6
#define SORA_KEY_NUMPAD7	GLFW_KEY_KP_7
#define SORA_KEY_NUMPAD8	GLFW_KEY_KP_8
#define SORA_KEY_NUMPAD9	GLFW_KEY_KP_9

#define SORA_KEY_MULTIPLY	GLFW_KEY_KP_MULTIPLY
#define SORA_KEY_DIVIDE		GLFW_KEY_KP_DIVIDE
#define SORA_KEY_ADD		GLFW_KEY_KP_ADD
#define SORA_KEY_SUBTRACT	GLFW_KEY_KP_SUBTRACT
#define SORA_KEY_DECIMAL	GLFW_KEY_KP_DECIMAL

#define SORA_KEY_F1			GLFW_KEY_F1
#define SORA_KEY_F2			GLFW_KEY_F2
#define SORA_KEY_F3			GLFW_KEY_F3
#define SORA_KEY_F4			GLFW_KEY_F4
#define SORA_KEY_F5			GLFW_KEY_F5
#define SORA_KEY_F6			GLFW_KEY_F6
#define SORA_KEY_F7			GLFW_KEY_F7
#define SORA_KEY_F8			GLFW_KEY_F8
#define SORA_KEY_F9			GLFW_KEY_F9
#define SORA_KEY_F10		GLFW_KEY_F10
#define SORA_KEY_F11		GLFW_KEY_F11
#define SORA_KEY_F12		GLFW_KEY_F12

#define GLFW_RELEASE            0
#define GLFW_PRESS              1


#define SORA_INPUT_KEYDOWN			GLFW_PRESS
#define SORA_INPUT_KEYUP			GLFW_RELEASE
#define SORA_INPUT_MBUTTONDOWN		3
#define SORA_INPUT_MBUTTONUP		4
#define SORA_INPUT_MOUSEMOVE		5
#define SORA_INPUT_MOUSEWHEEL		6

#define SORA_INPUT_FLAG_SHIFT		1
#define SORA_INPUT_FLAG_CTRL		2
#define SORA_INPUT_FLAG_ALT			4
#define SORA_INPUT_FLAG_CAPSLOCK	8
#define SORA_INPUT_FLAG_SCROLLLOCK	16
#define SORA_INPUT_FLAG_NUMLOCK		32
#define SORA_INPUT_FLAG_REPEAT		64

#define SORA_DIKEY_PRESSED	0x0
#define SORA_DIKEY_UP		0x10
#define SORA_DIKEY_DOWN		0x20

#define SORA_JOY_LEFT		0x40
#define SORA_JOY_RIGHT		0x50
#define SORA_JOY_UP			0x60
#define SORA_JOY_DOWN		0x70

#endif


typedef struct tagSoraKeyEvent {
	int		type;			// event type
	int		key;			// key code
	int		flags;			// event flags
	int		chr;			// character code
	int		wheel;			// wheel shift
	float	x;				// mouse cursor x-coordinate
	float	y;				// mouse cursor y-coordinate
} SoraKeyEvent;

#endif