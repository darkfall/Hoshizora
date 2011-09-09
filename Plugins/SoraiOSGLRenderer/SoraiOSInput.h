/*
 *  SoraiOSInput.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 2/13/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#ifndef _SORA_IOS_INPUT_H_
#define _SORA_IOS_INPUT_H_

#include "SoraPlatform.h"
#include "SoraInput.h"

#include "SoraiOSTouchInfo.h"
#include "SoraSingleton.h"
#include "SoraFrameListener.h"
#include <vector>

namespace sora {
	
	class SoraiOSTouch;
	
	class SoraiOSInput: public SoraInput {
	public:
		SoraiOSInput();
		// simple delegate methods for guichan
		void	getMousePos(float32 *x, float32 *y);
		void	setMousePos(float32 x, float32 y);
		float32 getMousePosX();
		float32 getMousePosY();
		
		bool	keyDown(int32 key);
		bool	keyUp(int32 key);
		int32	getKeyState(int32 key);
		
		
		// for touch event, use SoraiOSTouch class
		bool	getKeyEvent(SoraKeyEvent& ev) { return false; }
		
		int32	getMouseWheel() { return false; }
		
		bool	isMouseOver() { return true; }
		char*	getKeyName(int32 key) { return NULL; }
		
		bool	joyKeyPressed(int32 key) { return false; }
		bool	joyKeyDown(int32 key) { return false; }
		bool	joyKeyUp(int32 key) { return false; }
		bool	joyKeyState(int32 key, unsigned char state) { return false; }
		bool	setJoyKey(int32 key) { return false; }
		bool	hasJoy() { return false; }
		
		int32	getGesture() { return 0; }
		
		void setWindowHandle(ulong32 handle) {}
		
	private:
		SoraiOSTouch* pTouch;
	};
	
	class SoraiOSTouch: public SoraSingleton<SoraiOSTouch> {
		friend class SoraSingleton<SoraiOSTouch>;
		friend class SoraiOSInputDelegate;
		
		
		class SoraiOSTouchFrameListener: public SoraFrameListener {
		public:
			SoraiOSTouchFrameListener(SoraiOSTouch* pTouch): touch(pTouch) {}
			void onFrameStart() { }
			void onFrameEnd() {
				// flush every touch event when a frame ends
				if(touch)
					touch->flush();
			}
			
			SoraiOSTouch* touch;
		};
		
		SoraiOSTouch();
		
	public:
		typedef std::vector<SoraiOSTouchInfo> TouchInfos;
		
		void flush() {
			mTouchInfos.clear();
			mTouchInfos.reserve(5);
			touchCount = 0;
		}
		
		const TouchInfos& getTouchInfos() { return mTouchInfos; }
		int32 getTouchCount() const { return touchCount; }
		
		SoraiOSTouchInfo getTouch(int32 fingerId) {
			if(fingerId < touchCount)
                return mTouchInfos[fingerId];
            return SoraiOSTouchInfo();
		}
		
	private:
		TouchInfos mTouchInfos;
		int32 touchCount;
	};
}

#endif // _SORA_IOS_INPUT_H_
