//
//  player.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_player_h
#define Sora_iPhone_player_h

#include "SoraSprite.h"
#include "SoraObject.h"
#include "SoraRect.h"
#include "../controller/defines.h"

namespace flower {
    
    class PlayerController;
    
    class Player: public sora::SoraObject {
    public:
        Player();
        ~Player();
        
        static void setMaxSpeed(float maxSpeed);
        static float getMaxSpeed();
        
        static void setPlayerController(PlayerController* controller);
        
        void setSprite(sora::SoraSprite* spr);
        
        virtual uint32 update(float dt);
        virtual void render();
        
        void moveTo(float x, float y, float speed);
        
        void setCollisionRect(const sora::SoraRect& rect);
        
        void die();
        
    protected:
        static float mMaxSpeed;
        static PlayerController* mPlayerController;
                
        sora::SoraRect mCollisionRect;
        sora::SoraSprite* mTexture;
        
        float mToX, mToY;
        float mMoveSpeed;
        bool mIsMoving;
        bool mIsDied;
    };
    
    
} // namespace flower  


#endif
