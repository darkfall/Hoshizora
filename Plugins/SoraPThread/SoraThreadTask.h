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

namespace sora {
    
    class SoraThreadTask: uncopyable {
    public:
        SoraThreadTask() {}
        
        virtual void run(void* arg) = 0;
    };
    
    class SoraCThreadTask: public SoraThreadTask {
    public:
        typedef void (*CThreadTask)(void* arg);
        SoraCThreadTask(CThreadTask task): mCTask(task) {}
        
        void run(void* arg) {
            mCTask(arg);
        }
        
    private:
        CThreadTask mCTask;
    };
    
    
} // namespace sora

#endif
