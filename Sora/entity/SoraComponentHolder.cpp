//
//  SoraComponentHolder.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/21/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraComponentHolder.h"
#include "../SoraException.h"

namespace sora {
    
    void SoraComponentHolder::addComponent(const std::string& tag, SoraComponent* cop) {
        if(getComponent(tag) != NULL)
            THROW_SORA_EXCEPTION(ExistsException, "Component already exists!");
        mComponents.insert(std::make_pair(tag, cop));
    }
    
    SoraComponent* SoraComponentHolder::getComponent(const std::string& tag) {
        ComponentMap::iterator it = mComponents.find(tag);
        if(it != mComponents.end())
            return it->second;
        return NULL;
    }
    
    SoraComponent* SoraComponentHolder::removeComponent(const std::string& tag) {
        ComponentMap::iterator it = mComponents.find(tag);
        if(it != mComponents.end()) {
            SoraComponent* co = it->second;
            mComponents.erase(it);
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
                return co2;
            }
        }
        return NULL;
    }
    
    
} // namespace sora
