//
//  SoraEntity.h
//  Sora
//
//  Created by Ruiwei Bu on 7/31/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraEntity_h
#define Sora_SoraEntity_h

#include "../SoraObject.h"
#include "../SoraSimpleFSM.h"
#include "../SoraPreDeclare.h"

#include "../scripting/SoraScriptVMHolder.h"

#include "SoraEntityListener.h"
#include "SoraComponentHolder.h"

namespace sora {
    
#define SORA_DEF_ENTITY(class, description) \
    SORA_DEF_DYN_RTTI_CLASS(class, description) \
    public: \
        static ::sora::SoraEntity* create() { \
            ::sora::SoraEntity* ent = new class(); \
            return ent; \
        } \
        static void destroy(::sora::SoraEntity* ent) { \
            if(ent) \
                delete ent; \
        } \
        static class* cast(::sora::SoraEntity*);
    
#define SORA_IMPL_ENTITY(class) \
        SORA_IMPL_DYN_RTTI_CLASS(class) \
        class* class::cast(::sora::SoraEntity* ent) { \
            return dynamic_cast<class*>(ent); \
        }
    
    typedef DynRttiClassKeyType PropertyId;
    
    class SORA_API SoraEntity: public SoraObject, public SoraEntityListenerManager {
    public:
		SoraEntity();
		virtual ~SoraEntity();
        
        typedef EntityFsmType::StateType FsmStateType;
        typedef EntityFsmType::EventType FsmEventType;
        
        void addFsmState(const FsmStateType&);
        void addFsmTransition(const FsmStateType&, const FsmEventType&, const FsmStateType&);
        void setFsmState(const FsmStateType&);
        void processFsmEvent(const FsmEventType&);
        const FsmStateType& getCurrentFsmState() const;
        
        bool attachVM(ScriptVMPtr, const SoraString& tag);
        ScriptVMPtr detachVM(const SoraString& tag);
        ScriptVMPtr getVM(const SoraString& tag) const;
        
        template<typename T>
        SoraConnection subscribeToVMAttached(T);
        template<typename T>
        SoraConnection subscribeToVMDetached(T);
        
        void addComponent(const SoraString& name, SoraComponent* co);
        SoraComponent* removeComponent(const SoraString& name);
        SoraComponent* removeComponent(const SoraComponent* co);
        SoraComponent* getComponent(const SoraString& name);
        
        template<typename T>
        void sendMessage(const MessageIdType& message, const T& data);
        void sendMessageT(SoraMessageEvent* message);
                
        SORA_DEF_ENTITY(SoraEntity, "Entity")
        
    public:
        struct SORA_API PropertyAccess: uncopyable {
            friend class SoraEntity;

            PropertyAccess(SoraDynRTTIClass& holder, const SoraEntity& owner):
            mHolder(holder),
            mOwner(owner) { }
            
            const SoraEntity& getOwner() const;
            
            bool has(const PropertyId& pid);
            bool add(const PropertyId& pid, SoraPropertyInfo* prop);
            bool add(const PropertyId& pid, SoraPropertyPtr prop);
            SoraPropertyInfo* remove(const PropertyId& pid, bool release=true);
            
            SoraPropertyPtr get(const PropertyId& pid) const;
            
            size_t size() const;
            
        private:
            const SoraEntity& mOwner;
            SoraDynRTTIClass& mHolder;
        };
        
        PropertyAccess& getProperty();
        
    private:
        PropertyAccess mPropertyAccess;
        
    private:
        void onEnter(const EntityFsmType&, const EntityFsmType::StateType&);
        void onExit(const EntityFsmType&, const EntityFsmType::StateType&);
        
    protected:
        EntityFsmType mFsm;
        
        SoraScriptVMHolder mScriptVM;
        SoraComponentHolder mComponents;
    };
    
    template<typename T>
    SoraConnection SoraEntity::subscribeToVMAttached(T fn) {
        return mScriptVM.subscribeToVMAttached(fn);
    }
    
    template<typename T>
    SoraConnection SoraEntity::subscribeToVMDetached(T fn) {
        return mScriptVM.subscribeToVMDetached(fn);
    }
    
    template<typename T>
    void SoraEntity::sendMessage(const MessageIdType& message, const T& data) {
        mComponents.sendMessage(message, data);
    }
    
} // namespace sora


#endif
