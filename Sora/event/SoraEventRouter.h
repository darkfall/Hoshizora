//
//  Header.h
//  Sora
//
//  Created by Ruiwei Bu on 8/23/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraEventRouter_h
#define Sora_SoraEventRouter_h

#include "../signal/SoraSignal.h"
#include "../SoraEvent.h"

namespace sora {
    
    /**
     * A EventRouter can redirect events to others
     * Event it's not a EventHandler
     * Useful for message handling
     **/
    class SoraEvent;
    
    template<typename T = sora::SoraEvent>
    class SoraEventRouter: public SoraEventHandler {
    public:
        typedef T EventType;
        typedef SoraSignal<void(EventType*)> SignalType;
        
        SoraEventRouter();
        virtual ~SoraEventRouter() {}
        
        /**
         * Redirect events to signal connections
         * Inherited from SoraEventHandler
         **/
        void handleEvent(SoraEvent*);
        
        /**
         * Connect to the router using direct function
         * Class can be non SoraEventHandler
         * But function prototype should be void(SoraEvent*)
         * And have to determine what event it is
         * EventHandlers can use like SoraBind(this, &MY_CLASS::handleEvent)
         * and let it handle the event itself
         **/
        template<typename FN>
        SoraConnection connect(const FN& func) {
            return mConnections.connect(SignalType::slot_type(func));
        }
        
    protected:
        SignalType mConnections;
    };
    
} // namespace sora


#endif
