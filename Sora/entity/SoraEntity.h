//
//  SoraEntity.h
//  Sora
//
//  Created by Robert Bu on 7/31/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraEntity_h
#define Sora_SoraEntity_h

#include "SoraLightWeightEntity.h"
#include "SoraEntityListener.h"

namespace sora {
    
    typedef DynRttiClassKeyType PropertyId;
    
    class SORA_API SoraEntity: public SoraLightWeightEntity, public SoraEntityListenerManager {
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
                
    private:
        void onEnter(const EntityFsmType&, const EntityFsmType::StateType&);
        void onExit(const EntityFsmType&, const EntityFsmType::StateType&);
        
    protected:
        EntityFsmType mFsm;
        
        SoraScriptVMHolder mScriptVM;
    };
    
    template<typename T>
    inline SoraConnection SoraEntity::subscribeToVMAttached(T fn) {
        return mScriptVM.subscribeToVMAttached(fn);
    }
    
    template<typename T>
    inline SoraConnection SoraEntity::subscribeToVMDetached(T fn) {
        return mScriptVM.subscribeToVMDetached(fn);
    }
} // namespace sora


#endif
