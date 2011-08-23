//
//  SoraSocket.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/13/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraSocket.h"
#include "SoraException.h"
#include "thread/SoraThread.h"
#include "thread/SoraThreadTask.h"
#include "thread/SoraBlockingQueue.h"

namespace sora {
    
    class SoraSocketThread {
    public:
        SoraSocketThread():
        mSocket(NULL),
        mParent(NULL) {
            
        }
        
        typedef SoraSocket::DataInfo DataInfo;
        
        void setSocket(zmq::socket_t* socket) {
            mSocket = socket;
            sora_assert(mSocket);
        }
        
        void setParent(SoraSocket* parent) {
            mParent = parent;
            sora_assert(mParent);
        }
        
        void start() {
            sora_assert(mParent);
            sora_assert(mSocket);
            
            SoraThreadTask sendtask(&SoraSocketThread::sendTask, this);
            SoraThreadTask recvtask(&SoraSocketThread::recvTask, this);
            
            int result;
            result = mSendThread.startWithTask(sendtask);
            if(result == 0)
                mParent->setErrorMessage("Error starting message start thread");
            result = mRecvThread.startWithTask(recvtask);
            if(result == 0)
                mParent->setErrorMessage("Error starting message receive thread");
        }
        
        void stop() {
            mSendThread.exit();
            mRecvThread.exit();
        }
                
        void sendTask(void* info) {            
            while(true) {
                DataInfo data = mSendQueue.take();
                                
                if(mSendQueue.size() == 0) {
                    zmq::message_t message(data.mSize);
                    memcpy(message.data(), data.mData, data.mSize);
                    
                    mParent->notify(SoraSocket::EVT_SENDSTART);
                    
                    bool flag = mSocket->send(message);
                    if(!flag) {
                        mParent->setErrorMessage("Error sending message");
                    } else {
                        mParent->notify(SoraSocket::EVT_SENDEND);
                    }
                }
            }
        }
        
        void recvTask(void* info) {            
            while(true) {
                zmq::message_t* message = new zmq::message_t;
                
                bool flag = mSocket->recv(message);
                if(flag) {
                    DataInfo data;
                    data.mData = message->data();
                    data.mSize = message->size();
                    data.mMessage.assign(message);
                    
                    mRecvQueue.put(data);
                    mParent->notify(SoraSocket::EVT_RECEIVE);
                    
                } else {
                    mParent->setErrorMessage("Error receiving message");
                }
            }
        }
        
        size_t receivedSize() const {
            return mRecvQueue.size();
        }
        
        DataInfo takeRecved() {
            return mRecvQueue.take();
        }
        
        void putSend(const DataInfo& info) {
            mSendQueue.put(info);
        }
        
    private:
        zmq::socket_t* mSocket;
        SoraSocket* mParent;
        
        SoraBlockingQueue<DataInfo> mSendQueue;
        SoraBlockingQueue<DataInfo> mRecvQueue;
        
        SoraThread mSendThread;
        SoraThread mRecvThread;
    };
    
    SoraSocket::SoraSocket():
    mContext(NULL),
    mSocket(NULL),
    mDelegate(NULL),
    mThread(NULL),
    mLastEvent(EVT_NOTHING),
    mErrorMssg(std::string()) {

    }
    
    bool SoraSocket::setup(int32 socketType,
                           int32 ioThread) {
        if(mContext || mSocket) {
            setErrorMessage("Socket already initialized");
            return false;
        }
        
        try {   
            mContext = new zmq::context_t(ioThread);
            mSocket = new zmq::socket_t(*mContext, socketType);
        } catch(const zmq::error_t& error) {
            mErrorMssg = error.what();
            
            if(mContext) {
                delete mContext;
                mContext = NULL;
            }
            if(mSocket) {
                delete mSocket;
                mSocket = NULL;
            }
            return false;
        }
        
        mThread = new SoraSocketThread;
        sora_assert(mThread);
        
        mThread->setSocket(mSocket);
        mThread->setParent(this);
        mThread->start();
        
        return true;
    }
    
    SoraSocket::~SoraSocket() {
        if(mThread) {
            mThread->stop();
            delete mThread;
        }
        if(mSocket)
            delete mSocket;
        if(mContext)
            delete mContext;
        if(mDelegate)
            delete mDelegate;
    }
    
    void SoraSocket::setDelegate(const DelegateType& del) {
        mDelegate = del.clone();
    }
   
    bool SoraSocket::bind(const std::string& address, int32 adType) {
        if(!mSocket) {
            setErrorMessage("Socket not initialized");
            return false;
        }
        
        std::string realAdress;
        switch(adType) {
            case AD_IPC: realAdress = "ipc://"; break;
            case AD_TCP: realAdress = "tcp://"; break;
            case AD_INPROC: realAdress = "inproc://"; break;
            case AD_IGNORE:
            default: 
                break;
        }
        try {
            mSocket->bind((realAdress+address).c_str());
        } catch(const zmq::error_t& error) {
            mErrorMssg = error.what();
            return false;
        }
        
        notify(EVT_BIND);
        return true;
    }
                                                                               
    bool SoraSocket::connect(const std::string& address, int32 adType) {
        if(!mSocket) {
            setErrorMessage("Socket not initialized");
            return false;
        }
        
        std::string realAdress;
        switch(adType) {
            case AD_IPC: realAdress = "ipc://"; break;
            case AD_TCP: realAdress = "tcp://"; break;
            case AD_INPROC: realAdress = "inproc://"; break;
            case AD_IGNORE:
            default: 
                break;
        }
        try {
            mSocket->connect((realAdress+address).c_str());
        } catch(const zmq::error_t& error) {
            mErrorMssg = error.what();
            return false;
        }
        
        notify(EVT_CONNECT);
        return true;
    }
    
    void SoraSocket::notify(int32 event) {
        mLastEvent = event;
        if(mDelegate)
            mDelegate->notify(this, *this);
    }
    
    bool SoraSocket::sendMessage(void* data, ulong32 size) {
        DataInfo info;
        
        zmq::message_t* message = new zmq::message_t(size);
        sora_assert(message);
        
        memcpy(message->data(), data, size);
        info.mMessage.assign(message);
        info.mData = message->data();
        info.mSize = size;
        
        sora_assert(mThread);
        mThread->putSend(info);
    }
    
    bool SoraSocket::getReceivedMessageData(DataInfo& info) {
        sora_assert(mThread);
        
        if(mThread->receivedSize() == 0)
            return false;
        else {
            info = mThread->takeRecved();
        }
        return true;
    }
    
    void SoraSocket::setErrorMessage(const std::string& mssg) {
        mErrorMssg = mssg;
        mLastEvent = EVT_ERROR;
    }
    
    std::string SoraSocket::getLastErrorMessage() const {
        return mErrorMssg;
    }
    
    int32 SoraSocket::getLastEvent() const {
        return mLastEvent;
    }
} // namespace sora