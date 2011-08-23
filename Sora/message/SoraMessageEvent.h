//
//  SoraMessageEvent.h
//  Sora
//
//  Created by Ruiwei Bu on 8/22/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraMessageEvent_h
#define Sora_SoraMessageEvent_h

#include "../SoraEvent.h"
#include "../SoraAny.h"

namespace sora {
    
    /**
     * Template class for a event that contains a message 
     * and AnyData
     **/
    
    template<typename T>
    class SoraMessageEvent: public SoraEvent {
    public:
        typedef T MessageType;
        
        SoraMessageEvent() { }
        
        SoraMessageEvent(EventType message):
        mMessageType(message) { }
        
        template<typename T>
        SoraMessageEvent(EventType message, const T& data):
        mMessageType(message),
        mData(data) { }
        
        virtual ~SoraMessageEvent() { } 
        
        EventType getMessageType() const {
            return mEventType;
        }
        
        template<typename T>
        void setData(const T& data) {
            mData = data;
        }
        
        template<typename T>
        T getData() const {
            return AnyCast<T>(mData);
        }
        
    private:
        MessageType mMessageType;
        SoraAny mData;
    };
    
} // namespace sora



#endif
