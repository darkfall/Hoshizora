//
//  SoraComponentHolder.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraComponentHolder.h"
#include "../SoraException.h"
#include "../SoraStringTokenlizer.h"

namespace sora {
    
    void SoraComponentHolder::addComponent(const SoraString& tag, SoraComponent* cop) {
        if(getComponent(tag) != NULL)
            THROW_SORA_EXCEPTION(ExistsException, "Component already exists!");
        mComponents.insert(std::make_pair(tag, cop));
        
        ComponentMap::iterator it = mComponents.begin();
        while(it != mComponents.end()) {
            // inform heavy weight components that a new component have been added
            if(it->second->isHeavyWeight()) {
                it->second->onComponentAdd(cop);
            }
            
            // inform the new component of all existing components
            cop->onComponentAdd(it->second);
            
            ++it;
        }
    }
    
    SoraComponent* SoraComponentHolder::getComponent(const SoraString& tag) const {
        ComponentMap::const_iterator it = mComponents.find(tag);
        if(it != mComponents.end())
            return it->second;
        return NULL;
    }
    
    SoraComponent* SoraComponentHolder::removeComponent(const SoraString& tag) {
        ComponentMap::iterator it = mComponents.find(tag);
        if(it != mComponents.end()) {
            SoraComponent* co = it->second;
            mComponents.erase(it);
            
            ComponentMap::iterator itComp = mComponents.begin();
            while(itComp != mComponents.end()) {
                // inform heavy weight components that a component have been removed
                if(itComp->second->isHeavyWeight()) {
                    itComp->second->onComponentRemove(co);
                }
                ++itComp;
            }
            
            return co;
        }
        return NULL;
    }
    
    SoraComponent* SoraComponentHolder::removeComponent(const SoraComponent* co) {
        ComponentMap::iterator it = mComponents.begin();
        ComponentMap::iterator end = mComponents.end();
        for(it; it != end; ++it) {
            if(it->second == co) {
                SoraComponent* co2 = it->second;
                mComponents.erase(it);
                
                ComponentMap::iterator itComp = mComponents.begin();
                while(itComp != mComponents.end()) {
                    // inform heavy weight components that a component have been removed
                    if(itComp->second->isHeavyWeight()) {
                        itComp->second->onComponentRemove(co2);
                    }
                    ++itComp;
                }
                return co2;
            }
        }
        return NULL;
    }
    
    void SoraComponentHolder::sendMessage(SoraMessageEvent* message) {
        ComponentMap::const_iterator it = mComponents.begin();
        while(it != mComponents.end()) {
            it->second->onMessage(message);
            if(message->isConsumed()) {
                break;
            }
            ++it;
        }
    }
    
    bool SoraComponentHolder::hasProperty(const DynRttiClassKeyType& pid) {
        SoraStringTokenlizer token(pid);
        if(token.size() == 2) {
            SoraComponent* comp = getComponent(token.first());
            if(comp)
                return comp->hasProperty(token.last());
        }
        return false;
    }
    
    bool SoraComponentHolder::addProperty(const DynRttiClassKeyType& pid, SoraPropertyInfo* prop) {
        SoraStringTokenlizer token(pid);
        if(token.size() == 2) {
            SoraComponent* comp = getComponent(token.first());
            if(comp)
                return comp->addProperty(token.last(), prop);
        }
        return false;
    }
    
    bool SoraComponentHolder::addProperty(const DynRttiClassKeyType& pid, SoraPropertyPtr prop) {
        SoraStringTokenlizer token(pid);
        if(token.size() == 2) {
            SoraComponent* comp = getComponent(token.first());
            if(comp)
                return comp->addProperty(token.last(), prop);
        }
        return false;
    }
    
    SoraPropertyPtr SoraComponentHolder::getProperty(const DynRttiClassKeyType& pid) {
        SoraStringTokenlizer token(pid);
        if(token.size() == 2) {
            SoraComponent* comp = getComponent(token.first());
            if(comp)
                return comp->getProperty(token.last());
        }
        return SoraPropertyPtr();
    }
} // namespace sora
