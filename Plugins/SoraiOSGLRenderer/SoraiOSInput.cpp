/*
 *  SoraiOSInput.cpp
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/21/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */


#include "SoraiOSInput.h"

#include "SoraCore.h"
#include "SoraiOSTouchInfo.h"

namespace sora {

	SoraiOSTouch::SoraiOSTouch()
		: touchCount(0) {
			// max 5 touches;
			mTouchInfos.reserve(5);
			
			//SORA->addFrameListener(new SoraiOSTouchFrameListener(this));
		}
	
	SoraiOSInput::SoraiOSInput() {
		pTouch = SoraiOSTouch::Instance();
	}
	
	void SoraiOSInput::getMousePos(float *x, float *y) {
		// no input, impossible place
		if(pTouch->getTouchCount() == 0) {
			*x = -1.f;
			*y = -1.f;
		} else {
			SoraiOSTouchInfo info = pTouch->getTouch(0);
			*x = info.x;
			*y = info.y;
		}
	}
	
	void SoraiOSInput::setMousePos(float x, float y) {
		// do nothing, cannot set
	}
	
	float SoraiOSInput::getMousePosX() {
		if(pTouch->getTouchCount() == 0) {
			return -1.f;
		} else {
			SoraiOSTouchInfo info = pTouch->getTouch(0);
			return info.x;
		}
	}
	
	float SoraiOSInput::getMousePosY() {
		if(pTouch->getTouchCount() == 0) {
			return -1.f;
		} else {
			SoraiOSTouchInfo info = pTouch->getTouch(0);
			return info.y;
		}
	}
	
	bool SoraiOSInput::keyDown(int32 key) {
		if(key == SORA_KEY_LBUTTON) {
			if(pTouch->getTouchCount() == 0) {
				return false;
			} else {
				SoraiOSTouchInfo info = pTouch->getTouch(0);
				if(info.phase == TOUCHPHASE_STATIONARY ||
				   info.phase == TOUCHPHASE_MOVED ||
				   info.phase == TOUCHPHASE_BEGAN)
					return true;
			}
		}
		return false;
	}
	
	bool SoraiOSInput::keyUp(int32 key) {
		if(key == SORA_KEY_LBUTTON) {
			if(pTouch->getTouchCount() == 0) {
				return true;
			} else {
				SoraiOSTouchInfo info = pTouch->getTouch(0);
				return info.phase == TOUCHPHASE_NULL;
			}
		}
		return false;
	}
	
	int32 SoraiOSInput::getKeyState(int32 key) {
		if(key == SORA_KEY_LBUTTON) {
			if(pTouch->getTouchCount() == 0) {
				return SORA_INPUT_KEYUP;
			} else {
				SoraiOSTouchInfo info = pTouch->getTouch(0);
				if(info.phase == TOUCHPHASE_STATIONARY ||
				   info.phase == TOUCHPHASE_MOVED ||
				   info.phase == TOUCHPHASE_BEGAN)
					return SORA_INPUT_KEYDOWN;
			}
		}
		return SORA_INPUT_KEYUP;
	}

} // namespace sora