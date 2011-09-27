//
//  item.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "item.h"

namespace flower {

    Item::Item(float weight, float x, float y, int32 score):
    Object(weight),
    mScore(score) {
        setPosition(x, y);
    }
    
    void Item::setScore(int32 score) {
        mScore = score;
    }
    
    int32 Item::getScore() const {
        return mScore;
    }
    
    void Item::die() {
        mIsDied = true;
    }
        
} // namespace sora
