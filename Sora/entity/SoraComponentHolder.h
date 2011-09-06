//
//  SoraComponentHolder.h
//  Sora
//
//  Created by Robert Bu on 8/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraComponentHolder_h
#define Sora_SoraComponentHolder_h

#include "SoraPlatform.h"
#include "SoraUncopyable.h"
#include "message/SoraMessageEvent.h"
#include "property/SoraProperty.h"

namespace sora {
    
    class SoraComponent;
    
    struct SORA_API SoraComponentHolder: SoraUncopyable {
        SoraComponentHolder() {}
        
        /**
         * Add a component, may throw a ExistsException
         **/
        void addComponent(SoraComponent* cop);
        
        SoraComponent* removeComponent(const util::String& tag);
        SoraComponent* removeComponent(const SoraComponent*);
        
        SoraComponent* getComponent(const util::String& tag) const;
     
        bool hasComponent(const util::String& tag) const;
        
        void sendMessage(SoraMessageEvent* message);
        void sendMessageTo(const util::String& to, SoraMessageEvent* message);
        
        void onUpdate(float dt);
        void onRender();
        
        /**
         * Basic property access
         * You can use delimeters to access component properties
         * Such as Movement.speedx
         * Because we need to tokenlize pid here, it's less efficient
         * Suggesting use property access methods directly in components
         **/
        bool hasProperty(const std::string& pid);
        void addProperty(const std::string& pid, SoraPropertyBase* prop);
        SoraPropertyBase* getProperty(const std::string& pid) const;
        
    private:
        typedef hash_map<util::String, SoraComponent*> ComponentMap;
        ComponentMap mComponents;
    };
    
} // namespace sora


#endif
