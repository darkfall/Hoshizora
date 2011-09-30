//
//  item.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_item_h
#define Sora_iPhone_item_h

#include "SoraPlatform.h"
#include "object.h"

namespace flower {

    class Item: public Object {
    public:
        Item(float weight, float x, float y, int32 score);
        
        void onPlayerCollision(Player* player);
        
        virtual void die();
        
    private:
    };
    
    
} // namespace sora


#endif
