//
//  player.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "player.h"

#include "../controller/PlayerController.h"
#include "../controller/common.h"

#include "SoraCore.h"

namespace flower {
    
    float Player::mMaxSpeed;
    PlayerController* Player::mPlayerController;
    
    Player::Player() {
        
    }
    
    Player::~Player() {
        if(mTexture)
            delete mTexture;
    }
    
    void Player::setMaxSpeed(float maxSpeed) {
        mMaxSpeed = maxSpeed;
    }
    
    float Player::getMaxSpeed() {
        return mMaxSpeed;
    }
    
    void Player::setPlayerController(PlayerController* controller) {
        mPlayerController = controller;
    }
 
    void Player::setSprite(sora::SoraSprite* spr) {
        sora_assert(spr);
        mTexture = spr;
        mTexture->setCenter(mTexture->getTextureWidth()/2, 
                            mTexture->getTextureHeight()/2);
    }
    
    uint32 Player::update(float dt) {
        if(mTexture)
            mTexture->update(dt);
        
        if(mIsMoving) {
            float dest = getPositionX() + mMoveSpeed * dt;
            
            
            bool flag = enclose(dest,
                                mTexture->getTextureWidth()/2,
                                sora::SoraCore::Instance()->getScreenWidth()-mTexture->getTextureWidth()/2
                                );
            
            setPosition(dest,
                        getPositionY());
            
            if(flag) {
                mIsMoving = false;
                mToX = dest;
            } else  {
                if(abs(mToX - getPositionX()) <= 0.0001f) {
                    mIsMoving = false;
                }
            }
        }
        return 0;
    }
    
    void Player::moveTo(float x, float y, float speed) {
        mIsMoving = true;
        mMoveSpeed = speed;
        mToX = x;
        mToY = y;
    }
    
    void Player::render() {
        if(mTexture) {
            mTexture->render(getPositionX(),
                             getPositionY());
        }
    }
    
    void Player::die() {
        sora_assert(mPlayerController);
        
        mIsDied = true;
    }
    
    
} // namespace sora