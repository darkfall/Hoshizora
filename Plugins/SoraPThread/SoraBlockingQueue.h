//
//  SoraBlockingQueue.h
//  Sora
//
//  Created by Ruiwei Bu on 7/6/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraBlockingQueue_h
#define Sora_SoraBlockingQueue_h

#include "SoraPlatform.h"

#include "SoraMutex.h"
#include "SoraCondition.h"

#include <cassert>
#include <deque>

namespace sora {
    
    template<typename T>
    class SoraBlockingQueue: uncopyable {
    public:
        SoraBlockingQueue():
        mutex(),
        cond(mutex),
        queue() {
        }
        
        void put(const T& x) {
            SoraMutexGuard lock(mutex);
            queue.push_back(x);
            cond.notify();
        }
        
        T take() {
            SoraMutexGuard lock(mutex);
            while(queue.empty())  
                cond.wait();
            
            assert(!queue.empty());
            T front(queue.front());
            queue.pop_front();
            return front;
        }
        
        size_t size() const {
            SoraMutexGuard lock(mutex);
            return queue.size();
        }
        
    private:
        mutable SoraMutexLock mutex;
        SoraCondition cond;
        
        std::deque<T> queue;
    };
    
} // namespace sora

#endif
