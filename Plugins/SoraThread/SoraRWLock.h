//
//  SoraRWLock.h
//  Sora
//
//  Created by Ruiwei Bu on 7/7/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraRWLock_h
#define Sora_SoraRWLock_h

#include "SoraPlatform.h"

/*

 */
#include "SoraRWLockImplPosix.h"

namespace sora {
    
    class SoraRWLock: public SoraRWLockImpl {
        SoraRWLock() {}
        ~SoraRWLock() {}
        
        void readLock() {
            readLockImpl();
        }
        
        void writeLock() {
            writeLockImpl();
        }
        
        void unlock() {
            unlockImpl();
        }
    }
    
} // namespace sora



#endif
