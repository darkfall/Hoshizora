/*
 *  SoraiOSTouchInfo.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/21/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#ifndef SORA_IOS_TOUCH_INFO_H_
#define SORA_IOS_TOUCH_INFO_H_

#include "SoraPlatform.h"

namespace sora {
	/*
	 Class containing touch info
	 Motivated by Unity3d Touch implemention
	 */
	
	typedef enum {
		TOUCHPHASE_NULL,
		TOUCHPHASE_BEGAN,
		TOUCHPHASE_ENDED,
		TOUCHPHASE_MOVED,
		TOUCHPHASE_STATIONARY,
		TOUCHPHASE_CANCLED
	} SoraiOSTouchPhase;
	
	struct SoraiOSTouchInfo {
		// touch phase
		SoraiOSTouchPhase phase;
		// touch position
		float x, y;
		// delta time since last change;
		float delta;
		float timestamp;
		// delta position since last change
		float dx, dy;
		// tapcount
		uint32 tapCount;
		// fingerid
		int32 fingerId;
		
		SoraiOSTouchInfo(): phase(TOUCHPHASE_NULL), x(0.f), y(0.f), dx(0.f), dy(0.f), delta(0.f), tapCount(0), fingerId(0) {}
	};
		
}


#endif // SORA_IOS_TOUCH_INFO_H_