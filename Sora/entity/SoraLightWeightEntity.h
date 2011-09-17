//
//  SoraLightWeightEntity.h
//  Sora
//
//  Created by Robert Bu on 8/29/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraLightWeightEntity_h
#define Sora_SoraLightWeightEntity_h

#include "prerequisites.h"
#include "SoraComponentHolder.h"

namespace sora {
    
#define SORA_DEF_ENTITY(class, description) \
    SORA_DEF_DYN_RTTI_CLASS(class, description) \
    public: \
        static ::sora::SoraLightWeightEntity* create() { \
            ::sora::SoraLightWeightEntity* ent = new class(); \
            return ent; \
        } \
        static void destroy(::sora::SoraLightWeightEntity* ent) { \
            if(ent) \
                delete ent; \
        } \
        static class* cast(::sora::SoraLightWeightEntity*);
    
#define SORA_IMPL_ENTITY(class) \
    SORA_IMPL_DYN_RTTI_CLASS(class) \
    class* class::cast(::sora::SoraLightWeightEntity* ent) { \
        return dynamic_cast<class*>(ent); \
    }
    
    class SoraComponent;
    
    /**
     * Entity with base component and property supoort
     * No DynRtti class(inheritance) support
     * No FSM support
     * No Listener support
     * No ScriptVM supoort
     **/
    class SORA_API SoraLightWeightEntity: public SoraObject {
    public:
		SoraLightWeightEntity();
		virtual ~SoraLightWeightEntity();
        
        bool hasComponent(const SoraString& name) const;
        void addComponent(SoraComponent* co);
        void addComponent(const std::string& name);
        SoraComponent* removeComponent(const SoraString& name);
        SoraComponent* removeComponent(const SoraComponent* co);
        SoraComponent* getComponent(const SoraString& name);
        template<typename T>
        T* getComponentT(const SoraString& name) const;
        
        template<typename T>
        void sendMessage(const MessageIdType& message, const T& data);
        void sendMessageT(SoraMessageEvent* message);
        void sendMessageTo(const SoraString& comp, SoraMessageEvent* message);
        
        virtual void render();
        virtual uint32 update(float dt);
        virtual void onUpdate(float dt);
        virtual void onMessage(SoraMessageEvent* evt);
        
        size_t getPropertySize() const;
        bool hasProperty(const PropertyId& pid);
        void addProperty(SoraPropertyBase* prop);
        SoraPropertyBase* getPropertyBase(const PropertyId& pid) const;
        SoraPropertyBase* removeProperty(const PropertyId& pid, bool release=true);
        
        // template property accessors
        template<typename T>
        void addProperty(const PropertyId& pid, const T& prop);
        template<typename T>
        void setProperty(const PropertyId& pid, const T& prop);
        template<typename T>
        T getProperty(const PropertyId& pid);
        template<typename T>
        SoraProperty<T>* getPropertyPtr(const PropertyId& pid) const;
        template<typename T>
        T getProperty(const PropertyId& pid, const T& defaultValue) const;
        
        SORA_DEF_ENTITY(SoraLightWeightEntity, "SoraEntity")
        
    protected:
        SoraScriptVMHolder mScriptVM;
        SoraComponentHolder mComponents;
        SoraDynRTTIClass& mHolder;
    };
    
    template<typename T>
    inline void SoraLightWeightEntity::sendMessage(const MessageIdType& message, const T& data) {
        SoraMessageEvent mssg(mssg, data);
        this->onMessage(&mssg);
        mComponents.sendMessage(&mssg);
    }
    
    template<typename T>
    inline void SoraLightWeightEntity::addProperty(const PropertyId& pid, const T& prop) {
        return mHolder.addProperty(pid, prop);
    }
    
    template<typename T>
    inline void SoraLightWeightEntity::setProperty(const PropertyId& pid, const T& prop) {
        SoraStringTokenlizer tokens(pid);
        mHolder.setProperty(pid, prop);
    }
    
    template<typename T>
    inline T SoraLightWeightEntity::getProperty(const PropertyId& pid) {
        SoraPropertyBase* prop = getPropertyBase(pid);
        if(prop) {
            SoraProperty<T>* tprop = PropertyCast<T>(prop);
            if(tprop)
                return tprop->get();
        }
        return SoraTypeSerializer::defaultValue<T>();
    }
    
    template<typename T>
    inline SoraProperty<T>* SoraLightWeightEntity::getPropertyPtr(const PropertyId& pid) const {
        SoraPropertyBase* prop = getPropertyBase(pid);
        if(prop) {
            SoraProperty<T>* tprop = PropertyCast<T>(prop);
            return tprop;
        }
        return 0;
    }
    
    template<typename T>
    inline T SoraLightWeightEntity::getProperty(const PropertyId& pid, const T& defaultValue) const {
        SoraPropertyBase* prop = getPropertyBase(pid);
        if(prop) {
            SoraProperty<T>* tprop = PropertyCast<T>(prop);
            if(tprop)
                return tprop->get();
        }
        return defaultValue;
    }
    
} // namespace sora



#endif
