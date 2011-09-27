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
#include <list>
#include <map>

namespace flower {
    
    class Item;
    
    class ItemController: public sora::SoraSingleton<ItemController> {
    public:
        friend class sora::SoraSingleton<ItemController>;
        
        ItemController();
        
        void throwItem(float x, float y, const sora::SoraVector& force, const std::string& which);
        
        void update(float dt);
        void render();
        
        bool loadConfig(const std::wstring& path);
        
        void clear();
        
        void setGravity(float gravity);
        float getGravity() const;
        
    private:
        typedef std::list<Item*> ItemList;
        ItemList mItems;
        
        struct ItemConfig {
            int32 Score;
            float Weight;
            std::string Texture;
        };
        
        typedef std::map<std::string, ItemConfig> ItemConfigMap;
        ItemConfigMap mItemConfig;
        
        float mGravity;
    };
    
} // namespace flower

#endif
