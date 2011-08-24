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
    
    SORA_IMPL_ENTITY(SoraEntity)
    
    SoraEntity::SoraEntity():
    mPropertyAccess(*getRttiClass(),
                    *this) {
        // entity can handle SoraMessageEvent and send message to components
        registerEventFunc(this, &SoraEntity::sendMessageT);
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
    
    void SoraEntity::addComponent(const std::string& name, SoraComponent* co) {
        mComponents.addComponent(name, co);
    }
    
    SoraComponent* SoraEntity::removeComponent(const std::string& name) {
        return mComponents.removeComponent(name);
    }
    
    SoraComponent* SoraEntity::removeComponent(const SoraComponent* co) {
        return mComponents.removeComponent(co);
    }
    
    SoraComponent* SoraEntity::getComponent(const std::string& name) {
        return mComponents.getComponent(name);
    }
    
    bool SoraEntity::PropertyAccess::has(const PropertyId& pid) {
        if(!mHolder.hasProperty(pid)) {
            return mHolder.hasProperty(pid);
        }
        return true;
    }
    
    bool SoraEntity::PropertyAccess::add(const PropertyId& pid, SoraPropertyInfo* prop) {
        return mHolder.addProperty(pid, prop);
    }
    
    bool SoraEntity::PropertyAccess::add(const PropertyId& pid, SoraPropertyPtr prop) {
        return mHolder.addProperty(pid, prop);
    }
    
    SoraPropertyPtr SoraEntity::PropertyAccess::get(const PropertyId& pid) const {
        SoraPropertyPtr prop =  mHolder.getProperty(pid);
        if(!prop.valid()) {
            return mHolder.getProperty(pid);
        }
        return prop;
    }
    
    size_t SoraEntity::PropertyAccess::size() const {
        return mHolder.getPropertySize();
    }
    
    SoraEntity::PropertyAccess& SoraEntity::getProperty() {
        return mPropertyAccess;
    }
    
    SoraPropertyInfo* SoraEntity::PropertyAccess::remove(const PropertyId& pid, bool release) {
        SoraPropertyInfo* info = mHolder.removeProperty(pid);
        if(release) {
            delete info;
            info = 0;
        }
        return info;
    }
    
    void SoraEntity::sendMessageT(SoraMessageEvent* message) {
        mComponents.sendMessage(message);
    }
} // namespace sora