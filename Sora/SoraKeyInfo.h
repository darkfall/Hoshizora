#ifndef SORA_KEY_INFO_H
#define SORA_KEY_INFO_H

// simplely port the hge definition
// because we want cross-platform

#include "SoraPlatform.h"

#include "SoraEvent.h"

enum {
	SORA_KEY_0 = 0x30,
	SORA_KEY_1,
	SORA_KEY_2,
	SORA_KEY_3,
	SORA_KEY_4,
	SORA_KEY_5,
	SORA_KEY_6,
	SORA_KEY_7,
	SORA_KEY_8,
	SORA_KEY_9,
	SORA_KEY_10,
	
	SORA_KEY_A = 0x41,
	SORA_KEY_B,
	SORA_KEY_C,
	SORA_KEY_D,
	SORA_KEY_E,
	SORA_KEY_F,
	SORA_KEY_G,
	SORA_KEY_H,
	SORA_KEY_I,
	SORA_KEY_J,
	SORA_KEY_K,
	SORA_KEY_L,
	SORA_KEY_M,
	SORA_KEY_N,
	SORA_KEY_O,
	SORA_KEY_P,
	SORA_KEY_Q,
	SORA_KEY_R,
	SORA_KEY_S,
	SORA_KEY_T,
	SORA_KEY_U,
	SORA_KEY_V,
	SORA_KEY_W,
	SORA_KEY_X,
	SORA_KEY_Y,
	SORA_KEY_Z,
	
	SORA_KEY_F1 = 0x70,
	SORA_KEY_F2,
	SORA_KEY_F3,
	SORA_KEY_F4,
	SORA_KEY_F5,
	SORA_KEY_F6,
	SORA_KEY_F7,
	SORA_KEY_F8,
	SORA_KEY_F9,
	SORA_KEY_F10,
	SORA_KEY_F11,
	SORA_KEY_F12,
	
	SORA_KEY_NUMPAD0 = 0x60,
	SORA_KEY_NUMPAD1,
	SORA_KEY_NUMPAD2,
	SORA_KEY_NUMPAD3,
	SORA_KEY_NUMPAD4,
	SORA_KEY_NUMPAD5,
	SORA_KEY_NUMPAD6,
	SORA_KEY_NUMPAD7,
	SORA_KEY_NUMPAD8,
	SORA_KEY_NUMPAD9,
	
	SORA_KEY_ESCAPE		= 0x1B,
	SORA_KEY_BACKSPACE	= 0x08,
	SORA_KEY_TAB		= 0x09,
	SORA_KEY_ENTER		= 0x0D,
	SORA_KEY_SPACE		= 0x20,
	
	SORA_KEY_SHIFT	= 0x10,
	SORA_KEY_CTRL	= 0x11,
	SORA_KEY_ALT	= 0x12,
	
	SORA_KEY_PAUSE		= 0x13,
	SORA_KEY_CAPSLOCK	= 0x14,
	SORA_KEY_NUMLOCK	= 0x90,
	SORA_KEY_SCROLLLOCK = 0x91,
	
	SORA_KEY_PGUP		= 0x21,
	SORA_KEY_PGDN		= 0x22,
	SORA_KEY_HOME		= 0x24,
	SORA_KEY_END		= 0x23,
	SORA_KEY_INSERT		= 0x2D,
	SORA_KEY_DELETE		= 0x2E,
	
	SORA_KEY_LEFT	= 0x25,
	SORA_KEY_UP		= 0x26,
	SORA_KEY_RIGHT	= 0x27,
	SORA_KEY_DOWN	= 0x28,
	
	SORA_KEY_GRAVE		= 0xC0,
	SORA_KEY_MINUS		= 0xBD,
	SORA_KEY_EQUALS		= 0xBB,
	SORA_KEY_BACKSLASH	= 0xDC,
	SORA_KEY_LBRACKET	= 0xDB,
	SORA_KEY_RBRACKET	= 0xDD,
	SORA_KEY_SEMICOLON	= 0xBA,
	SORA_KEY_APOSTROPHE = 0xDE,
	SORA_KEY_COMMA		= 0xBC,
	SORA_KEY_PERIOD		= 0xBE,
	SORA_KEY_SLASH		= 0xBF,
	
	SORA_KEY_LBUTTON = 0x01,
	SORA_KEY_RBUTTON = 0x02,
	SORA_KEY_MBUTTON = 0x04,
	
