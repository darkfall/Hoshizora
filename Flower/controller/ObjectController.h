//
//  ItemController.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_ItemController_h
#define Sora_iPhone_ItemController_h

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "SoraVector.h"

#include "defines.h"

#include <list>
#include <map>

namespace flower {
    
    class Object;
    
    class ObjectController: public sora::SoraSingleton<ObjectController> {
    public:
        friend class sora::SoraSingleton<ObjectController>;
        
        ObjectController();
        
        void throwObject(float x, float y, const sora::SoraVector& force, const std::string& which, ObjectType type);
        
        void update(float dt);
        void render();
        
        bool loadConfig(const std::string& path);
        
        void clear();
        
        void setGravity(float gravity);
        float getGravity() const;
        
    private:
        typedef std::list<Object*> ObjectList;
        ObjectList mObjects;
        
        struct ObjectConfig {
            int32 Score;
            float Weight;
            std::string Texture;
        };
        
        typedef std::map<std::string, ObjectConfig> ObjectConfigMap;
        ObjectConfigMap mObjectConfig;
        
        float mGravity;
    };
    
} // namespace flower

#endif
