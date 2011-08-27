//
//  SoraAutoUpdate.cpp
//  Sora
//
//  Created by Ruiwei Bu on 8/21/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraAutoUpdate.h"
#include "SoraIteratorHelper.h"

namespace sora {
    
    SoraAutoUpdate::UpdateObjectList SoraAutoUpdate::mObjects;
    
    void SoraAutoUpdate::updateList(float dt) {
        ConstVectorIterator<UpdateObjectList> iterator(mObjects);
        while(iterator.hasMoreElements()) {
            iterator.getNext()->onUpdate(dt);
        }
    }
    
} // namespace sora
