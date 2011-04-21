//
//  SoraPlaybackEvent.h
//  Sora
//
//  Created by Griffin Bu on 4/19/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef SORA_PLAYBACK_EVENT_H_
#define SORA_PLAYBACK_EVENT_H_

#include "SoraEvent.h"

namespace sora {
    
    /* 
        Base event class for playback events, such as a music, a soundeffect or a movie
     */
    
    enum SORA_PLAYBACK_EVENT {
        SORAPB_EV_NULL,
        SORAPB_EV_PLAY_STARTED,
        SORAPB_EV_PLAY_ENDED,
        SORAPB_EV_PLAY_PAUSED,
        SORAPB_EV_PLAY_RESUMED,
        SORAPB_EV_PLAY_STOPPED,
    };
    
    class SoraPlaybackEvent: public SoraEvent {
    public:
        SoraPlaybackEvent(): eventType(SORAPB_EV_NULL) {}
        SoraPlaybackEvent(SORA_PLAYBACK_EVENT event): eventType(event) {
            _setEventName(event);
        }
        
        SORA_PLAYBACK_EVENT getEventType() const { return eventType; }
        void setEventType(SORA_PLAYBACK_EVENT event) { 
            eventType = event;
            _setEventName(event);
        }
        
    private:
        void _setEventName(SORA_PLAYBACK_EVENT event) {
            switch(event) {
                case SORAPB_EV_PLAY_STARTED: setName(STR_ID_PLAY_STARTED); break;
                case SORAPB_EV_PLAY_ENDED: setName(STR_ID_PLAY_ENDED); break;
                case SORAPB_EV_PLAY_PAUSED: setName(STR_ID_PLAY_PAUSED); break;
                case SORAPB_EV_PLAY_RESUMED: setName(STR_ID_PLAY_RESUMED); break;
                case SORAPB_EV_PLAY_STOPPED: setName(STR_ID_PLAY_STOPPED); break;
            }
        }
        SORA_PLAYBACK_EVENT eventType;
    };
    
    class SoraPlaybackEventHandler: public SoraEventHandler {
    public:
        SoraPlaybackEventHandler(): pEventHandler(NULL) {}
        
        void registerEventHandler(SoraEventHandler* pHandler) {
            pEventHandler = pHandler;
        }
        
        void publishEvent(SORA_PLAYBACK_EVENT eventType) {
            event.setEventType(eventType);
			event.setSource(this);
            if(pEventHandler)
                pEventHandler->handleEvent(&event);
            this->handleEvent(&event);
        }
        
        // default does nothing
        virtual void onSoundEvent(const SoraPlaybackEvent* pEvent) {}
        
    private:
        SoraEventHandler* pEventHandler;
        
        SoraPlaybackEvent event;
    };
} // namespace sora

#endif