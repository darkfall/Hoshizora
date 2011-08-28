//
//  SoraComponentHolder.h
//  Sora
//
//  Created by Ruiwei Bu on 8/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraComponentHolder_h
#define Sora_SoraComponentHolder_h

#include "SoraComponent.h"
#include "../message/SoraMessageEvent.h"

#include "uncopyable.h"

namespace sora {
    
    struct SORA_API SoraComponentHolder: uncopyable {
        SoraComponentHolder() {}
        
        /**
         * Add a component, may throw a ExistsException
         **/
        void addComponent(SoraComponent* cop);
        
        SoraComponent* removeComponent(const SoraString& tag);
        SoraComponent* removeComponent(const SoraComponent*);
        
        SoraComponent* getComponent(const SoraString& tag) const;
        
        bool hasComponent(const SoraString& tag) const;
        
        template<typename T>
        void sendMessage(const MessageIdType& message, const T& data);
        void sendMessage(SoraMessageEvent* message);
        void sendMessageTo(const SoraString& to, SoraMessageEvent* message);
        
        void onUpdate(float32 dt);
        void onRender();
        
        /**
         * Basic property access
         * You can use delimeters to access component properties
         * Such as Movement.speedx
         * Because we need to tokenlize pid here, it's less efficient
         * Suggesting use property access methods directly in components
         **/
        bool hasProperty(const DynRttiClassKeyType& pid);
        void addProperty(const DynRttiClassKeyType& pid, SoraPropertyBase* prop);
        SoraPropertyBase* getProperty(const DynRttiClassKeyType& pid) const;
        
    private:
        typedef std::map<SoraString, SoraComponent*> ComponentMap;
        ComponentMap mComponents;
    };
    
    template<typename T>
    void SoraComponentHolder::sendMessage(const MessageIdType& message, const T& data) {
        ComponentMap::const_iterator it = mComponents.begin();
        
        SoraMessageEvent mssgEvt(message, data);
        while(it != mComponents.end()) {
            it->second->onMessage(&mssgEvt);
            if(mssgEvt.isConsumed()) {
                break;
            }
            ++it;
        }
    }
    
} // namespace sora


#endif
