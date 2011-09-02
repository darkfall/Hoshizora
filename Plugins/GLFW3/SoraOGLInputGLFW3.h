/*
 *  SoraOGLInput.h
 *  Sora
 *
 *  Created by GriffinBu on 12/12/10.
 *  Copyright 2010 GameMaster. All rights reserved.
 *
 */

#ifndef SORA_OGL_INPUT_H_
#define SORA_OGL_INPUT_H_

#include "SoraInput.h"

namespace sora {
	
	class SoraOGLInput: public SoraInput {
	public:
		SoraOGLInput();
		
		void setWindowHandle(ulong32 window);
		
		void	getMousePos(float32 *x, float32 *y);
		void	setMousePos(float32 x, float32 y);
		float32 getMousePosX();
		float32 getMousePosY();
		int32	getMouseWheel();
		bool	isMouseOver();
		bool	keyDown(int32 key);
		bool	keyUp(int32 key);
		int32	getKeyState(int32 key);
		char*	getKeyName(int32 key);
		bool	getKeyEvent(SoraKeyEvent& ev);
		bool	joyKeyPressed(int32 key);
		bool	joyKeyDown(int32 key);
		bool	joyKeyUp(int32 key);
		bool	joyKeyState(int32 key, unsigned char state);
		bool	setJoyKey(int32 key);
		bool	hasJoy();
		
	private:
		void* _window;
	};
	
} // namespace sora

#endif

