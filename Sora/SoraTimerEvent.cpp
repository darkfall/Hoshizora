#include "SoraTimerEvent.h"

namespace sora {

	void timer::start() {
		started = 1;
		startTime = pauseTime = 0.f;
	}

	void timer::stop() {
		started = 0;
	}

	void timer::pause() {
		paused = 1;
		pauseTime = 0.f;
	}

	void timer::resume() {
		paused = 0;
	}

	float32 timer::getPausedTime() const {
		return pauseTime;
	}

	float32 timer::getStartedTime() const {
		return startTime;
	}

	float32 timer::getTime() const {
		if(started == 1) {
			if(paused == 1) {
				return pauseTime;
			}
			return startTime;
		}
		return 0.f;
	}
	
	uint8 timer::isStarted() {
		return started;
	}
	
	uint8 timer::isPaused() {
		return paused;
	}

	uint32 timer::update(float32 delta) {
		if(started == 1) {
			if(paused == 1) {
				pauseTime += delta;
			} else startTime += delta;
		}
		return 0;
	}

} // namespace sora