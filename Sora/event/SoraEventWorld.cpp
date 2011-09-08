//
//  SoraEventWorld.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/5/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraEventWorld.h"
#include "SoraEventWorldImpl.h"
#include "SoraSingletonHolder.h"
#include "SoraException.h"
#include "SoraSystemEvents.h"
#include "SoraCore.h"

namespace sora {
    
    SoraEventWorld::SoraEventWorld(const Parameter& param) {
        if(param.isMultiThread()) {
#ifdef SORA_ENABLE_MULTI_THREAD
            mImpl.reset(new SoraEventWorldImpl());
#else 
            mImpl.reset(new SoraEventWorldImpl());
            THROW_SORA_EXCEPTION(RuntimeException, "You must enable SORA_ENALB_MULTI_THREAD to use multi threaded EventWorld");
#endif
        } else {
            mImpl.reset(new SoraEventWorldImpl());
        }
        mFixedStep = param.isFixedStep();
        
        if(!init()) 
            THROW_SORA_EXCEPTION(RuntimeException, "Error initializing SoraEventWorld");
    }
    
    SoraEventWorld::~SoraEventWorld() {
        
    }
    
    void SoraEventWorld::update(float32 dt) {
        if(mFixedStep) {
            uint32 step = static_cast<uint32>(dt / SoraCore::Instance()->getDelta());
            if(step >= 3) {
                // to many steps?
                return;
            }
            for(uint32 i=0; i<step; ++i)
                mImpl->update(dt);
        } else {
            mImpl->update(dt);
        }
    }
    
    void SoraEventWorld::enter(EventHandlerPtr handler) {
        handler->enable(this);
        mImpl->enter(handler);
    }
    
    void SoraEventWorld::leave(EventHandlerPtr handler) {
        if(!handler->isInWorld(this)) {
            return;
        }
        
        handler->disable();
        mImpl->leave(handler);
    }
    
    bool SoraEventWorld::init() {
        return mImpl->init();
    }
    
    void SoraEventWorld::broadcasting(SoraEvent* evt) {
        if(IsSystemEvent(evt)) {
            THROW_SORA_EXCEPTION(InvalidArgumentException, "Cannot broadcasting a system event");
        }
        mImpl->broadcasting(evt);
    }
    
    namespace {
        static SoraSingletonHolder<SoraEventWorld, SoraEventWorld::Parameter> world;
    }
    
    SoraEventWorld& SoraEventWorld::defaultWorld(const Parameter& param) {
        return *world.get(param);
    }
} // namespace sora
