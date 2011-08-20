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
    
    SoraEntity::SoraEntity() {
        
    }
    
    SoraEntity::~SoraEntity() {
        
    }
    
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
        listeners_fsmEventHandled(*this, mFsm, evt);
    }
    
    const SoraEntity::FsmStateType& SoraEntity::getCurrentFsmState() const {
        return mFsm.curr();
    }
    
    void SoraEntity::onEnter(const EntityFsmType& fsm, const EntityFsmType::StateType& state) {
        listeners_fsmEnterState(*this, fsm, state);
    }
    
    void SoraEntity::onExit(const EntityFsmType& fsm, const EntityFsmType::StateType& state) {
        listeners_fsmExitState(*this, fsm, state);
    }
    
    bool SoraEntity::attachVM(ScriptVMPtr ptr, const SoraString& tag) {
        return mScriptVM.attachVM(ptr, tag);
    }
    
    ScriptVMPtr SoraEntity::detachVM(const SoraString& tag) {
        return mScriptVM.detachVM(tag);
    }
   
    ScriptVMPtr SoraEntity::getVM(const SoraString& tag) const {
        return mScriptVM.getVM(tag);
    }
} // namespace sora