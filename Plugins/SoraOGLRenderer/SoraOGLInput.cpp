/*
 *  SoraOGLInput.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/12/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "SoraOGLInput.h"

#include "glfw/GL/glfw.h"

#include <vector>

#include "SoraOGLKeyPoll.h"

namespace sora {
	
	SoraOGLInput::SoraOGLInput() {
		glfwSetKeyCallback(glfwKeyCallback);
		glfwSetMouseButtonCallback(glfwMouseCallback);
	}

	void SoraOGLInput::getMousePos(float32 *x, float32 *y) {
		int32 ix, iy;
		glfwGetMousePos(&ix, &iy);
		*x = (float32)ix; *y = (float32)iy; 
	}

	void SoraOGLInput::setMousePos(float32 x, float32 y) {
		glfwSetMousePos((int32)x, (int32)y);
	}

	float32 SoraOGLInput::getMousePosX() {
		int32 ix, iy;
		glfwGetMousePos(&ix, &iy);
		return (float32)ix;
	}

	float32 SoraOGLInput::getMousePosY() {
		int32 ix, iy;
		glfwGetMousePos(&ix, &iy);
		return (float32)iy;
	}

	int32 SoraOGLInput::getMouseWheel() {
		return glfwGetMouseWheel();
	}

	bool SoraOGLInput::isMouseOver() {
		return false;
	}

	bool SoraOGLInput::keyDown(int32 key) {
		if(key >= SORA_KEY_LBUTTON && key <= SORA_KEY_MBUTTON)
			return (glfwGetMouseButton(sorakey_to_glfwkey(key)) == GLFW_PRESS);
		return (glfwGetKey(sorakey_to_glfwkey(key))==GLFW_PRESS);
	}

	bool SoraOGLInput::keyUp(int32 key) {
		if(key >= SORA_KEY_LBUTTON && key <= SORA_KEY_MBUTTON)
			return (glfwGetMouseButton(sorakey_to_glfwkey(key)) == GLFW_RELEASE);
		return (glfwGetKey(sorakey_to_glfwkey(key))==GLFW_RELEASE);
	}

	int32 SoraOGLInput::getKeyState(int32 key) {
		if(key >= SORA_KEY_LBUTTON && key <= SORA_KEY_MBUTTON)
			return (glfwGetMouseButton(sorakey_to_glfwkey(key))==GLFW_PRESS)?SORA_INPUT_KEYDOWN:SORA_INPUT_KEYUP;
		return glfwGetKey(sorakey_to_glfwkey(key))==GLFW_PRESS?SORA_INPUT_KEYDOWN:SORA_INPUT_KEYUP;
	}
	
	char* SoraOGLInput::getKeyName(int32 key) {
		char name[2];
		name[0] = toascii(key);
		name[1] = '\0';
		return name;
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