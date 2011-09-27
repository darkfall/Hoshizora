//
//  ItemController.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "ItemController.h"
#include "../core/item.h"

#include "config/SoraConfigParser.h"
#include "ThemeController.h"
#include "Common.h"

namespace flower {
    
    ItemController::ItemController():
    mGravity(5.f) {
        
    }
    
    void ItemController::throwItem(float x, float y, const sora::SoraVector& force, const std::string& which) {
        ItemConfigMap::const_iterator it = mItemConfig.find(which);
        if(it != mItemConfig.end()) {
            sora::SoraSprite* spr = ThemeController::Instance()->getSprite(it->second.Texture);
            if(spr) {
                Item* item = new Item(it->second.Weight,
                                      x,
                                      y,
                                      it->second.Score);
                item->setSprite(spr);
                item->applyForce(force);
                
                mItems.push_back(item);
            }
        }
    }
    
    void ItemController::update(float dt) {
        ItemList::iterator it = mItems.begin();
        ItemList::iterator end = mItems.end();
        
        for(; it != end; ++it) {
            Item* item = (*it);
            
            if(item->isDied()) {
                delete *it;
                it = mItems.erase(it);
                continue;
            }
            
            item->applyForce(sora::SoraVector(0.f, mGravity));
            item->update(dt);
            
            if(!PositionCheck(item->getPositionX(),
                              item->getPositionY(), 
                              item->getWidth(),
                              item->getHeight())) {
                item->die();
            }
        }
    }
    
    void ItemController::render() {
        ItemList::const_iterator it = mItems.begin();
        ItemList::const_iterator end = mItems.end();
        
        for(; it != end; ++it) {
            (*it)->render();
        }
    }
    
    bool ItemController::loadConfig(const std::wstring& path) {
        sora::SoraConfigParser parser;
        if(parser.open(path)) {
            if(parser.toNode("/speedthreshold")) {
                Object::setSpeedThreshold(parser.getFloat("value", 10.f));
            } else {
                Object::setSpeedThreshold(10.f);
            }
            if(parser.toNode("/gravity")) {
                mGravity = parser.getFloat("value", 1.f);
            } else {
                mGravity = 1.f;
            }
            if(parser.toNode("/items")) {
                parser.toFirstChild();
                do {
                    ItemConfig config;
                    
                    std::string tag = parser.getString("name");
                    config.Score = parser.getInt("score", 1);
                    config.Weight = parser.getFloat("weight", 1.f);
                    config.Texture = parser.getString("texture");
                    
                    if(!tag.empty() &&
                       !config.Texture.empty()) {
                        mItemConfig.insert(std::make_pair(tag, config));
                    }
                    
                } while(parser.toNextChild());
    
                return true;
            }
        }
        return false;
    }
    
    void ItemController::setGravity(float gravity) {
        mGravity = gravity;
    }
    
    float ItemController::getGravity() const {
        return mGravity;
    }
    
} // namespace flower