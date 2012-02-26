//
//  SoraAction.h
//  SoraF
//
//  Created by Ruiwei Bu on 2/24/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef SoraF_SoraAction_h
#define SoraF_SoraAction_h

#include "SoraPlatform.h"
#include "SoraAutoPtr.h"
#include "SoraPreDeclare.h"
#include "SoraModifier.h"
#include "SoraRefCounted.h"
#include "SoraFunction.h"

#include <vector>
#include <list>

namespace sora {
    
    class SoraObject;
    class SoraAbstractModifierAdapter;
    
    // interface for action
    class SoraAction: public SoraSharedClass<SoraAction> {
    public:
        SoraAction(): mTarget(0) { }
        virtual ~SoraAction() { }
        
        virtual void startWithTarget(SoraObject* target) {
            mTarget = target;
        }
        
        virtual bool isDone() const {
            return true;
        }
        
        virtual void step(float dt) {
            return;
        }
        
        SoraObject* getTarget() const { return mTarget; }
        
    public:
        SoraObject* mTarget;
    };
    
    class SoraFiniteAction: public SoraAction {
    public:
        virtual ~SoraFiniteAction() { }
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(float, Duration, m);
        
        virtual SoraAction::Ptr reverse() const { return new SoraFiniteAction(); }
        
        virtual void startWithTarget(SoraObject* target) {
            mCurrentTime = 0;
            SoraAction::startWithTarget(target);
        }
        
        virtual void step(float dt) {
            if(mCurrentTime < getDuration()) {
                mCurrentTime += dt;
            }
        }
        
        virtual bool isDone() const {
            return mCurrentTime >= getDuration();
        }
        
        float getCurrentTime() const {
            return mCurrentTime;
        }
        
    private:
        float mCurrentTime;
    };
    
    // a action that makes a action faster or slower
    class SoraSpeedAdjustAction: public SoraAction {
    public:
        SoraSpeedAdjustAction(): mSpeedRatio(1.f), mAction(0) { }
        SoraSpeedAdjustAction(const SoraAction::Ptr& action, float ratio):
        mSpeedRatio(ratio),
        mAction(action) { }
        
        virtual ~SoraSpeedAdjustAction() { }
        
        virtual void step(float dt) {
            if(mAction) {
                mAction->step(dt * mSpeedRatio);
            }
        }
        
        virtual bool isDone() const {
            if(mAction)
                return mAction->isDone();
            return true;
        }
        
        virtual void startWithTarget(SoraObject* target) {
            if(mAction)
                mAction->startWithTarget(target);
            SoraAction::startWithTarget(target);
        }
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(float, SpeedRatio, m);
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraAction::Ptr, Action, m);
        