	SORA_KEY_LWIN = 0x5B,
	SORA_KEY_RWIN = 0x5C,
	SORA_KEY_APPS = 0x5D,
};

enum {
	SORA_JOYSTICK_1 = 0,
	SORA_JOYSTICK_2,
	SORA_JOYSTICK_3,
	SORA_JOYSTICK_4,
	SORA_JOYSTICK_5,
	SORA_JOYSTICK_6,
	SORA_JOYSTICK_7,
	SORA_JOYSTICK_8,
	SORA_JOYSTICK_9,
	SORA_JOYSTICK_10,
	SORA_JOYSTICK_11,
	SORA_JOYSTICK_12,
	SORA_JOYSTICK_13,
	SORA_JOYSTICK_14,
	SORA_JOYSTICK_15,
	SORA_JOYSTICK_16,
};

enum {
	SORA_INPUT_PRESSED = 0,
	SORA_INPUT_KEYDOWN = 1,
	SORA_INPUT_KEYUP = 2,
	SORA_INPUT_MBUTTONDOWN =3,
	SORA_INPUT_MBUTTONUP = 4,
	SORA_INPUT_MOUSEMOVE = 5,
	SORA_INPUT_MOUSEWHEEL = 6,
};

enum {
	SORA_INPUT_FLAG_SHIFT		= 1,
	SORA_INPUT_FLAG_CTRL		= 2,
	SORA_INPUT_FLAG_ALT			= 4,
	SORA_INPUT_FLAG_CAPSLOCK	= 8,
	SORA_INPUT_FLAG_SCROLLLOCK	= 16,
	SORA_INPUT_FLAG_NUMLOCK		= 32,
	SORA_INPUT_FLAG_REPEAT		= 64,
};

enum {
	SORA_JOYSTICK_PRESSED = 0,
	SORA_JOYSTICK_UP	  = 1,
	SORA_JOYSTICK_DOWN	  = 2,
};

namespace sora {
	
    bool isKeyPrintable(int key);
    char toasciiWithFlag(int key, int flag);
	
	class SORA_API SoraKeyEvent: public SoraEvent {
	public:
		int		type;			// event type
		int		key;			// key code
		int		flags;			// event flags
		int		chr;			// character code
		int		wheel;			// wheel shift
		float	x;				// mouse cursor x-coordinate
		float	y;				// mouse cursor y-coordinate
        bool    isIME;          // is ime key
        
        int getType() const;
        bool isIMEKey() const;
		
		bool isKeyDown() const;
		bool isKeyUp() const;
		
		int getKey() const;
		bool isKeyPressed(int k) const;
		bool isKeyReleased(int k) const;
		
		bool isShiftFlag() const;
		bool isCtrlFlag() const;
		bool isAltFlag() const;
		
        bool isNumber() const;
        bool isLetter() const;
        bool isPrintable() const;
		
#ifndef SORA_USE_RTTI
		SORA_EVENT_IDENTIFIER(18446744071251492855ULL);
#endif
	};
    
    class SORA_API SoraMouseEvent: public SoraKeyEvent {
    public:
        enum {
            None = -1,
            Moved = 0,
            Pressed,
            Released,
            Dragged,
            WheelDown,
            WheelUp,
        };
        
        SoraMouseEvent();
        
        SoraMouseEvent(int key,
                       int type,
                       float x,
                       float y,
                       int wheel);
        
       
        int clickCount;
        bool leftBtnDown;
        bool rightBtnDown;
        bool middleBtnDown;
        float dragDirection;
        
        int getClickCount() const;
        float getX() const;
        float getY() const;
        float getDragDirection() const;
        
        bool isMouseLButtonDown() const;
        bool isMouseMButtonDown() const;
		bool isMouseRButtonDown() const;
		
		bool isMouseLButtonUp() const;
        bool isMouseMButtonUp() const;
		bool isMouseRButtonUp() const;
        
        
        
#ifndef SORA_USE_RTTI
        SORA_EVENT_IDENTIFIER(18446744070821151360ULL);
#endif
    };
    
    class SORA_API SoraJoystickEvent: public SoraKeyEvent {
    public:
        // which joystick
        int joystickId;    
        
        int getJoystickId() const;
        
#ifndef SORA_USE_RTTI
        SORA_EVENT_IDENTIFIER(18446744073365881885ULL);
#endif
    };
}

#endif