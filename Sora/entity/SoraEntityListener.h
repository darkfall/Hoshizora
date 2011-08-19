//
//  SoraObjectListener.h
//  Sora
//
//  Created by Ruiwei Bu on 8/18/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraObjectListener_h
#define Sora_SoraObjectListener_h

#include "../SoraObject.h"
#include "../SoraListenerManager.h"

namespace sora {
    
    class SoraEntity;
    
    typedef SoraSimpleFSM<SoraString, SoraString> EntityFsmType;

    class SoraEntityListener {
    public:
        virtual ~SoraEntityListener() { }
         
        void onFsmEnterState(SoraEntity& ent, const EntityFsmType& fsm, const EntityFsmType::StateType& state);
        void onFsmExitState(SoraEntity& ent, const EntityFsmType& fsm, const EntityFsmType::StateType& state);
        
        void onInit(SoraEntity& ent);
        void onUpdate(SoraEntity& ent);
    };
    
    class SoraEntityListenerManager: public SoraListenerManager<SoraEntityListener> {
    protected:
        void listeners_init(SoraEntity& entity) {
            
        }
        
        void listeners_update(SoraEntity& entity) {
            
        }
        
        void listeners_fsmEventHandled(SoraEntity& entity) {
            
        }
        
        void listeners_fsmEnterState(SoraEntity& entity) {
            
        }
        
        void listeners_fsmExitState(SoraEntity& entity) {
            
        }
        
    };
    
} // namespace sora


#endif
