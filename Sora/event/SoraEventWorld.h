//
//  SoraEventWorld.h
//  Sora
//
//  Created by Ruiwei Bu on 8/5/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraEventWorld_h
#define Sora_SoraEventWorld_h

#include "SoraPlatform.h"
#include "SoraScopedPtr.h"
#include "SoraEvent.h"
#include "uncopyable.h"

namespace sora {
    
    class SoraEventWorldImpl;
    class SoraEventWorld;
    
    /**
     * Notice: Event go through SoraEventWorld must be constructed by SoraEventFactory
     **/
    
    class SORA_API SoraEventWorld: uncopyable {
    public:
        typedef SoraEventHandler* EventHandlerPtr;
        
        struct Parameter {
            Parameter():
            mMultitrhead(false),
            mFixedStep(false) {
                
            }
            
            Parameter(bool multithread, bool fixedstep):
            mMultitrhead(multithread),
            mFixedStep(fixedstep) {
                
            }
            
            inline Parameter& multiThread(bool multiThread=true) {
                mMultitrhead = multiThread;
                return *this;
            }
            
            inline Parameter& fixedStep(bool fixedStep=true) {
                mFixedStep = fixedStep;
                return *this;
            }
            
            bool isMultiThread() const {
                return mMultitrhead;
            }
            
            bool isFixedStep() const {
                return mFixedStep;
            }
            
        private:
            bool mMultitrhead;
            bool mFixedStep;
        };
        
    public:
        SoraEventWorld(const Parameter& param = Parameter());
        ~SoraEventWorld();
        
        void update(float32 dt);
        
        void enter(EventHandlerPtr eh);
        void leave(EventHandlerPtr eh);
        
        bool init();
        
        void shutdown();
        
        void broadcasting(SoraEvent* evet);
        
        static SoraEventWorld& defaultWorld(const Parameter& param = Parameter());
        
    private:
        SoraScopedPtr<SoraEventWorldImpl> mImpl;
        bool mFixedStep;
    };
    
    static void EnterDefaultEventWorld(SoraEventHandler* handler) {
        SoraEventWorld::defaultWorld().enter(handler);
    }
    
    static void LeaveDefaultEventWorld(SoraEventHandler* handler) {
        SoraEventWorld::defaultWorld().leave(handler);
    }
    
} // namespace sora 

#endif
