//
//  SoraMutexImplWin32.h
//  Sora
//
//  Created by Ruiwei Bu on 7/7/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraMutexImplWin32_h
#define Sora_SoraMutexImplWin32_h

#include "SoraPlatform.h"

#ifdef OS_WIN32

#include <windows.h>

#include "SoraException.h"
#include "uncopyable.h"

namespace sora {
    
    class SoraMutexLockImpl: public uncopyable {
    protected:
        SoraMutexLockImpl() {
            InitializeCriticalSectionAndSpinCount(&cs, 4000);
        }
        
        ~SoraMutexLockImpl() {
            DeleteCriticalSection(&cs);
        }
        
        inline void lockImpl() {
            try {
                EnterCriticalSection(&cs);
            } catch(...) {
                THROW_SORA_EXCEPTION("Cannot lock mutex");
            }
        }
        
        inline void unlockImpl() {
            try {
                LeaveCriticalSection(&cs);
            } catch(...) {
                THROW_SORA_EXCEPTION("Cannot lock mutex");
            }
        }
        
        inline void* getSysMutexImpl() {
            return &cs;
        }
        
    private:
        CRITICAL_SECTION cs;
    };
    
    
} // namespace sora

#endif //OS_WIN32

#endif
