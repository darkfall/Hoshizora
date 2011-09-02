/*
 *  SoraOGLInput.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/12/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "SoraOGLInputGLFW3.h"

#include "GL/glfw3.h"

#include <vector>

#include "SoraOGLKeyPoll.h"

namespace sora {
	
	SoraOGLInput::SoraOGLInput() {
		glfwSetKeyCallback(glfwKeyCallback);
	}
	
	void SoraOGLInput::setWindowHandle(ulong32 window) {
		_window = (GLFWwindow)window;
	}

	void SoraOGLInput::getMousePos(float32 *x, float32 *y) {
		int32 ix, iy;
		glfwGetMousePos(_window, &ix, &iy);
		*x = (float32)ix; *y = (float32)iy; 
	}

	void SoraOGLInput::setMousePos(float32 x, float32 y) {
		glfwSetMousePos(_window, (int32)x, (int32)y);
	}

	float32 SoraOGLInput::getMousePosX() {
		int32 ix, iy;
		glfwGetMousePos(_window, &ix, &iy);
		return (float32)ix;
	}

	float32 SoraOGLInput::getMousePosY() {
		int32 ix, iy;
		glfwGetMousePos(_window, &ix, &iy);
		return (float32)iy;
	}

	int32 SoraOGLInput::getMouseWheel() {
		//return glfwGetMouseWheel();
		return 0;
	}

	bool SoraOGLInput::isMouseOver() {
		return false;
	}

	bool SoraOGLInput::keyDown(int32 key) {
		if(key >= GLFW_MOUSE_BUTTON_1 && key <= GLFW_MOUSE_BUTTON_8)
			return (glfwGetMouseButton(_window, key) == GLFW_PRESS);
		return (glfwGetKey(_window, (key))==GLFW_PRESS);
	}

	bool SoraOGLInput::keyUp(int32 key) {
		if(key >= GLFW_MOUSE_BUTTON_1 && key <= GLFW_MOUSE_BUTTON_8)
			return (glfwGetMouseButton(_window, key) == GLFW_RELEASE);
		return (glfwGetKey(_window, (key))==GLFW_RELEASE);
	}

	int32 SoraOGLInput::getKeyState(int32 key) {
		if(key >= GLFW_MOUSE_BUTTON_1 && key <= GLFW_MOUSE_BUTTON_8)
			return (glfwGetMouseButton(_window, key)==GLFW_PRESS)?SORA_INPUT_KEYDOWN:SORA_INPUT_KEYUP;
		return glfwGetKey(_window, (key))==GLFW_PRESS?SORA_INPUT_KEYDOWN:SORA_INPUT_KEYUP;
	}
	
	char* SoraOGLInput::getKeyName(int32 key) {
		static char ckey[2];
		ckey[0] = (char)key;
		ckey[1] = '\0';
		return ckey;
	}
	
	bool SoraOGLInput::getKeyEvent(SoraKeyEvent& ev) {
		return getEv(ev);
	}
	
	bool SoraOGLInput::joyKeyPressed(int32 key) {
		return false;
	}
	
	bool SoraOGLInput::joyKeyDown(int32 key) {
		return false;
	}
	
	bool SoraOGLInput::joyKeyUp(int32 key) {
		return false;
	}
	
	bool SoraOGLInput::joyKeyState(int32 key, unsigned char state) {
		return false;
	}
	
	bool SoraOGLInput::setJoyKey(int32 key) {
		return false;
	}
	
	bool SoraOGLInput::hasJoy() {
		return false;
	}
	
	int32 translateSpeicalKey(int32 okey) {
		return 0;
	}
	
} // namespace sora