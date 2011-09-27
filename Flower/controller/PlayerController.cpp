//
//  PlayerController.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "PlayerController.h"
#include "config/SoraConfigParser.h"

#include "ThemeController.h"
#include "ConfigController.h"

#include "../core/player.h"
#include "../core/localplayer.h"
#include "../core/remoteplayer.h"
#include "../core/aiplayer.h"

namespace flower {
    
    PlayerController::PlayerController() {
        Player::setPlayerController(this);
        Player::setMaxSpeed(ConfigController::MaxSpeed);
    }
    
    PlayerController::~PlayerController() {
        if(mLocalPlayer.mPlayer)
            delete mLocalPlayer.mPlayer;
        if(mRemotePlayer.mPlayer)
            delete mRemotePlayer.mPlayer;
        if(mAIPlayer.mPlayer)
            delete mAIPlayer.mPlayer;
    }
    
    bool PlayerController::loadConfig(const std::wstring& path) {
        sora::SoraConfigParser parser;
        if(parser.open(path)) {
            if(parser.toNode("/players")) {
                parser.toFirstChild();
                do {
                    PlayerConfig config;
                    config.mPlayerId = parser.getString("id");
                    config.mTextureId = parser.getString("texture");
                    
                    if(!config.mPlayerId.empty() &&
                       !config.mTextureId.empty()) {
                        mPlayerConfigs.insert(std::make_pair(config.mPlayerId, config));
                    }
                    
                } while (parser.toNextChild());
                
                return true;
            }
        }
        
        return false;
    }
    
    Player* PlayerController::createPlayer(float x, float y, const std::string& playerId, PlayerType type) {
        PlayerInfo info;
        
        PlayerConfigMap::const_iterator it = mPlayerConfigs.end();
        switch(type) {
            case PT_AI:
                it = mPlayerConfigs.find("AIPlayer");
                break;
                
            case PT_LOCAL_PLAYER:
                it = mPlayerConfigs.find("LocalPlayer");
                break;
                
            case PT_REMOTE_PLAYER:
                it = mPlayerConfigs.find("RemotePlayer");
                break;
        }
        
        sora_assert(it != mPlayerConfigs.end());
        
        Player* player;
        switch(type) {
            case PT_AI:
                player = new AIPlayer;
                break;
            case PT_LOCAL_PLAYER:
                player = new LocalPlayer;
                break;
            case PT_REMOTE_PLAYER:
                player = new RemotePlayer;
                break;
        }
        
        sora_assert(player);
        
        sora::SoraSprite* spr = ThemeController::Instance()->getSprite(it->second.mTextureId);
        sora_assert(spr);
        
        player->setSprite(spr);
        player->setPosition(x, y);
        
        info.mPlayerId = playerId;
        info.mPlayer = player;
        
        switch(type) {
            case PT_AI:
                mAIPlayer = info;
                break;
            case PT_LOCAL_PLAYER:
                mLocalPlayer = info;
                break;
            case PT_REMOTE_PLAYER:
                mRemotePlayer = info;
                break;
        }
    }
    

    void PlayerController::update(float dt) {
        if(mLocalPlayer.mPlayer)
            mLocalPlayer.mPlayer->update(dt);
        if(mAIPlayer.mPlayer)
            mAIPlayer.mPlayer->update(dt);
        if(mRemotePlayer.mPlayer)
            mRemotePlayer.mPlayer->update(dt);
    }
    
    void PlayerController::render() {
        if(mLocalPlayer.mPlayer)
            mLocalPlayer.mPlayer->render();
        if(mAIPlayer.mPlayer)
            mAIPlayer.mPlayer->render();
        if(mRemotePlayer.mPlayer)
            mRemotePlayer.mPlayer->render();
    }
    
} // namespace flower