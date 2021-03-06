//
//  SoraThreadTask.h
//  Sora
//
//  Created by Ruiwei Bu on 7/5/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraThreadTask_h
#define Sora_SoraThreadTask_h

#include "SoraPlatform.h"
#include "uncopyable.h"

#include "SoraAutoPtr.h"

namespace sora {
    
    class SoraThreadTaskImpl: uncopyable {
    public:
        virtual void operator()(void* arg) = 0;
    };
    
    template<typename T>
    class SoraMemberThreadTask: public SoraThreadTaskImpl {
    public:
        typedef void (T::*ThreadFunc)(void* args);
        SoraMemberThreadTask(ThreadFunc func, T* obj):
        _func(func),
        _obj(obj) {}
        
        void operator()(void* arg) {
            (_obj->*_func)(arg);
        }
        
    private:
        ThreadFunc _func;
        T* _obj;
    };
    
    class SoraCThreadTask: public SoraThreadTaskImpl {
    public:
        typedef void (*CThreadTask)(void* arg);
        SoraCThreadTask(CThreadTask task): mCTask(task) {}
        
        void operator()(void* arg) {
            if(mCTask)
                mCTask(arg);
        }
        
    private:
        CThreadTask mCTask;
    };
    
    class SoraThreadTask {
    public:
        SoraThreadTask();
        SoraThreadTask(void (*ThreadFunc)(void* arg));
        template<typename T>
        SoraThreadTask(void (T::*ThreadFunc)(void* args), T* obj) {
            setAsMemberFunc(ThreadFunc, obj);
        }
        
        template<typename T>
        void setAsMemberFunc(void (T::*ThreadFunc)(void* args), T* obj);
        void setAsCFunc(void (*ThreadFunc)(void* arg));
        
        void run();
        void operator()();
        
        void setArg(void* arg);
        void* getArg() const;
        
        bool isValid() const;
        
        SoraThreadTask& operator=(const SoraThreadTask& rhs) {
            func = rhs.func;
            iarg = rhs.iarg;
            return *this;
        }
        
    private:
        SoraAutoPtr<SoraThreadTaskImpl> func;
        
        void* iarg;
    };
    
    template<typename T>
    void SoraThreadTask::setAsMemberFunc(void (T::*ThreadFunc)(void* args), T* obj) {
        SoraMemberThreadTask<T>* memberTask = new SoraMemberThreadTask<T>(ThreadFunc, obj);
        if(!memberTask)
            THROW_SORA_EXCEPTION(RuntimeException, "Error creating member thread task");
        else
            func = memberTask;
    }
    
    template<typename T>
    static SoraThreadTask ThreadTask(void (T::*ThreadFunc)(void*), T* obj) {
        SoraThreadTask task;
        task.setAsMemberFunc(ThreadFunc, obj);
        return task;
    }
    
    static SoraThreadTask ThreadTask(void (*ThreadFunc)(void*)) {
        return SoraThreadTask(ThreadFunc);
    }
} // namespace sora

#endif
