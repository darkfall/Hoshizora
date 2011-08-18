//
//  SoraSignal.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/2/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraSignal.h"

namespace sora {
    
    namespace signal {
        SignalImpl::~SignalImpl() {
            IteratorType it = mConnections.begin();
            while(it != mConnections.end()) {
                it->second.setControl(false);
                ++it;
            }
            mConnections.clear();
        }
    }
    
} // namespace sora
