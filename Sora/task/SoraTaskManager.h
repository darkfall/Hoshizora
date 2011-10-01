//
//  SoraTaskManager.h
//  Sora
//
//  Created by Ruiwei Bu on 9/29/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraTaskManager_h
#define Sora_SoraTaskManager_h

#include "SoraPlatform.h"
#include "SoraUncopyable.h"
#include "SoraAutoPtr.h"
#include "SoraSignal.h"

#ifdef SORA_ENABLE_MULTI_THREAD
#include "thread/SoraThreadPool.h"
#endif

#include <list>

namespace sora {
    
    class SoraAbstractTask;
    class SoraEventHandler;
    class SoraTaskNotification;
    
    class SoraTaskManager: public SoraUncopyable {
        friend class SoraAbstractTask;
        
    public:
        SoraTaskManager(bool multithreaded=false);
        ~SoraTaskManager();
        
        void start(SoraAbstractTask* task);
        
        // request to cancel all tasks
        void cancelAll();
        
#ifdef SORA_ENABLE_MULTI_THREAD
        void joinAll();
#endif
        
    private:
        void taskFinished(SoraAbstractTask* task);
        void taskStarted(SoraAbstractTask* task);
        void taskCanceld(SoraAbstractTask* task);
        void taskProgress(SoraAbstractTask* task);
        
    public:
        typedef SoraAutoPtr<SoraAbstractTask>           TaskPtr;
        typedef std::list<TaskPtr>                      TaskList;
        typedef SoraSignal<void(SoraTaskNotification*)> NotificationSignal;
        
        SoraAbstractTask* taskByName(const std::string& name) const;
        
        int count() const;
        
        bool isMultiThreaded() const;
        
        const TaskList& allTasks() const;
        
        // notifications would be posted under SoraTaskNotification subclass
        
        template<typename T>
        SoraConnection subscribeToNotification(const T& func);
        
        static SoraTaskManager& defaultManager(bool multiThreaded=false);
        
        // start managed in defaultManager
        static void StartTask(SoraAbstractTask* task);
#ifdef SORA_ENABLE_MULTI_THREAD
        static void StartAsyncTask(SoraAbstractTask* task);
#endif
        
    private:
        bool mMultiThread;
        
        TaskList mTasks;
        
        NotificationSignal mNoficationSig;
        
#ifdef SORA_ENABLE_MULTI_THREAD
        SoraThreadPool mThreadPool;
        SoraMutexLock mMutex;
        
        void taskRun(void* task);
#endif
    };
    
    template<typename T>
    inline SoraConnection SoraTaskManager::subscribeToNotification(const T& func) {
        return mNoficationSig.connect(func);
    }

} // namespace sora


#endif
