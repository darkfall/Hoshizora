#ifndef TIMER_H_
#define TIMER_H_

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraEvent.h"

namespace sora {
	
	class SoraTimerEvent: public SoraEvent {
	public:
		float32 getTime() const { return time; }
		void setTime(float32 _time) { time = _time; }
	private:
		float32 time;
	};

	class timer {
	public:
		void start();
		void stop();

		void pause();
		void resume();

		float32 getTime() const ;
		float32 getPausedTime() const ;
		float32 getStartedTime() const ;

		virtual uint32 update(float32 delta);
		
		uint8 isStarted();
		uint8 isPaused();

	private:
		float32 startTime;
		float32 pauseTime;
		
		uint8 started;
		uint8 paused;
	};

} // namespace sora

#endif