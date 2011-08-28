//
//  SoraEntity.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/31/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraEntity.h"
#include "../function/SoraFunction.h"
#include "../function/SoraBind.h"
#include "SoraComponentFactory.h"

namespace sora {
    
    SORA_IMPL_ENTITY(SoraEntity)
    
    SoraEntity::SoraEntity():
    mHolder(*getRttiClass()) {
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
    
    void SoraEntity::addComponent(SoraComponent* co) {
        sora_assert(co);
        co->setOwner(this);
        mComponents.addComponent(co);
    }
    
    void SoraEntity::addComponent(const SoraString& name) {
        SoraComponent* cop = CreateComponent(name);
        if(cop)
            addComponent(cop);
    }
    
    SoraComponent* SoraEntity::removeComponent(const std::string& name) {
        SoraComponent* cop = mComponents.removeComponent(name);
        if(cop)
            cop->setOwner(0);
    }
    
    SoraComponent* SoraEntity::removeComponent(const SoraComponent* co) {
        SoraComponent* cop =  mComponents.removeComponent(co);
        if(cop)
            cop->setOwner(0);
    }
    
    SoraComponent* SoraEntity::getComponent(const std::string& name) {
        return mComponents.getComponent(name);
    }
    
    bool SoraEntity::hasComponent(const SoraString& name) const {
        return mComponents.hasComponent(name);
    }
    
    bool SoraEntity::hasProperty(const PropertyId& pid) {
        if(!mHolder.hasProperty(pid)) {
            return mHolder.hasProperty(pid);
        }
        return true;
    }
    
    void SoraEntity::addProperty(SoraPropertyBase* prop) {
        return mHolder.addProperty(prop);
    }
    
    SoraPropertyBase* SoraEntity::getPropertyBase(const PropertyId& pid) const {
        SoraPropertyBase* prop =  mHolder.getPropertyBase(pid);
        if(prop == 0) {
            return mComponents.getProperty(pid);
        }
        return prop;
    }
    
    size_t SoraEntity::getPropertySize() const {
        return mHolder.getPropertySize();
    }
    
    SoraPropertyBase* SoraEntity::removeProperty(const PropertyId& pid, bool release) {
        return mHolder.removeProperty(pid, release);
    }
    
    void SoraEntity::sendMessageT(SoraMessageEvent* message) {
        mComponents.sendMessage(message);
    }
    
    void SoraEntity::sendMessageTo(const SoraString& comp, SoraMessageEvent* message) {
        mComponents.sendMessageTo(comp, message);
    }

    void SoraEntity::onUpdate(float dt) {
        update(dt);
    }
    
    uint32 SoraEntity::update(float dt) {
        mComponents.onUpdate(dt);
    }
    
    void SoraEntity::render() {
        mComponents.onRender();
    }
} // namespace sora