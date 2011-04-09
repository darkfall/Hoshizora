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
#include "SoraASSM_State.h"
#include "stringId.h"
#include "lMemFile.h"
#include "SoraTimerEvent.h"

#include "SoraASSM_Event.h"
#include "SoraASSM_Variable.h"

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
	class SoraASSM_SM {
	public:
		typedef SoraASSM_State<entity_type>* PASSM_STATE;
		typedef SoraASSM_SM<entity_type> ASSM_SM;
	
	private:
		PASSM_STATE pCurrentState;
		PASSM_STATE pGlobalState;
		PASSM_STATE pPreviousState;
		
		entity_type* pOwner;
				
	public:
		SoraASSM_SM(entity_type* owner, PASSM_STATE _defaultState): pOwner(owner), pCurrentState(NULL), pGlobalState(NULL), pPreviousState(NULL), defaultState(_defaultState) {
			assert( defaultState != NULL );
			assert( pOwner != NULL );
			
			// enter start event
			getEvent(EVENT_START)->linkToState(_defaultState);
			event(EVENT_START);
		}
		
		PASSM_STATE getCurrentState() const { return pCurrentState; }
		PASSM_STATE getPreviousState() const { return pPreviousState; }
		PASSM_STATE getGlobalState() const { return pGlobalState; }
		PASSM_STATE getDefaultState() const { return defaultState; }
		
		PASSM_STATE changeState(PASSM_STATE state) {
			assert(state != NULL);
			
			if(pCurrentState) {
				pCurrentState->onLeave(pOwner);
			}
			pPreviousState = pCurrentState;
			pCurrentState = state;
			if(pCurrentState) {
				pCurrentState->onEnter(pOwner);
			}
			
			return pCurrentState;
		}
		
		void returnToPreviousState() { 
			pCurrentState = pPreviousState;
			pPreviousState = NULL;
		}
		
		bool isInState(const SoraASSM_State<entity_type>& state) { return pCurrentState == state; }
		
		void runActionList(PASSM_STATE state) {
			SoraNamedObjectList actions = state->getActionList();
			size_t finishCount = 0;
			SoraNamedObjectList::Iterator itAction = actions.begin();
			while(itAction != actions.end()) {
				SoraASSM_Action* pAction = (SoraASSM_Action*)(*itAction);
				pAction->run();
				if(pAction->isFinished())
					++finishCount;
			}
			if(finishCount == actions.count()) {
				event(EVENT_FINISH);
			}
		}
		
		void update() {
			if(pCurrentState) {
				pCurrentState->onExecute(pOwner);
				
				runActionList(pCurrentState);
			}
			
			if(pGlobalState) {
				pGlobalState->onExecute(pOwner);
			}
		}
		
		void setGlobalState(PASSM_STATE state) { pGlobalState = state; }
		
		void restart() {
			pCurrentState = defaultState;
			pPreviousState = NULL;
			pGlobalState = NULL;
		}
		
		void event(stringId name) {
			SoraASSM_Event<entity_type>* pEvent = (SoraASSM_Event<entity_type>*)events.getObjectByName(name);
			if(pEvent) {
				// to do, transaction
				PASSM_STATE pState = pEvent->getLinkedState();
				if(pState != NULL)
					changeState(pState);
			}
		}
			
		void addEvent(SoraASSM_Event<entity_type>* event) { events.addObject(event); }
        template<typename varType>
		void addVariable(stringId name, SoraASSM_Variable<varType>* var) { var->setName(name); variables.addObject(var); }
		void removeEvent(stringId name) { events.removeObjectByName(name); }
		void removeVariable(stringId name) { variables.removeObjectByName(name); }
		
		SoraASSM_Event<entity_type>* getEvent(stringId name) { return (SoraASSM_Event<entity_type>*)events.getObjectByName(name); }
        template<typename varType>
		SoraASSM_Variable<varType>* getVariable(stringId name) { return variables.getObjectByName(name); }
		
		entity_type* getOwner() const { return pOwner; }
		
		// serialize the state machine and store it to memory
		lMemFile* serialize();
		void unserialize(void* data, ulong32 size);
		
	private:
		bool bStarted;
		
		PASSM_STATE defaultState;
		
		SoraNamedObjectList events;
		SoraNamedObjectList variables;
	};
	
} // namespace sora


#endif // SORA_FSM_H_