//
//  PlayerController.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_PlayerController_h
#define Sora_iPhone_PlayerController_h

#include "SoraPlatform.h"
#include "SoraSingleton.h"

#include "defines.h"
#include <list>
#include <map>

namespace flower {
    
    class Player;
    
    class PlayerController: public sora::SoraSingleton<PlayerController> {
    public:
        friend class sora::SoraSingleton<PlayerController>;
        
        PlayerController();
        ~PlayerController();
        
        bool loadConfig(const std::wstring& path);
        
        Player* createPlayer(float x, float y, const std::string& playerId, PlayerType type);
        
        struct PlayerConfig {            
            std::string mPlayerId;
            std::string mTextureId;
        };
        
        struct PlayerInfo {
            PlayerType mType;
            Player* mPlayer;
            
            std::string mPlayerId;
            
            PlayerInfo():
            mPlayer(0) { }
        };
        
        void update(float dt);
        void render();
        
    private:
        PlayerInfo mAIPlayer;
        PlayerInfo mLocalPlayer;
        PlayerInfo mRemotePlayer;
        
        typedef std::map<std::string, PlayerConfig> PlayerConfigMap;
        PlayerConfigMap mPlayerConfigs;
    };
    
    
} // namespace flower


#endif
