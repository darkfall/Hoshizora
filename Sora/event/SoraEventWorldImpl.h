//
//  SoraEventWorldImpl.h
//  Sora
//
//  Created by Ruiwei Bu on 8/5/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraEventWorldImpl_h
#define Sora_SoraEventWorldImpl_h

#include "SoraPlatform.h"
#include "SoraScopedPtr.h"
#include "uncopyable.h"

#include <vector>

namespace sora {    

    class SoraEvent;
    class SoraSystemEvent;
    class SoraEventHandler;
    class SoraEventFactory;
    
    namespace detail {
        struct EventWorldImpl: uncopyable {
            virtual ~EventWorldImpl() {}
            
            virtual void update(float32 dt) = 0;
            virtual void enter(SoraEventHandler* handler) = 0;
            virtual void leave(SoraEventHandler* handler) = 0;
            
            virtual bool init() = 0;
            virtual void shutdown() = 0;
            virtual void broadcasting(SoraEvent* evt) = 0;
            virtual void broadcastingDirect(SoraEvent* evt) = 0;
        };
    }
    
#ifdef SORA_ENABLE_MULTI_THREAD
    class SoraEventWorldImpl: public detail::EventWorldImpl {
    public:
        void update(float32 dt);
        void enter(SoraEventHandler* handler);
        void leave(SoraEventHandler* handler);
        bool init();
        void shutdown();
        void broadcasting(SoraEvent* evt);
        void broadcastingDirect(SoraEvent* evt);
    };
    
#else 
    class SoraEventWorldImpl: public detail::EventWorldImpl {
    public:
        typedef std::vector<SoraEvent*> EventList;
        typedef std::vector<SoraEventHandler*> EventHandlerList;
        
        SoraEventWorldImpl();
        ~SoraEventWorldImpl();
        
        void update(float32 dt);
        void enter(SoraEventHandler* handler);
        void leave(SoraEventHandler* handler);
        bool init();
        void shutdown();
        
        void broadcasting(SoraEvent* evt);
        void broadcastingDirect(SoraEvent* evt);
        
    private:
        inline void doBroadCasting(SoraEvent* evt, bool destroy=true);
        
        bool addHandler(SoraEventHandler* handler);
        void removeHandler(SoraEventHandler* handler);
        
        void swapEventList() throw();
        
        SoraSystemEvent* mUpdateEvt;
        SoraEventFactory* mFactory;
        
        EventHandlerList mHandlers;
        SoraScopedPtr<EventList> mCurrEvtList;
        SoraScopedPtr<EventList> mNextEvtList;
    };
#endif // SORA_ENABLE_MULTI_THREAD
    
} // namespace sora


#endif
