//
//  SoraSemaphoreImplWin32.h
//  Sora
//
//  Created by Ruiwei Bu on 7/8/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraSemaphoreImplWin32_h
#define Sora_SoraSemaphoreImplWin32_h

#include "SoraPlatform.h"

#ifdef OS_WIN32
#include <windows.h>
#endif

namespace sora {
    
    class SoraSemaphoreImpl: public uncopyable {
    public:
        SoraSemaphoreImpl(int n, int max) {
            assert( n>=0 && max>0 && n<=max);
            
            sema = CreateSemaphoreW(NULL, n, max, NULL);
        }
        
        ~SoraSemaphoreImpl() {
            CloseHandle(sema);
        }
        
        void setImpl() {
            if(!ReleaseSemaphore(sema, 1, NULL)) {
                THROW_SORA_EXCEPTION("cannot signal semaphore");
            }
        }
        
        void waitImpl() {
            switch(WaitForSingleObject(sema,INFINITE)) {
                case WAIT_OBJECT_0:
                    return;
                default:
                    THROW_SORA_EXCEPTION("Wait for semaphore failed");
            }
        }

        
    private:
        HANDLE sema;
    };
    
} // namespace sora

#endif
