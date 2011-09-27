//
//  localplayer.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_localplayer_h
#define Sora_iPhone_localplayer_h

#include "player.h"

namespace flower {
    
    class LocalPlayer: public Player {
    public:
        LocalPlayer();
        virtual ~LocalPlayer();
        
        uint32 update(float dt);
        
        void setControllMode(ControlMode mode);
        ControlMode getControlMode() const;
        
        
    private:
        ControlMode mControlMode;

        void accelerometerUpdate();
        void touchUpdate();
    };
    
} // namespace flower

#endif
