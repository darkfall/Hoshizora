//
//  SoraEventWorldImpl.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/5/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraEventWorldImpl.h"
#include "SoraEvent.h"
#include "SoraSystemEvents.h"
#include "SoraCore.h"
#include "SoraEventFactory.h"

namespace sora {
    
#ifdef SORA_ENABLE_MULTI_TRHEAD


#else 
    
    SoraEventWorldImpl::SoraEventWorldImpl():
    mHandlers(),
    mCurrEvtList(new EventList()),
    mNextEvtList(new EventList()),
    mUpdateEvt(NULL),
    mFactory(SoraEventFactory::Instance()) {
        
    }
    
    SoraEventWorldImpl::~SoraEventWorldImpl() {
        if(mUpdateEvt != NULL)
            shutdown();
    }
    
    void SoraEventWorldImpl::enter(SoraEventHandler* handler) {
        SoraSystemEvent* evt = (SoraSystemEvent*)mFactory->createEvent();
        evt->setType(SYS_EVT_ADD_HANDLER);
        evt->setSource(handler);
        broadcasting(evt);
    }
    
    void SoraEventWorldImpl::leave(SoraEventHandler* handler) {
        SoraSystemEvent* evt = (SoraSystemEvent*)mFactory->createEvent();
        evt->setType(SYS_EVT_DEL_HANDLER);
        evt->setSource(handler);
        broadcasting(evt);
    }

    bool SoraEventWorldImpl::init() {
        mUpdateEvt = (SoraSystemEvent*)mFactory->createEvent();
        mUpdateEvt->setType(SYS_EVT_UPDATE);
        return true;
    }
    
    void SoraEventWorldImpl::shutdown() {
        float32 dt = 0.f;
        while(!(mCurrEvtList->empty() && mNextEvtList->empty())) {
            update(dt);
        }
        
        mFactory->destroyEvent(mUpdateEvt);
        mUpdateEvt = NULL;
    }
    
    void SoraEventWorldImpl::broadcasting(SoraEvent* evt) {
        mNextEvtList->push_back(evt);
    }
    
    bool SoraEventWorldImpl::addHandler(SoraEventHandler* handler) {
        if(std::find(mHandlers.begin(), mHandlers.end(), handler) == mHandlers.end()) {
            mHandlers.push_back(handler);
        } else 
            THROW_SORA_EXCEPTION(ExistsException, "Handler already exists in the world");
        return true;
    }
    
    void SoraEventWorldImpl::removeHandler(SoraEventHandler* handler) {
        mHandlers.erase(std::remove(mHandlers.begin(),
                                    mHandlers.end(),
                                    handler));
    }
    
    void SoraEventWorldImpl::update(float32 dt) {
        EventList::iterator itEvent = mCurrEvtList->begin();
        EventList::iterator itEnd = mCurrEvtList->end();
        
        for(itEvent; itEvent != itEnd; ++itEvent) {
            if(IsSystemEvent(*itEvent)) {
                SoraSystemEvent* evt = static_cast<SoraSystemEvent*>((*itEvent));
                switch(evt->getType()) {
                    case SYS_EVT_ADD_HANDLER:
                        addHandler((*itEvent)->getSource());
                        break;
                    case SYS_EVT_DEL_HANDLER:
                        addHandler((*itEvent)->getSource());
                        break;
                }
            } else {
                SoraEventHandler* receiver = (*itEvent)->getReceiver();
                if(receiver) 
                    receiver->handleEvent(*itEvent);
                else {
                    EventHandlerList::iterator itHandler = mHandlers.begin();
                    while(itHandler != mHandlers.end()) {
                        if((*itHandler)->isEnabled() && (*itHandler)->getChannel().listenning((*itEvent)->getChannel())) {
                            (*itHandler)->handleWorldEvent(*itEvent);
                        }
                        ++itHandler;
                    }
                }
            }
            
            mFactory->destroyEvent(*itEvent);
        }
                           
        EventHandlerList::iterator itHandler = mHandlers.begin();
        mUpdateEvt->setDelta(dt);
        while (itHandler != mHandlers.end()) {
            if((*itHandler)->isEnabled()) {
                (*itHandler)->handleSystemEvent(mUpdateEvt);
            }
            ++itHandler;
        }
        
        mCurrEvtList->clear();
        swapEventList();
    }
    
    void SoraEventWorldImpl::swapEventList() throw() {
        mCurrEvtList->swap(*mNextEvtList.get());
    }
#endif // SORA_ENABLE_MULTI_TRHEAD

} // namespace sora