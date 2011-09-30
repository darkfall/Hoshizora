//
//  defines.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_defines_h
#define Sora_iPhone_defines_h

namespace flower {
    
    enum GameMode {
        GM_STORY,
        
        // extended
        GM_TIME_ATTACK,
        GM_UNLIMITED,
        GM_CHALLENGE_WITH_AI,
        GM_CHALLENGE_TIME_ATTACK,
        GM_CHALLENGE_BOMB_ONLY,
    };
    
    enum ObjectType {
        OBJ_ITEM,
        OBJ_BOMB,
    };
    
    
    enum ControlMode {
        CM_TOUCH,

        CM_ACCELEROMETER,
    };
    
    enum PlayerType {
        PT_AI,
        PT_LOCAL_PLAYER,
        PT_REMOTE_PLAYER,
    };
    
    static const char* const SID_EVENT_ITEM_GET = "ItemGet";
    static const char* const SID_EVENT_BOMB_GET = "BombGet";
    static const char* const SID_EVENT_PLAYER_SWAM = "PlayerSwam";
    static const char* const SID_EVENT_PLAYER_DEATH = "PlayerDeath";
    static const char* const SID_EVENT_PLAYER_DESTROY = "PlayerDestroy";
    
} // namespace flower

#endif
