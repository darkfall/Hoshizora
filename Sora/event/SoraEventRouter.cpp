//
//  SoraEventRouter.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/23/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraEventRouter.h"
#include "SoraEventWorld.h"
#include "../SoraPreDeclare.h"

namespace sora {
    
    template<typename T>
    SoraEventRouter<T>::SoraEventRouter() {
        SoraEventWorld::defaultWorld().enter(this);
        SoraEventHandler::enableUpdate();
    }
    
    template<typename T>
    void SoraEventRouter<T>::handleEvent(SoraEvent* evt) {
        mConnections.sig(evt);
    }

} // namespace sora