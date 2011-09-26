//
//  mover.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "mover.h"
#include "object.h"

namespace atd {
    
    Mover::Mover(Object* obj):
    SoraComponent(obj, false),
    mCurrTime(0.f),
    mTotalTime(0.f),
    mIsMoving(false),
    mObject(obj->getMovable()) {
    }
    
    void Mover::moveTo(const Vector2& end) {
        mMovingPoints.push(end);
    }
    
    void Mover::onUpdate(float dt) {
        if(mEnd.empty() && mMovingPoints.size() != 0) {
            mEnd = mMovingPoints.front();
            mMovingPoints.pop();
            
            mIsMoving = true;
            mObject = static_cast<Object*>(getOwner())->getMovable();
            
            mCurrTime = 0.f;
            
            sora_assert(mObject);
            mObject->mSpeed.setDirection(atan2f(mEnd.y-mObject->getPosition().y, mEnd.x-mObject->getPosition().x));
            mTotalTime = (mEnd - mObject->getPosition()).length() / mObject->mSpeed;
        }
        
        if(mIsMoving && mObject) {
            mCurrTime += dt;
            mObject->transform(dt);
            
            if(mCurrTime >= mTotalTime) {
                mObject->setPosition(mEnd);

                if(mMovingPoints.size() == 0) {
                    getOwner()->sendMessage(SID_EVENT_MOVE_END, this);
                } else {
                    mEnd.clear();
                }
            }
        }
    }
    
    bool Mover::isMoving() const {
        return mIsMoving;
    }
    
    void Mover::onMessage(sora::SoraMessageEvent* evt) {
        
    }

    TrackerMover::TrackerMover():
    mObject(0),
    mTrackerObject(0) {
    }
    
    void TrackerMover::trackObject(Movable* obj, Movable* tracker) {
        mObject = obj;
        mTrackerObject = tracker;
        
        mIsMoving = true;
    }
    
    void TrackerMover::onUpdate(float dt) {
        if(mIsMoving && mObject) {
            mObject->transform(dt);
            
            Vector2 vpos = mTrackerObject->getPosition() - mObject->getPosition();
            mObject->mSpeed.setDirection(atan2f(vpos.y, vpos.x));
            
            if(mTrackerObject && mTrackerObject->getCollidor()->testPoint(mObject->getPosition())) {
                mIsMoving = false;
                
                mObject->setPosition(mTrackerObject->getPosition());
                mMoveEndSig(*this);
                mObject->onCollision(mTrackerObject);
            }
        }
    }
}