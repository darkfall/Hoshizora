/*
 *  rftdEvents.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_EVENTS_H_
#define RFTD_EVENTS_H_

#include "SoraEvent.h"

namespace rftd {
	
	class EnemyReachEndEvent: public sora::SoraEvent {
	public:
		EnemyReachEndEvent(int32 atk): attack(atk) {}
		
		int32 getAttack() const {
			return attack;
		}
		
		SORA_EVENT_IDENTIFIER(sora::str2id("EnemyReachEndEvent"));
	private:
		int32 attack;
	};	
	
} // namespace rftd


#endif // RFTD_EVENTS_H_