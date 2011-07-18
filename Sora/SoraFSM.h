//
//  SoraFSM.h
//  Sora
//
//  Created by Ruiwei Bu on 7/17/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraFSM_h
#define Sora_SoraFSM_h

/**
 *  A Simple FSM(Finite State Machine) Implemention
 *  No advanced FSM features such as HSM supported here
 **/

#include "SoraPlatform.h"
#include "stringId.h"
#include <map>

namespace sora {
    
    class SoraFSMManager;
    
    class SORA_API SoraFSMState {
    public:
        friend class SoraFSMManager;
        
        SoraFSMState(): mInitiated(false), mManager(NULL) {}
        
        virtual void onInitiate() {}
        virtual void onDestroy() {
            delete this;
        }
        
        virtual void onUpdate(float32 dt) {}
        virtual void onRender() {}
        
        virtual void onEnter() {}
        virtual void onLeave() {}
                
    private:
        bool mInitiated;
        SoraFSMManager* mManager;
    };
    
    class SORA_API SoraFSMManager {
    public:
        SoraFSMManager();
        ~SoraFSMManager();
        
        SoraFSMManager& defState(SoraFSMState* state, const SoraString& name);
        void delState(const SoraString& name);
        
        void switchToState(const SoraString& name);
        void switchToState(SoraFSMState* state);

        void setGlobalState(SoraFSMState* state);
        
        SoraFSMState* getCurrentState() const;
        SoraFSMState* getPreviousState() const;
        SoraFSMState* getGlobalState() const;
        
        void onUpdate(float32 dt);
        void onRender();
        
        bool returnToPreviousState();
        
        static SoraFSMManager& defaultFSMManager();
        
        SoraFSMState* operator[](const SoraString& name);
        
    private:
        typedef std::map<stringId, SoraFSMState*> FSMStateMap;
        FSMStateMap mStates;
        
        SoraFSMState* mGlobalState;
        SoraFSMState* mCurrentState;
        SoraFSMState* mPreviousState;
    };
} // namespace sora



#endif