    public:
        static SoraAction* ActionWithAction(const SoraAction::Ptr& action, float ratio) {
            return new SoraSpeedAdjustAction(action, ratio);
        }
    };
    
    class SoraModifierAdapterAction: public SoraAction {
    public:
        SoraModifierAdapterAction(): mAdapter(0) { }
        SoraModifierAdapterAction(SoraAbstractModifierAdapter* adapter):
        mAdapter(adapter) { }
        
        virtual ~SoraModifierAdapterAction() { }
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraAbstractModifierAdapter*, Adapter, m);
        
        virtual void step(float dt);
        virtual bool isDone() const;
        
        static SoraAction* ActionWithModifierAdapter(SoraAbstractModifierAdapter* adapter) {
            return new SoraModifierAdapterAction(adapter);
        }
        
    private:
        bool mDone;
    };
    
    template<typename T>
    class SoraModifierAction: public SoraAction {
    public:
        SoraModifierAction(): mModifier(0) { }
        SoraModifierAction(SoraModifier<T>* modifier):
        mModifier(modifier) { }
        
        virtual ~SoraModifierAction() { 
            if(mModifier) {
                mModifier->release();
            }
        }
        
        virtual void startWithTarget(SoraObject* target) {
            mDone = false;
            SoraAction::startWithTarget(target);
        }
        
        virtual void step(float dt) {
            if(this->getTarget()) {
                mDone = mModifier->update(dt);
                mModifier->modify((T*)this->getTarget());
            }
        }
        
        virtual bool isDone() const {
            return mDone;
        }
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraModifier<T>*, Modifier, m);
        
        static SoraAction* ActionWithModifier(SoraModifier<T>* modifier) {
            return new SoraModifierAction<T>(modifier);
        }
        
    private:
        bool mDone;
    };
    
    class SoraActionRepeat: public SoraAction {
    public:
        SoraActionRepeat(): mCurrentRepeatTime(0), mAction(0) { }
        SoraActionRepeat(const SoraAction::Ptr& action, int repeat):
        mAction(action),
        mRepeatTimes(repeat) {
            
        }
        virtual ~SoraActionRepeat() { }
        
        virtual void step(float dt) {
            if(mAction) {
                mAction->step(dt);
                
                if(mAction->isDone()) {
                    ++mCurrentRepeatTime;
                    mAction->startWithTarget(this->getTarget());
                }
            }
        }
        
        virtual bool isDone() const {
            return mCurrentRepeatTime > getRepeatTimes();
        }
        
        virtual void startWithTarget(SoraObject* target) {
            mCurrentRepeatTime = 0;
            if(mAction)
                mAction->startWithTarget(target);
            SoraAction::startWithTarget(target);
        }
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(int, RepeatTimes, m);
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraAction::Ptr, Action, m);
        
        static SoraAction* ActionWithRepeatTimes(const SoraAction::Ptr& action, int repeat) {
            return new SoraActionRepeat(action, repeat);
        }

    private:
        int mCurrentRepeatTime;
    };
    
    class SoraActionSequence: public SoraAction {
    public:
        SoraActionSequence();
        virtual ~SoraActionSequence();
        
        void startWithTarget(SoraObject* target);
        // must end in 0(NULL)
        bool initWithActions(SoraAction* action, ...);
        
        virtual bool isDone() const;
        virtual void step(float dt);
        
        void addAction(const SoraAction::Ptr& action);
        
        static SoraAction* ActionWithActions(SoraAction* action, ...);
        
    public:
        typedef std::vector<SoraAction::Ptr> ActionList;

    private:
        ActionList mActions;
        int mCurrentAction;
    };
    
    class SoraDelayAction: public SoraFiniteAction {
    public:
        SoraDelayAction(): mAction(0) { }
        SoraDelayAction(const SoraAction::Ptr& action, float delay):
        mAction(action) { 
            this->setDuration(delay);
        }
        
        virtual ~SoraDelayAction() { }
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraAction::Ptr, Action, m);
        
        virtual bool isDone() const {
            if(mCurrentTime < this->getDuration())
                return false;
            if(mAction)
                return mAction->isDone();
            return true;
        }
        
        virtual void step(float dt) {
            if(mCurrentTime > this->getDuration()) {
                mAction->step(dt);
            } else {
                mCurrentTime += dt;
            }
        }
        
        virtual void startWithTarget(SoraObject* target) {
            mCurrentTime = 0.f;
            SoraFiniteAction::startWithTarget(target);
        }
        
        static SoraAction* ActionWithDelay(const SoraAction::Ptr& action, float delay) {
            return new SoraDelayAction(action, delay);
        }
        
    private:
        float mCurrentTime;
    };
    
    // call a function
    class SoraCallFuncNAction: public SoraAction {
    public:
        typedef SoraFunction<void()> FuncType;
        
        SoraCallFuncNAction() { }
        SoraCallFuncNAction(const FuncType& func):
        mFunc(func) {  }
        
        virtual ~SoraCallFuncNAction() { }
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(FuncType, Func, m);
        
        virtual bool isDone() const {
            return mDone;
        }
        
        virtual void step(float dt) {
            if(!mDone)
                mFunc();
            mDone = true;
        }
        
        virtual void startWithTarget(SoraObject* target) {
            mDone = false;
            SoraAction::startWithTarget(target);
        }
        
        static SoraAction* ActionWithFunction(const FuncType& func) {
            return new SoraCallFuncNAction(func);
        }
        
    private:
        bool mDone;
    };
    
    // call a function with dt param
    class SoraCallFuncAction: public SoraAction {
    public:
        typedef SoraFunction<void(float)> FuncType;
        
        SoraCallFuncAction() { }
        SoraCallFuncAction(const FuncType& func):
        mFunc(func) {  }
        
        virtual ~SoraCallFuncAction() { }
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(FuncType, Func, m);
        
        virtual bool isDone() const {
            return mDone;
        }
        
        virtual void step(float dt) {
            if(!mDone)
                mFunc(dt);
            mDone = true;
        }
        
        virtual void startWithTarget(SoraObject* target) {
            mDone = false;
            SoraAction::startWithTarget(target);
        }
        
        static SoraAction* ActionWithFunction(const FuncType& func) {
            return new SoraCallFuncAction(func);
        }
        
    private:
        bool mDone;
    };

    class SoraActionContainer {
    public:
        SoraActionContainer(SoraObject* owner): mOwner(owner) { }
        ~SoraActionContainer();
        
        void            add(const SoraAction::Ptr& act);
        SoraAction::Ptr del(const SoraAction::Ptr& act);
        
        void update(float dt);
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(SoraObject*, Owner, m);
        
    public:
        typedef std::list<SoraAction::Ptr> ActionList;
        
    private:
        ActionList mActions;
    };
}



#endif
