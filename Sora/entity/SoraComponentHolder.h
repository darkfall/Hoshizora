//
//  SoraComponentHolder.h
//  Sora
//
//  Created by Robert Bu on 8/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraComponentHolder_h
#define Sora_SoraComponentHolder_h

#include "SoraComponent.h"
#include "message/SoraMessageEvent.h"

#include "SoraUncopyable.h"

namespace sora {
    
    struct SORA_API SoraComponentHolder: SoraUncopyable {
        SoraComponentHolder() {}
        
        /**
         * Add a component, may throw a ExistsException
         **/
        void addComponent(SoraComponent* cop);
        
        SoraComponent* removeComponent(const SoraString& tag);
        SoraComponent* removeComponent(const SoraComponent*);
        
        SoraComponent* getComponent(const SoraString& tag) const;
        
        // get component and cast
        // may throw exceptions
        template<typename T>
        T* getComponentT(const SoraString& tag) const;
        
        bool hasComponent(const SoraString& tag) const;
        
        template<typename T>
        void sendMessage(const MessageIdType& message, const T& data);
        void sendMessage(SoraMessageEvent* message);
        void sendMessageTo(const SoraString& to, SoraMessageEvent* message);
        
        void onUpdate(float dt);
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
    inline void SoraComponentHolder::sendMessage(const MessageIdType& message, const T& data) {
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
    
    template<typename T>
    inline T* SoraComponentHolder::getComponentT(const SoraString& tag) const {
        ComponentMap::const_iterator it = mComponents.find(tag);
        if(it != mComponents.end()) {
            if(it->second->getName() == T::GetName()) {
                return static_cast<T>(it->second);
            } else 
                THROW_SORA_EXCEPTION(BadCastException, "Different component type");
        } else
            THROW_SORA_EXCEPTION(NotFoundException, "Component not found");
    }
    
} // namespace sora


#endif
