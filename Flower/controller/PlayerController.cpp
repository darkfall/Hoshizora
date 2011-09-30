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

#include "../core/object.h"
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
                    
                    config.mCollisionRect = sora::SoraRect(parser.getFloat("collision_x", 0.f),
                                                           parser.getFloat("collision_y", 0.f),
                                                           parser.getFloat("collision_width", 32.f),
                                                           parser.getFloat("collision_height", 32.f));
                    
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
        player->setCollisionRect(it->second.mCollisionRect);
        
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
        return player;
    }
    
    void PlayerController::sendMessage(sora::SoraMessageEvent* mssg) {
        if(mssg->getMessage() == SID_EVENT_PLAYER_DEATH) {
            // do player death
        } else {
            if(mLocalPlayer.mPlayer && mssg->getReceiver() == mLocalPlayer.mPlayer)
                mLocalPlayer.mPlayer->sendMessage(mssg);
            if(mAIPlayer.mPlayer && mssg->getReceiver() == mAIPlayer.mPlayer)
                mAIPlayer.mPlayer->sendMessage(mssg);
            if(mRemotePlayer.mPlayer && mssg->getReceiver() == mRemotePlayer.mPlayer)
                mRemotePlayer.mPlayer->sendMessage(mssg);
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
    
    Player* PlayerController::getLocalPlayer() const {
        return mLocalPlayer.mPlayer;
    }
    
    Player* PlayerController::getRemotePlayer() const {
        return mRemotePlayer.mPlayer;
    }
    
    Player* PlayerController::getAIPlayer() const {
        return mAIPlayer.mPlayer;
    }
    
    bool PlayerController::testObjectOnPlayer(Player* player, Object* obj) {
        if(player) {
            sora::SoraRect rect = player->getCollisionRect();
            if(rect.testPoint(obj->getPositionX(),
                              obj->getPositionY())) {
                obj->onPlayerCollision(player);
                return true;
            }
        }
        return false;
    }
    
    bool PlayerController::testObject(Object* obj) {
        if(testObjectOnPlayer(mLocalPlayer.mPlayer, obj))
            return true;
        if(testObjectOnPlayer(mRemotePlayer.mPlayer, obj))
            return true;
        if(testObjectOnPlayer(mAIPlayer.mPlayer, obj))
            return true;
        return false;
    }
    
} // namespace flower