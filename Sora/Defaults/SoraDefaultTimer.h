/*
 *  SoraDefaultTimer.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/24/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_OSX_TIMER_H
#define SORA_OSX_TIMER_H

#include "../SoraTimer.h"

#include "SoraTimestamp.h"

namespace sora {
	class SoraDefaultTimer: public SoraTimer {
	public:
		SoraDefaultTimer(): fpsInterval(SORA_FPS_INFINITE), frameCounter(0), fps(0.f), fTime(0.f) {
			time.update();
			oldtime = time;
		}
		
		void setFPS(int32 fps) {
			fpsInterval = (uint64)((1.f / fps) * 1000 * 1000);
			this->fps = (float32)fps;
		}
		
		float32 getFPS()	{ return fps; }
		float32 getDelta()	{ return fdt; }
		int32 getFrameCount() { return frameCounter; }
		float32 getTime() {  return fTime; }
		uint64 getCurrentSystemTime() {
            SoraTimestamp currtime;
            return currtime.epochMicroseconds();
		}
		
		bool update() {
			if(fpsInterval == SORA_FPS_INFINITE) {
				if(!(time == oldtime)) {
					fps = 1.f / ((time - oldtime)/(float32)(1000*1000));
					oldtime = time;
					time.update();
				} else {
					time.update();
				}
				return true;
			}

			if(SoraTimestamp::currentTime()-oldtime.epochMicroseconds() < fpsInterval) {
				for(;;) {
                    uint64 ldt = SoraTimestamp::currentTime() - oldtime.epochMicroseconds();
					if(ldt >= fpsInterval - 1000) {
						for(;;)
							if(SoraTimestamp::currentTime() - oldtime.epochMicroseconds() >= fpsInterval)
								break;
						break;
					} else {
						msleep(1);
					}
				}
			}
            time.update();
            
            if(oldtime <= time - fpsInterval && oldtime >= time - (fpsInterval*2)) {
				fdt = fpsInterval / (float32)(1000*1000);
				oldtime += fpsInterval;
			} else {
				fdt = (time-oldtime) / (float32)(1000*1000);
				oldtime = time;
			}
			/* too long */
			if(fdt >= 1.f)
				fdt = 1.f / fps;
			
			fps = 1.f / fdt;
			
			frameCounter++;
			fTime += fdt;
			return true;
		}
		
	private:
		SoraTimestamp time, oldtime;
		uint64 fpsInterval;
        float32 fps;
		float32 fdt;
		
		float32 fTime;
		int32 frameCounter;
    };
}

#endif