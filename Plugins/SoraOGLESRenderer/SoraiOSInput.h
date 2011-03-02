/*
 *  SoraiOSInput.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 2/13/11.
 *  Copyright 2011 Studio Symphonie. All rights reserved.
 *
 */

#ifndef _SORA_IOS_INPUT_H_
#define _SORA_IOS_INPUT_H_

#include "SoraPlatform.h"
#include "SoraInput.h"

namespace sora {
	
	class SoraiOSInput: public SoraInput {
	public:
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
		
		bool	joyKeyPressed(int32 key) { return false; }
		bool	joyKeyDown(int32 key) { return false; }
		bool	joyKeyUp(int32 key) { return false; }
		bool	joyKeyState(int32 key, unsigned char state) { return false; }
		bool	setJoyKey(int32 key) { return false; }
		bool	hasJoy() { return false; }
		
		int32	getGesture();
	};
}

#endif // _SORA_IOS_INPUT_H_
