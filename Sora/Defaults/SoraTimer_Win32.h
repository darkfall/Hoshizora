#ifndef SORA_TIMER_WIN32
#define SORA_TIMER_WIN32

#include "../SoraTimer.h"
#include "../SoraPlatform.h"

namespace sora {
	class SoraWin32Timer;
} // namespace sora

#ifdef OS_WIN32

extern "C" {
#include "orz.h"
}

namespace sora {

	class SoraWin32Timer: public SoraTimer {
		friend class SoraCore;

	public:
		SoraWin32Timer() { desiredFPS = SORA_FPS_INFINITE; nFrameCounter = 0; }
		void setFPS(int32 fps) { desiredFPS = (float)fps; }
		float getDelta() { return fDeltaTime; }
		float getFPS() { return fFPS; }
		float getTime() { return fTime; }
		int32 getFrameCount() { return nFrameCounter; }
		uint64 getCurrentSystemTime() { 
			LONGLONG  TimePrecision;
			LARGE_INTEGER Frequency;
		    QueryPerformanceFrequency(&Frequency);
			TimePrecision =  Frequency.QuadPart;
			return GetCurrentSystemTime(); 
		}

		
		bool update() { 
			int bPriorityRaised = 0;
		    LONGLONG TimeInterval, TimePrecision, NowTime;
			static LONGLONG lastTime = GetCurrentSystemTime();
			
			LARGE_INTEGER Frequency;
		    QueryPerformanceFrequency(&Frequency);
			TimePrecision =  Frequency.QuadPart;
			if (desiredFPS > 0)
			{
				TimeInterval = TimePrecision / (LONGLONG)desiredFPS;
			}
			else
			{
				TimeInterval = 0;
			}
   
			if (GetCurrentSystemTime() - lastTime >= TimeInterval)
		    {
				if (GetThreadPriority(GetCurrentThread()) > THREAD_PRIORITY_NORMAL)
					SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
		    }
		    else
		    {
		        for (;;)
		        {
		            if (GetCurrentSystemTime() - lastTime >= (TimeInterval - (TimePrecision / (1000 * 2))))
		            {
		                if (!bPriorityRaised && GetThreadPriority(GetCurrentThread()) > THREAD_PRIORITY_NORMAL)
		                    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
						for (;;)
						{
						    if (GetCurrentSystemTime() - lastTime >= TimeInterval)
					            break;
				        }
			            break;
					}
		            else
		            {
		                if (!bPriorityRaised)
		                {
		                    if (GetThreadPriority(GetCurrentThread()) < THREAD_PRIORITY_HIGHEST)
								SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
		                    bPriorityRaised = 1;
		                }
		                Sleep(1);
		            }
		        }
		    }

			NowTime = GetCurrentSystemTime();


		    if (lastTime <= (NowTime - TimeInterval) && lastTime >= (NowTime - (TimeInterval * 2)))
			{
				fDeltaTime = (float)((double)(TimeInterval) / (double)(TimePrecision));
		        lastTime += TimeInterval;
			}
		    else
			{
				fDeltaTime = (float)((double)(NowTime - lastTime) / (double)(TimePrecision));
		        lastTime = NowTime;
			}
			long long t = (NowTime-lastTime);
			if(fDeltaTime >= 1.f) 
				fDeltaTime = 1.f / desiredFPS;

			nFrameCounter++;
			fTime += fDeltaTime;
			fFPS = 1.0f / fDeltaTime;
			return true;
		}

	private:
		float desiredFPS;
		int32 oldTimeStamp;
		float fDeltaTime, fTime, fFPS;
		int32 nFrameCounter;
	};

} // namespace sora

#endif // OS_WIN32

#endif // SORA_TIMER_WIN32_H_