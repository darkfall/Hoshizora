//
//  SoraASSMStateMachine.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/7/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraASSMStateMachine.h"

namespace sora {
    
    template<typename entity_type>
    SoraASSMStateMachine<entity_type>::SoraASSMStateMachine(entity_type* owner, SoraASSMStateMachine::PASSM_STATE _defaultState): 
    pOwner(owner), 
    pCurrentState(NULL), 
    pGlobalState(NULL), 
    pPreviousState(NULL), 
    defaultState(_defaultState) {
        assert( defaultState != NULL );
        assert( pOwner != NULL );
        
        // enter start event
        defaultState->setOwner(owner);
        getEvent("START")->linkToState(_defaultState);
        event("START");
    }
    
    template<typename entity_type>
    typename SoraASSMStateMachine<entity_type>::PASSM_STATE SoraASSMStateMachine<entity_type>::getCurrentState() const { 
        return pCurrentState; 
    }
    
    template<typename entity_type>
    typename SoraASSMStateMachine<entity_type>::PASSM_STATE SoraASSMStateMachine<entity_type>::getPreviousState() const { 
        return pPreviousState; 
    }
    
    template<typename entity_type>
    typename SoraASSMStateMachine<entity_type>::PASSM_STATE SoraASSMStateMachine<entity_type>::getGlobalState() const { 
        return pGlobalState;
    }
    
    template<typename entity_type>
    typename SoraASSMStateMachine<entity_type>::PASSM_STATE SoraASSMStateMachine<entity_type>::getDefaultState() const { 
        return defaultState; 
    }
    
    template<typename entity_type>
   typename SoraASSMStateMachine<entity_type>::PASSM_STATE SoraASSMStateMachine<entity_type>::changeState(SoraASSMStateMachine::PASSM_STATE state) {
        assert(state != NULL);
        
        if(pCurrentState) {
            pCurrentState->onLeave();
        }
        pPreviousState = pCurrentState;
        pCurrentState = state;
        if(pCurrentState) {
            pCurrentState->setOwner(pOwner);
            pCurrentState->onEnter();
        }
        
        return pCurrentState;
    }
    
    template<typename entity_type>
    void SoraASSMStateMachine<entity_type>::returnToPreviousState() { 
        pCurrentState = pPreviousState;
        pPreviousState = NULL;
    }
    
    template<typename entity_type>
    bool SoraASSMStateMachine<entity_type>::isInState(const SoraASSMState<entity_type>& state) { 
        return pCurrentState == state;
    }
    
    template<typename entity_type>
    void SoraASSMStateMachine<entity_type>::runActionList(SoraASSMStateMachine::PASSM_STATE state) {
        if(state->runActionList()) {
            event("FINISH");
        }
    }
    
    template<typename entity_type>
    void SoraASSMStateMachine<entity_type>::update() {
        if(pCurrentState) {
            pCurrentState->onExecute();
            
            runActionList(pCurrentState);
        }
        
        if(pGlobalState) {
            pGlobalState->onExecute();
        }
    }
    
    template<typename entity_type>
    void SoraASSMStateMachine<entity_type>::setGlobalState(SoraASSMStateMachine::PASSM_STATE state) { 
        pGlobalState = state;
    }
    
    template<typename entity_type>
    void SoraASSMStateMachine<entity_type>::restart() {
        pCurrentState = defaultState;
        pPreviousState = NULL;
        pGlobalState = NULL;
    }
    
    template<typename entity_type>
    void SoraASSMStateMachine<entity_type>::event(const SoraString& name) {
        stringId nameid = str2id(name);
        SoraASSMEvent<entity_type>* pEvent = (SoraASSMEvent<entity_type>*)events.getObjectByName(nameid);
        if(pEvent) {
            // to do, transaction
            PASSM_STATE pState = pEvent->getLinkedState();
            if(pState != NULL)
                changeState(pState);
        }
    }
    
    template<typename entity_type>
    void SoraASSMStateMachine<entity_type>::addEvent(SoraASSMEvent<entity_type>* event) { 
        events.addObject(event);
    }
    
    template<typename entity_type>
    void SoraASSMStateMachine<entity_type>::removeEvent(const SoraString& name) { 
        events.removeObjectByName(str2id(name));
    }
   
    
} // namespace sora
