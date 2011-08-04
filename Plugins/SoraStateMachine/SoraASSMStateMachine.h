/*
 *  FSM.h
 *  Sora
 *
 *  Created by GriffinBu on 1/20/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_FSM_H_
#define SORA_FSM_H_

#include "SoraEventManager.h"
#include "SoraASSMState.h"
#include "stringId.h"
#include "SoraTimerEvent.h"

#include "SoraASSMEvent.h"

#include <assert.h>

#include <map>
#include <list>

namespace sora {
	
	/*
	 ASSM (A Simple State Machine) implemention
	 */
	static const stringId EVENT_FINISH = str2id("FINISH");
	static const stringId EVENT_START = str2id("START");
	
	template<typename entity_type>
	class SoraASSMStateMachine {
	public:
		typedef SoraASSMStateMachine<entity_type>* PASSM_STATE;
		typedef SoraASSMStateMachine<entity_type> ASSM_SM;
	
	private:
		PASSM_STATE pCurrentState;
		PASSM_STATE pGlobalState;
		PASSM_STATE pPreviousState;
		
		entity_type* pOwner;
				
	public:
		SoraASSMStateMachine(entity_type* owner, PASSM_STATE _defaultState);
		
		PASSM_STATE getCurrentState() const;
		PASSM_STATE getPreviousState() const;
		PASSM_STATE getGlobalState() const;
		PASSM_STATE getDefaultState() const;
		
		PASSM_STATE changeState(PASSM_STATE state);
		
		void returnToPreviousState();
		
		bool isInState(const SoraASSMState<entity_type>& state);
		
		void runActionList(PASSM_STATE state);
		
		void update();
		
		void setGlobalState(PASSM_STATE state);
		
		void restart();
		
		void event(const SoraString& name);			
		void addEvent(SoraASSMEvent<entity_type>* event);
		void removeEvent(const SoraString& name);
		SoraASSMEvent<entity_type>* getEvent(const SoraString& name);
		
		entity_type* getOwner() const;
		
	private:
		bool bStarted;
		
		PASSM_STATE defaultState;
		
		SoraNamedObjectList events;
	};
	
    template<typename entity_type>
    SoraASSMEvent<entity_type>* SoraASSMStateMachine<entity_type>::getEvent(const SoraString& name) { 
        return (SoraASSMEvent<entity_type>*)events.getObjectByName(str2id(name));
    }
    
    template<typename entity_type>
    entity_type* SoraASSMStateMachine<entity_type>::getOwner() const { 
        return pOwner;
    }
} // namespace sora


#endif // SORA_FSM_H_