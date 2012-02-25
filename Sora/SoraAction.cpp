//
//  SoraAction.cpp
//  SoraF
//
//  Created by Ruiwei Bu on 2/24/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "SoraAction.h"
#include "SoraModifierAdapter.h"
#include "SoraForEach.h"

namespace sora {
    
    void SoraModifierAdapterAction::step(float dt) {
        {
            if(mAdapter) {
                mDone = mAdapter->update(dt);
            }
        }
    }
    
    bool SoraModifierAdapterAction::isDone() const {
        return mDone;
    }
    
    SoraActionSequence::SoraActionSequence():
    mCurrentAction(-1) {
        
    }
    
    SoraActionSequence::~SoraActionSequence() {
        
    }
    
    void SoraActionSequence::startWithTarget(SoraObject* target) {
        mCurrentAction = 0;
        
        SoraAction::startWithTarget(target);
    }
    
    bool SoraActionSequence::initWithActions(SoraAction* action, ...) {        
        va_list list;
        va_start(list, action);
     
        while(action) {
            mActions.push_back(action);
            
            action = va_arg(list, SoraAction*);
        }
        
        va_end(list);
        return true;
    }
    
    bool SoraActionSequence::isDone() const {
        return mCurrentAction == mActions.size();
    }
    
    void SoraActionSequence::step(float dt) {
        if(mCurrentAction != -1 && !mActions.empty() &&
           mCurrentAction < mActions.size()) {
            SoraAction* action = mActions[mCurrentAction];
            
            action->step(dt);
            if(action->isDone()) {
                mCurrentAction ++;
            }
        }
    }
    
    SoraActionContainer::~SoraActionContainer() {
        
    }
    
    void SoraActionContainer::add(const SoraAction::Ptr& act) {
        mActions.push_back(act);
        act->startWithTarget(getOwner());
    }
    
    SoraAction::Ptr SoraActionContainer::del(const SoraAction::Ptr& act) {
        SoraAction::Ptr aa = act;
        mActions.remove(act);
        return aa;
    }
    
    void SoraActionContainer::update(float dt) {
        ActionList::iterator it = mActions.begin();
        while(it != mActions.end()) {
            (*it)->step(dt);
            if((*it)->isDone()) {
                it = mActions.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    
} // namesapce sora
