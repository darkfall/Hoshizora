/*
 *  SoraAutoProfile.h
 *  Sora
 *
 *  Created by GriffinBu on 1/23/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_AUTO_PROFILE_H_
#define SORA_AUTO_PROFILE_H_

#include "SoraGlobalProfiler.h"

#ifdef OS_OSX
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#ifdef OS_PSP
#include <psprtc.h>
#endif

#include "SoraTimestamp.h"

namespace sora {
	
	inline uint64 getCurrentSystemTime() {
#ifdef OS_PSP
		uint64 t;
		sceRtcGetCurrentTick(&t);
		return t;
#else
		SoraTimestamp currTime;
        return currTime.epochMicroseconds();
#endif
	}
	
	struct SORA_API SoraAutoProfile {
		SoraAutoProfile(const char* name): sName(name), startTime(getCurrentSystemTime()) { }
		~SoraAutoProfile() {
			uint64 endTime = getCurrentSystemTime();
			uint64 elapsedtime = endTime - startTime;
			
			SORA_PROFILER->storeProfile(sName, elapsedtime);
		}
		
		const char* sName;
		uint64 startTime;
	};
	
	#define PROFILE(name) SoraAutoProfile(name)
} // namespace sora

#endif