/*
 *  SoraInputSimulator.cpp
 *  Sora
 *
 *  Created by Robert Bu on 6/6/11.
 *  Copyright 2011 GameMaster Studio(Project Hoshizora). All rights reserved.
 *
 */

#include "SoraInputSimulator.h"
#include "SoraEventManager.h"

namespace sora {
	
	SoraInputSimulator::KEY_STATE_MAP SoraInputSimulator::keyStates;

	void SoraInputSimulator::simulateKey(int32 key, int32 state) {
		KeyState s;
		s.key = key;
		s.state = state;
		keyStates[key] = s;
	}
	
	void SoraInputSimulator::clear() {
		keyStates.clear();
	}
 
	void SoraInputSimulator::simulateEvent(SoraKeyEvent* ev) {
		SORA_EVENT_MANAGER->publishInputEvent(ev);
	}
	
	int32 SoraInputSimulator::getKeyState(int32 key) {
		KEY_STATE_MAP::iterator itState = keyStates.find(key);
		if(itState != keyStates.end()) {
			return itState->second.state;
		}
		return -1;
	}

} // namespace sora