#ifndef SORA_INPUT_H_
#define SORA_INPUT_H_

#include "SoraPlatform.h"
#include "SoraKeyInfo.h"

namespace sora {

	/*
		Abstract input class that defines input APIs
	*/

	class SoraInput {
	public:
		// inputs
		virtual void	getMousePos(float32 *x, float32 *y) = 0;
		virtual void	setMousePos(float32 x, float32 y) = 0;
		virtual float32 getMousePosX() = 0;
		virtual float32 getMousePosY() = 0;
		virtual int32	getMouseWheel()	= 0;
		virtual bool	isMouseOver() = 0;
		virtual bool	keyDown(int32 key) = 0;
		virtual bool	keyUp(int32 key) = 0;
		virtual int32	getKeyState(int32 key) = 0;
		virtual char*	getKeyName(int32 key) = 0;
		virtual bool	getKeyEvent(SoraKeyEvent& ev) = 0;
		virtual bool	joyKeyPressed(int32 key) = 0;
		virtual bool	joyKeyDown(int32 key) = 0;
		virtual bool	joyKeyUp(int32 key) = 0;
		virtual bool	joyKeyState(int32 key, unsigned char state) = 0;
		virtual bool	setJoyKey(int32 key) = 0;
		virtual bool	hasJoy() = 0;
		
#ifdef OS_IOS
		/* ***** corresponding to mobile touch screens only */
		virtual int32	getGesture() = 0;
#endif
	};

} // namespace sora


#endif