//
//  SoraLightWeightEntity.cpp
//  Sora
//
//  Created by Robert Bu on 8/29/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraLightWeightEntity.h"
#include "SoraComponent.h"
#include "SoraComponentHolder.h"

namespace sora {
    
    SORA_IMPL_ENTITY(SoraLightWeightEntity)
    
    SoraLightWeightEntity::SoraLightWeightEntity():
    mHolder() {
        // entity can handle SoraMessageEvent and send message to components
    }
    
    SoraLightWeightEntity::~SoraLightWeightEntity() {
        
    }
    
    void SoraLightWeightEntity::addComponent(SoraComponent* co) {
        sora_assert(co);
        co->setOwner(this);
        mComponents.addComponent(co);
    }
    
    void SoraLightWeightEntity::addComponent(const SoraString& name) {
        SoraComponent* cop = CreateComponent(name, this);
        if(cop)
            addComponent(cop);
    }
    
    SoraComponent* SoraLightWeightEntity::removeComponent(const std::string& name) {
        SoraComponent* cop = mComponents.removeComponent(name);
        if(cop)
            cop->setOwner(0);
        return cop;
    }
    
    SoraComponent* SoraLightWeightEntity::removeComponent(const SoraComponent* co) {
        SoraComponent* cop =  mComponents.removeComponent(co);
        if(cop)
            cop->setOwner(0);
        return cop;
    }
    
    SoraComponent* SoraLightWeightEntity::getComponent(const std::string& name) {
        return mComponents.getComponent(name);
    }
    
    bool SoraLightWeightEntity::hasComponent(const SoraString& name) const {
        return mComponents.hasComponent(name);
    }
    
    bool SoraLightWeightEntity::hasProperty(const PropertyId& pid) {
        if(!mHolder.hasProperty(pid)) {
            return mHolder.hasProperty(pid);
        }
        return true;
    }
    
    void SoraLightWeightEntity::addProperty(SoraPropertyBase* prop) {
        mHolder.addProperty(prop);
    }
    
    void SoraLightWeightEntity::onMessage(SoraMessageEvent* evt) {
       // mComponents.sendMessage(evt);
        sendMessage(evt);
    }
    
    SoraPropertyBase* SoraLightWeightEntity::getPropertyBase(const PropertyId& pid) const {
        SoraPropertyBase* prop =  mHolder.getPropertyBase(pid);
        return prop;
    }
    
    size_t SoraLightWeightEntity::getPropertySize() const {
        return mHolder.size();
    }
    
    SoraPropertyBase* SoraLightWeightEntity::removeProperty(const PropertyId& pid, bool release) {
        return mHolder.removeProperty(pid, release);
    }
    
    void SoraLightWeightEntity::sendMessage(SoraMessageEvent* message) {
        if(message->getReceiver() == this)
            this->onMessage(message);
        else 
            mComponents.sendMessage(message);
    }
    
    void SoraLightWeightEntity::sendMessageTo(const SoraString& comp, SoraMessageEvent* message) {
        mComponents.sendMessageTo(comp, message);
    }
    
    void SoraLightWeightEntity::onUpdate(float dt) {
        update(dt);
    }
    
    int32 SoraLightWeightEntity::update(float dt) {
        mComponents.onUpdate(dt);
        return 0;
    }
    
    void SoraLightWeightEntity::render() {
        mComponents.onRender();
    }
    
    void SoraLightWeightEntity::copyPropertyMap(SoraLightWeightEntity* entity) {
        sora_assert(entity);
        
        entity->mHolder = mHolder;
    }

    
} // namespace sora