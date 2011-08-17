//
//  SoraNetwork.h
//  Sora
//
//  Created by Ruiwei Bu on 8/13/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraNetwork_h
#define Sora_SoraNetwork_h

#include "SoraPlatform.h"
#include "SoraDelegate.h"
#include "SoraStream.h"

#include "zeromq/zmq.hpp"

namespace sora {

    /**
     * Sora Network Plugin Based on Zeromq Library
     * Message send and receive would run in a independet thread
     **/
    
    class SoraSocketThread;
    
    class SoraSocket {
    public:
        friend class SoraSocketThread;
        
        typedef SoraAbstractDelegate<SoraSocket> DelegateType;
        typedef SoraAutoPtr<zmq::message_t> MessagePtr;
        
        struct DataInfo {
            void* mData;
            ulong32 mSize;
            
            MessagePtr mMessage;
        };
        
        /**
         * Contructor
         * @param socketType see Zeromq Reference for available socket types
         *  note that socket types must pair between client and server
         *  otherwise connection would fail
         *  available pairs:
                ZMQ_REQ
                ZMQ_REP
                pattern: send, receive, send, receive
         
                ZMQ_DEALER
                ZMQ_REQ, ZMQ_ROUTER, ZMQ_REP
                pattern: unrestricted
         
                ZMQ_ROUTER
                ZMQ_DEALER, ZMQ_REQ, ZMQ_REP
                pattern: unrestricted, messages through router would contain extra
                    data to identify the sender
         
                ZMQ_PUB
                ZMQ_SUB
                pattern: send / receive only
         
                pipeline:
                ZMQ_PUSH
                ZMQ_PULL
         
                for inproc communication:
                ZMQ_PAIR
                ZMQ_PAIR
         */
        SoraSocket();
        
        ~SoraSocket();
        
        bool setup(int32 socketType,
                   int32 ioThread=1);
        
        /**
         * When received the delegate call
         * Users can use getLastEvent to get the last active event
         * @see getLastEvent
         */
        void setDelegate(const DelegateType& del);
        
        enum {
            AD_IGNORE = 0,
            AD_TCP,
            AD_IPC,
            AD_INPROC,
        };  
        
        /**
         * @param if adType is AD_IGNORE, then address itself must identify the address type
         *  such as tcp://, inproc:// 
         **/
        bool bind(const std::string& address, int32 adType=AD_IGNORE);
        bool connect(const std::string& address, int32 adType=AD_IGNORE);
        
        bool sendMessage(void* data, ulong32 size);
        
        template<typename T>
        bool sendMessage(T* data);
        
        /**
         * Received message data are queued by SoraSocket
         * @retval are there message data left in the queue
         */
        bool getReceivedMessageData(DataInfo& data);
        
        std::string getLastErrorMessage() const;
        
        enum {
            EVT_NOTHING = 0,
            EVT_ERROR,
            EVT_BIND,
            EVT_CONNECT,
            EVT_DISCONNECT,
            EVT_SENDSTART,
            EVT_SENDEND,
            EVT_RECEIVE,
        };
        
        int32 getLastEvent() const;
        
    private:
        void setErrorMessage(const std::string& mssg);
        void notify(int32 event);
        
        zmq::context_t* mContext;
        zmq::socket_t* mSocket;
        
        DelegateType* mDelegate;
        
        int32 mLastEvent;
        std::string mErrorMssg;
        
        SoraSocketThread* mThread;
    };
    
    template<typename T>
    bool SoraSocket::sendMessage(T* data) {
        return sendMessage(static_cast<void*>(data), sizeof(T));
    }
    
    
} // namespace sora


#endif
