
//
//  SoraFSM.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/17/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraFSM.h"
#include "SoraSingletonHolder.h"

namespace sora {
    
    SoraFSMManager::SoraFSMManager(): 
    mGlobalState(NULL),
    mCurrentState(NULL),
    mPreviousState(NULL) {
        
    }
    
    SoraFSMManager::~SoraFSMManager() {
        FSMStateMap::iterator itState = mStates.begin();
        while(itState != mStates.end()) {
            itState->second->onDestroy();
            itState->second->mManager = NULL;
            ++itState;
        }
    }
    
    SoraFSMManager& SoraFSMManager::defState(SoraFSMState* state, const char* name) {
        stringId uniqueId = GetUniqueStringId(name);
        mStates.insert(std::make_pair(uniqueId, state));
        state->onInitiate();
        state->mManager = this;
        return *this;
    }
    
    void SoraFSMManager::delState(const char* name) {
        stringId uniqueId = GetUniqueStringId(name);
        FSMStateMap::iterator itState = mStates.find(uniqueId);
        if(itState != mStates.end()) {
            itState->second->onDestroy();
            itState->second->mManager = NULL;
            mStates.erase(itState);
        }
    }
    
    void SoraFSMManager::switchToState(const char* name) {
        stringId uniqueId = GetUniqueStringId(name);
        FSMStateMap::iterator itState = mStates.find(uniqueId);
        if(itState != mStates.end()) {
            switchToState(itState->second);
        }
    }
    
    void SoraFSMManager::switchToState(SoraFSMState* state) {
        assert(state);
        
        if(mCurrentState != NULL) {
            mCurrentState->onLeave();
            mPreviousState = mCurrentState;
        }
        mCurrentState = state;
        if(mCurrentState)
            mCurrentState->onEnter();
    }
    
    SoraFSMState* SoraFSMManager::getCurrentState() const {
        return mCurrentState;
    }
    
    SoraFSMState* SoraFSMManager::getPreviousState() const {
        return mPreviousState;
    }
    
    SoraFSMState* SoraFSMManager::getGlobalState() const {
        return mGlobalState;
    }
    
    void SoraFSMManager::onUpdate(float32 dt) {
        if(mCurrentState)
            mCurrentState->onUpdate(dt);
        if(mGlobalState)
            mGlobalState->onUpdate(dt);
    }
   
    void SoraFSMManager::onRender() {
        if(mCurrentState)
            mCurrentState->onRender();
        if(mGlobalState)
            mGlobalState->onRender();
    }
    
    bool SoraFSMManager::returnToPreviousState() {
        if(mPreviousState != NULL) {
            mCurrentState->onLeave();
            mCurrentState = mPreviousState;
            mCurrentState->onEnter();
            return true;
        }
        return false;
    }
    
    void SoraFSMManager::setGlobalState(SoraFSMState* state) {
        mGlobalState = state;
    }
    
    SoraFSMState* SoraFSMManager::operator[](const char* name) {
        stringId uniqueId = GetUniqueStringId(name);
        FSMStateMap::iterator itState = mStates.find(uniqueId);
        if(itState != mStates.end()) {
            return itState->second;
        }
        return NULL;
    }
    
    namespace {
        static SoraSingletonHolder<SoraFSMManager> fsm;
    }
    
    SoraFSMManager& SoraFSMManager::defaultFSMManager() {
        return *fsm.get();
    }
    
    
} // namespace sora