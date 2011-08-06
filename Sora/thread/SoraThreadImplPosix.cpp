//
//  SoraThreadImplPosix.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/7/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraThreadImplPosix.h"

#if !defined(OS_WIN32) || defined(SORA_WIN32_PTHREAD)
namespace sora {
 
    void* SoraThreadImpl::entry(void* pthis) {
        SoraThreadImpl* pt = static_cast<SoraThreadImpl*>(pthis);
        
        pt->setActiveImpl(true);
        
        SoraThreadTask task(pt->getThreadTaskImpl());
        if(task.isValid())
            task( );
        
        pthread_exit(0);
        pt->setActiveImpl(false);
        return 0;
    }

} // namespace sora

#endif
