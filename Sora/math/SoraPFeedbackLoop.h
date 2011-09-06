//
//  SoraFeedbackLoop.h
//  Sora
//
//  Created by Ruiwei Bu on 9/5/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraFeedbackLoop_h
#define Sora_SoraFeedbackLoop_h

#include <cmath>
#include "SoraTimestamp.h"

namespace sora {
    
    /**
     A P feedback loop (proportional feedback loop) is a simple object which 
     moves a system's current state towards a goal, using the resulting error 
     (difference between goal and state as feedback on the next run.
     
     If you need to implement motion controllers, camera controllers, etc...
     then the feedback loop is your friend.
     **/
    
    template<typename T>
    class SoraPFeedbackLoop {
    public:
        SoraPFeedbackLoop();
        void reset(float time, float stepSize, float gain, const T& curState);
        void setGain(float g);
        float getGain() const;
        void setGoal(const T& c);
        const T& getGoal() const;
        void setState(const T& s);
        const T& getState() const;
        void update(float time);
        
    private:
        float mTime;
        float mStepSize;
        float mGain;
        T mGoal;
        T mState;
    };
    
    template<typename T>
    SoraPFeedbackLoop<T>::SoraPFeedbackLoop():
    mTime(0.f),
    mStepSize(0.001f),
    mGain(-1.0f) {
        
    }
    
    template<typename T>
    void SoraPFeedbackLoop<T>::reset(float time, float stepSize, float gain, const T& curState) {
        mTime = time;
        mStepSize = stepSize;
        mGain = gain;
        mState = curState;
    }
    
    template<typename T>
    void SoraPFeedbackLoop<T>::setGain(float g) {
        mGain = g;
    }
    
    template<typename T>
    float SoraPFeedbackLoop<T>::getGain() const {
        return mGain;
    }
    
    template<typename T>
    void SoraPFeedbackLoop<T>::setGoal(const T& g) {
        mGoal = g;
    }
    
    template<typename T>
    const T& SoraPFeedbackLoop<T>::getGoal() const {
        return mGoal;
    }
    
    template<typename T>
    void SoraPFeedbackLoop<T>::setState(const T& s) {
        mState = s;
    }
    
    template<typename T>
    const T& SoraPFeedbackLoop<T>::getState() const {
        return mState;
    }
    
    template<typename T>
    void SoraPFeedbackLoop<T>::update(float dt) {
        float curTime = mTime + dt;
        while(mTime < curTime) {
            mState = mState + (mState - mGoal) * mGain * mStepSize;
            mTime += mStepSize;
        }
    }
    
} // namespace sora

#endif
