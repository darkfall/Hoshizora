//
//  SoraEventRouter.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/23/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraEventRouter.h"
#include "../SoraPreDeclare.h"

namespace sora {
    
    struct TempEventHandler {
        
    };
    
    SoraEventRouter::SoraEventRouter() {
        
    }
    
    void SoraEventRouter::handleEvent(SoraEvent* evt) {
        mConnections.sig(evt);
    }

} // namespace sora