//
//  Header.h
//  Sora
//
//  Created by Ruiwei Bu on 8/23/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraMessageRouter_h
#define Sora_SoraMessageRouter_h

#include "SoraEventWorld.h"
#include "../signal/SoraSignal.h"

namespace sora {
    
    /**
     * A EventRouter can redirect events to others
     * Event it's not a EventHandler
     * Useful for message handling
     **/
    struct SoraEventRouter: public SoraEventHandler {
        typedef SoraEvent EventType;
        typedef SoraSignal<void(EventType*)> SignalType;
        
        SoraEventRouter();
        virtual ~SoraEventRouter() {}
        
        /**
         * Redirect events to signal connections
         * Inherited from SoraEventHandler
         **/
        virtual void handleEvent(SoraEvent*);
        
        /**
         * Connect to the router using direct function
         * Class can be non SoraEventHandler
         * But function prototype should be void(SoraEvent*)
         * And have to determine what event it is
         * EventHandlers can use like SoraBind(this, &MY_CLASS::handleEvent)
         * and let it handle the event itself
         **/
        template<typename T>
        SoraConnection connect(const T&);
        
    private:
        SignalType mConnections;
    };
    
    template<typename T>
    SoraConnection SoraEventRouter::connect(const T& func) {
        return mConnections.connect(SignalType::slot_type(func));
    }
    
} // namespace sora


#endif
