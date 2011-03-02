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


#include <vector>

namespace sora {

	typedef std::vector<SoraKeyEvent> VKEYEVENT_POLL;
	static VKEYEVENT_POLL keyPoll;

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

		if(key > 0 && key < 255)
			ev.chr = (char)key;
		else ev.chr = 0;
		ev.wheel = glfwGetMouseWheel();
		int32 x, y;
		glfwGetMousePos(&x, &y);
		ev.x = x;
		ev.y = y;
		
#ifdef _DEBUG
		printf("ev, char: %c, wheel: %d, x, y: %d, %d, flag: %d, key: %d\n", ev.chr, ev.wheel, x, y, ev.flags, ev.key);
#endif

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


