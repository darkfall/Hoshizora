/*
 *  SoraTimer.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/24/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_TIMER_H_
#define SORA_TIMER_H_

#include "SoraPlatform.h"

namespace sora {
	
#define SORA_FPS_INFINITE 1000
	
	class SORA_API SoraTimer {
	public:
		virtual void setFPS(int32 fps) = 0;
		virtual float32 getDelta() = 0;
		virtual float32 getFPS() = 0;
		virtual bool update() = 0;
		virtual float32 getTime() = 0;
		virtual int32 getFrameCount() = 0;
		virtual uint64 getCurrentSystemTime() = 0;
	};
}

#endif