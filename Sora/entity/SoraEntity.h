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

namespace sora {
    
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
        
    private:
        void onEnter(const EntityFsmType&, const EntityFsmType::StateType&);
        void onExit(const EntityFsmType&, const EntityFsmType::StateType&);
        
    protected:
        NamedValueList mProperties;
        
        EntityFsmType mFsm;
        
        SoraScriptVMHolder mScriptVM;
    };
    
    template<typename T>
    SoraConnection SoraEntity::subscribeToVMAttached(T fn) {
        return mScriptVM.subscribeToVMAttached(fn);
    }
    
    template<typename T>
    SoraConnection SoraEntity::subscribeToVMDetached(T fn) {
        return mScriptVM.subscribeToVMDetached(fn);
    }
} // namespace sora


#endif
