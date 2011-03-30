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

namespace sora {
	
	inline s_int64 getCurrentSystemTime() {
#ifdef OS_WIN32
		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);
		return (s_int64)t.QuadPart;
#elif defined(OS_OSX)
		return mach_absolute_time();
#else
		timeb t;
		ftime(&t);
		return t.time*1000 + t.millitm;
#endif
	}
	
	struct SoraAutoProfile {
		SoraAutoProfile(const char* name): sName(name), startTime(getCurrentSystemTime()) { }
		~SoraAutoProfile() {
			s_int64 endTime = getCurrentSystemTime();
			s_int64 elapsedtime = endTime - startTime;
			
			SORA_PROFILER->storeProfile(sName, elapsedtime);
		}
		
		const char* sName;
		s_int64 startTime;
	};
	
	#define PROFILE(name) SoraAutoProfile(name)
} // namespace sora

#endif