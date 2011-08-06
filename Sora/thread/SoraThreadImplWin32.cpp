//
//  SoraThreadImplWin32.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/7/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraThreadImplWin32.h"

#ifdef OS_WIN32

namespace sora {
    
	DWORD WINAPI SoraThreadImpl::_ThreadProc(LPVOID param) {
        SoraThreadImpl* pt = static_cast<SoraThreadImpl*>(param);
        if(pt) {
            pt->setActiveImpl(true);
            
            SoraThreadTask task(pt->getThreadTaskImpl());
            if(task.isValid())
                task();
            
            pt->setActiveImpl(false);
            return 0;
        }
		return 0;
    }
    
    
    
} // namespace sora

#endif