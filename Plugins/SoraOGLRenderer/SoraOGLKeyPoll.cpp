/*
 *  SoraOGLKeyPoll.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/12/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */
#include "SoraOGLKeyPoll.h"
#include "glfw/GL/glfw.h"
#include "Debug/SoraInternalLogger.h"
#include "SoraEventManager.h"
#include <vector>

namespace sora {

	typedef std::vector<SoraKeyEvent> VKEYEVENT_POLL;
	static VKEYEVENT_POLL keyPoll;

	char toasciiWithFlag(int key, int flag) {
		char chr;
		if(key >= 0 && key <= 255) {
			chr = toascii(key);
			if(flag & SORA_INPUT_FLAG_SHIFT) {
				switch(key) {
					case SORA_KEY_1: chr = '!'; break;
					case SORA_KEY_2: chr = '@'; break;
					case SORA_KEY_3: chr = '#'; break;
					case SORA_KEY_4: chr = '$'; break;
					case SORA_KEY_5: chr = '%'; break;
					case SORA_KEY_6: chr = '^'; break;
					case SORA_KEY_7: chr = '&'; break;
					case SORA_KEY_8: chr = '*'; break;
					case SORA_KEY_9: chr = '('; break;
					case SORA_KEY_0: chr = ')'; break;
					case SORA_KEY_GRAVE: chr = '~';  break;
					case SORA_KEY_EQUALS: chr = '+';  break;
					case SORA_KEY_BACKSLASH: chr = '|';  break;
					case SORA_KEY_LBRACKET: chr = '{';  break;
					case SORA_KEY_RBRACKET: chr = '}';  break;
					case SORA_KEY_SEMICOLON: chr = ':';  break;
					case SORA_KEY_APOSTROPHE: chr = '"';  break;
					case SORA_KEY_COMMA: chr = '<';  break;
					case SORA_KEY_PERIOD: chr = '>';  break;
					case SORA_KEY_SLASH: chr = '?';  break;
				}
			} else {
				if(key >= SORA_KEY_A && key <= SORA_KEY_Z)
					chr = tolower(chr);
			}
		}
		else 
			chr = 0;

		return chr;
	}
	
	void glfwKeyCallback(int key, int action) {
		SoraKeyEvent ev;
		if(action == GLFW_PRESS) ev.type = SORA_INPUT_KEYDOWN;
		else ev.type = SORA_INPUT_KEYUP;

		ev.key = key;
		ev.flags = 0;
		if(glfwGetKey(GLFW_KEY_LSHIFT) == GLFW_PRESS || glfwGetKey(GLFW_KEY_RSHIFT) == GLFW_PRESS) ev.flags |= SORA_INPUT_FLAG_SHIFT;
		if(glfwGetKey(GLFW_KEY_LALT) == GLFW_PRESS || glfwGetKey(GLFW_KEY_RALT) == GLFW_PRESS) ev.flags |= SORA_INPUT_FLAG_ALT;
		if(glfwGetKey(GLFW_KEY_LCTRL) == GLFW_PRESS || glfwGetKey(GLFW_KEY_RCTRL) == GLFW_PRESS) ev.flags |= SORA_INPUT_FLAG_CTRL;
		if(glfwGetKey(GLFW_KEY_CAPS_LOCK) == GLFW_PRESS) ev.flags |= SORA_INPUT_FLAG_CAPSLOCK;
		if(glfwGetKey(GLFW_KEY_SCROLL_LOCK)) ev.flags |= SORA_INPUT_FLAG_SCROLLLOCK;

		ev.chr = toasciiWithFlag(ev.key, ev.flags);
		ev.wheel = glfwGetMouseWheel();
		int32 x, y;
		glfwGetMousePos(&x, &y);
		ev.x = x;
		ev.y = y;
				
		SORA_EVENT_MANAGER->publishInputEvent(&ev);
		keyPoll.push_back(ev);
	}

	void clearPoll() {
		keyPoll.clear();
	}

	bool getEv(SoraKeyEvent& ev) {
		if(keyPoll.size() != 0) {
			ev = keyPoll.back();
			keyPoll.pop_back();
			return true;
		} else return false;
	}

} // namespace sora


