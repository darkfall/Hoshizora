//
//  SoraEntity.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/31/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraEntity.h"
#include "../function/SoraFunction.h"
#include "../function/SoraBind.h"

namespace sora {
    
    void SoraEntity::addFsmState(const FsmStateType& state) {
        mFsm.add(state);
    }
    
    void SoraEntity::addFsmTransition(const FsmStateType& state, const FsmEventType& evt, const FsmStateType& to) {
        mFsm.defTrans(state, evt, to);
    }
    
    void SoraEntity::setFsmState(const FsmStateType& state) {
        mFsm.setState(state); 
    }
    
    void SoraEntity::processFsmEvent(const FsmEventType& evt) {
        typedef SoraFunction<void(const EntityFsmType&, const EntityFsmType::StateType&)> FuncType;
        
        FuncType fnEnter = Bind(this, &SoraEntity::onEnter);
        FuncType fnExit = Bind(this, &SoraEntity::onExit);
        
        mFsm.procEvent(evt, fnEnter, fnExit);
    }
    
    const SoraEntity::FsmStateType& SoraEntity::getCurrentFsmState() const {
        return mFsm.curr();
    }
    
    void SoraEntity::onEnter(const EntityFsmType&, const EntityFsmType::StateType&) {
        
    }
    
    void SoraEntity::onExit(const EntityFsmType&, const EntityFsmType::StateType&) {
        
    }
    
} // namespace sora