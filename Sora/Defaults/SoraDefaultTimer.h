/*
 *  SoraDefaultTimer.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/24/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORA_OSX_TIMER_H
#define SORA_OSX_TIMER_H

#include "../SoraTimer.h"

#include <sys/timeb.h>

namespace sora {
	class SoraDefaultTimer: public SoraTimer {
	public:
		SoraDefaultTimer(): fpsInterval(SORA_FPS_INFINITE), frameCounter(0), fps(0.f), dt(0.f), fTime(0.f), fTimeScale(1.f) {
			ftime(&time);
			oldtime = time;
		}
		
		void setFPS(int32 fps) {
			fpsInterval = 1.f / fps * 1000;
			this->fps = (float32)fps;
		}
		
		float32 getFPS()	{ return fps; }
		float32 getDelta()	{ return fdt; }
		int32 getFrameCount() { return frameCounter; }
		float32 getTime() {  return fTime; }
		void setTimeScale(float32 ts) { fTimeScale = ts; }
		float32 getTimeScale() { return fTimeScale; }
		uint64 getCurrentSystemTime() {
			timeb t;
			ftime(&t);
			return (uint64)(t.time*1000 + t.millitm);
		}
		
		bool update() {
			if(fpsInterval == SORA_FPS_INFINITE) {
				if(!timebEqual(time, oldtime)) {
					fps = 1.f / (getDelta(time, oldtime)/1000.f);
					oldtime = time;
					ftime(&time);
				} else {
					ftime(&time);
				}
				return true;
			}
			
			ftime(&time);
			dt = getDelta(time, oldtime);

			if(dt < fpsInterval) {
				for(;;) {
					// 1 millisecond precision
					if(getDelta(time, oldtime) < fpsInterval - 1000) {
						for(;;)
							if(getDelta(time, oldtime) >= fpsInterval)
								break;
						break;
					} else {
						msleep(1);
					}
				}
				return false;
			}
			fdt = dt/1000.f * fTimeScale;
			/* too long */
			if(fdt >= 1.f)
				fdt = fpsInterval / 1000.f;
			oldtime = time;
			
			fps = 1.f / fdt;
			frameCounter++;
			fTime += dt / 1000.f;
			return true;
		}
		
	private:
		inline float32 getDelta(timeb& t1, timeb& t2) {
			return (float32)((t1.millitm - t2.millitm) + 1000*(t1.time-t2.time));
		}
		inline bool timebEqual(timeb& t1, timeb& t2) {
			return (t1.millitm == t2.millitm && t1.time == t2.time);
		}
		
		timeb time, oldtime;
		float32 dt, fpsInterval, fps;
		float32 fdt;
		
		float32 fTime;
		int32 frameCounter;
		
		float32 fTimeScale;
	};
}

#endif