//
//  EnemyManager.h
//  Sora
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_EnemyManager_h
#define Sora_EnemyManager_h

#include "SoraPlatform.h"
#include "SoraAutoUpdate.h"
#include "message/SoraMessageEvent.h"

namespace atd {
    
    
    class EnemyManager: public sora::SoraAutoUpdate {
    public:
        EnemyManager();
        ~EnemyManager();
        
        // enemies in SoraAutoListElement
        void onUpdate(float dt);
        
        void postEvent(sora::SoraMessageEvent* evt);
        
    private:
        
    };
    
} // namespace atd


#endif
