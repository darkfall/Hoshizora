//
//  SoraAutoUpdate.h
//  Sora
//
//  Created by Ruiwei Bu on 8/21/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef Sora_SoraAutoUpdate_h
#define Sora_SoraAutoUpdate_h

/**
 * Classes that will be updated each frame
 **/
#include <list>

namespace sora {

    class SoraAutoUpdate {
    public:
        SoraAutoUpdate() {
            mObjects.push_back(this);
        }
        
        virtual ~SoraAutoUpdate() {
            mObjects.remove(this);
        }
        
        virtual void onUpdate(float dt) = 0;
        
        static void updateList(float dt);
        
    private:
        typedef std::list<SoraAutoUpdate*> UpdateObjectList;
        static UpdateObjectList mObjects;
    };

} // namespace sora



#endif
