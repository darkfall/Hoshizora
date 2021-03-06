//
//  SoraCountDownLatch.h
//  Sora
//
//  Created by Ruiwei Bu on 7/6/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraCountDownLatch_h
#define Sora_SoraCountDownLatch_h

#include "SoraPlatform.h"

#include "SoraMutex.h"
#include "SoraCondition.h"


namespace sora {
    
    class SoraCountDownLatch: uncopyable {
    public:
        explicit SoraCountDownLatch(int32 count);
        
        void wait();
        void countDown();
        int32 getCount() const;
        
    private:
        mutable SoraMutexLock mutex;
        SoraCondition condition;
        int32 count;
    };
    
}

#endif
