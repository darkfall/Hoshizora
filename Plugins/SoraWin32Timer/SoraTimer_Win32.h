#ifndef SORA_TIMER_WIN32
#define SORA_TIMER_WIN32

#include "SoraTimer.h"
extern "C" {
#include "orz.h"
}

namespace sora {

	class SoraWin32Timer: public SoraTimer {
	public:
		SoraWin32Timer() { desiredFPS = 60; nFrameCounter = 0; }
		void setFPS(int32 fps) { desiredFPS = fps; }
		float32 getDelta() { return fDeltaTime; }
		float32 getFPS() { return fFPS; }
		float32 getTime() { return GetCurrentSystemTime(); }
		bool update() { 
			int bPriorityRaised = 0;
		    LONGLONG TimeInterval, TimePrecision, NowTime;
			static LONGLONG lastTime = GetCurrentSystemTime();

			LARGE_INTEGER Frequency;
		    QueryPerformanceFrequency(&Frequency);
			TimePrecision =  Frequency.QuadPart;
			if (desiredFPS > 0)
			{
				TimeInterval = TimePrecision / desiredFPS;
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

			nFrameCounter++;
			fTime += fDeltaTime;
			fFPS = 1.0f / fDeltaTime;
			return true;
		}

	private:
		int32 desiredFPS, oldTimeStamp;
		float32 fDeltaTime, fTime, fFPS;
		int32 nFrameCounter;
	};

} // namespace sora

#endif