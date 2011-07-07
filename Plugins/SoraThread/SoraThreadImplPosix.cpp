//
//  SoraThreadImplPosix.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/7/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraThreadImplPosix.h"

namespace sora {
    
    int32 SoraThreadImpl::active_thread_num = 0;

    void* SoraThreadImpl::entry(void* pthis) {
        SoraThreadImpl* pt = static_cast<SoraThreadImpl*>(pthis);
        
        ++active_thread_num;
        pt->setActiveImpl(true);
        
        SoraThreadTask task(pt->getThreadTaskImpl());
        if(task.isValid())
            task( pt->getargImpl() );
        
        pthread_exit(0);
        pt->setActiveImpl(false);
        --active_thread_num;
        return 0;
    }
    
} // namespace sora
