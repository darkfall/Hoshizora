#ifndef TIMER_H_
#define TIMER_H_

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraEvent.h"

namespace sora {
	
	class SORA_API SoraTimerEvent: public SoraEvent {
	public:
		float getTime() const { return time; }
		void setTime(float _time) { time = _time; }
		
		float getTotalTime() const { return totalTime; }
		void setTotalTime(float _totalTime) { totalTime = _totalTime; }
	
#ifndef SORA_USE_RTTI
		SORA_EVENT_IDENTIFIER(18446744070739255393ULL);
#endif
		
	protected:
		float time;
		float totalTime;
	};

	class timer {
	public:
		void start();
		void stop();

		void pause();
		void resume();

		float getTime() const ;
		float getPausedTime() const ;
		float getStartedTime() const ;

		virtual uint32 update(float delta);
		
		uint8 isStarted();
		uint8 isPaused();

	private:
		float startTime;
		float pauseTime;
		
		uint8 started;
		uint8 paused;
	};

} // namespace sora

#endif