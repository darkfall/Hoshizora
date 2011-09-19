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


} // namespace sora

#endif