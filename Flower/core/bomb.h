//
//  bomb.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_bomb_h
#define Sora_iPhone_bomb_h

#include "SoraPlatform.h"

#include "object.h"

namespace flower {

    class Bomb: public Object {
    public:
        Bomb(float weight, float x, float y, int32 score);
        
        void onPlayerCollision(Player* player);
        
        virtual void die();
        
    private:
    };
    

} // namespace flower

#endif
