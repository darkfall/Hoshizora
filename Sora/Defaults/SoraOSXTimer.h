/*
 *  SoraOSXTimer.h
 *  Sora
 *
 *  Created by GriffinBu on 1/22/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_OSX_TIMER_H_
#define SORA_OSX_TIMER_H_

#include "SoraTimer.h"
#include "SoraPlatform.h"

#if defined(OS_OSX) || defined(OS_IOS)

#include <mach/mach.h>
#include <mach/mach_time.h>
#if defined(OS_OSX)
#include <CoreServices/CoreServices.h>
#endif
#include <time.h>

void mach_absolute_difference(uint64_t end, uint64_t start, struct timespec *tp) {  
	uint64_t difference = end - start;  
	static mach_timebase_info_data_t info = {0,0};  
	
	if (info.denom == 0)  
		mach_timebase_info(&info);  
	
	uint64_t elapsednano = difference * (info.numer / info.denom);  
	
	tp->tv_sec = elapsednano * 1e-9;  
	tp->tv_nsec = elapsednano - (tp->tv_sec * 1e9);  
}

namespace sora {
	
	class SoraOSXTimer: public SoraTimer {
	public:
		SoraOSXTimer(): fpsInterval(SORA_FPS_INFINITE), frameCounter(0), fps(0.f), dt(0.f), fTime(0.f), fTimeScale(1.f) {
			time = mach_absolute_time();
			oldtime = time;
		}
		
		void setFPS(int32 fps) {
			fpsInterval = 1.f / fps * (1000*1000*1000);
			this->fps = (float32)fps;
		}
		
		float32 getFPS()	{ return fps; }
		float32 getDelta()	{ return fdt; }
		float32 getTime() { return fTime; }
		int32 getFrameCount() { return frameCounter; }
		void setTimeScale(float32 ts) { fTimeScale = ts; }
		float32 getTimeScale() { return fTimeScale; }
		s_int64 getCurrentSystemTime() { return mach_absolute_time(); }
		
		bool update() {
			if(fpsInterval == SORA_FPS_INFINITE) {
				if(oldtime != time) {
					oldtime = time;
					time = mach_absolute_time();
					fps = 1.f / (dtToMil(time-oldtime)/1000.f);;
				} else {
					time = mach_absolute_time();
				}
				return true;
			}
			
			time = mach_absolute_time();
			dt = time - oldtime;
			
			uint64_t nanodt = machTimeToNano(dt);
			if(nanodt < fpsInterval) {
				return false;
			}
			oldtime = time;
			fdt = dtToMil(dt)/1000.f * fTimeScale;
			/* too long */
			if(fdt >= 0.1f)
				fdt = 1.f / fps;
			
			fps = 1.f / fdt;
			frameCounter++;
			fTime += dtToMil(dt);
			return true;
		}
		
	private:
		inline uint64_t machTimeToNano(uint64_t dt) {
#if defined(OS_IOS)
			return dt;
#elif defined(OS_OSX) 
			Nanoseconds elapsedNano = (AbsoluteToNanoseconds( *(AbsoluteTime *) &dt ));
			return * (uint64_t *) &elapsedNano;
#endif
		}
		
		inline float32 dtToMil(uint64_t dt) {
			return (float32)(machTimeToNano(dt) / (1000*1000));
		}
		
		uint64_t time, oldtime;
		uint64_t dt;
		uint64_t fpsInterval;
		float32 fps;
		float32 fdt;
		int32 frameCounter;
		float32 fTime;
		float32 fTimeScale;
	};
	
} // namespace sora

#endif // OS_OSX

#endif