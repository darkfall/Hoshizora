//
//  ObjectController.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "ObjectController.h"
#include "ThemeController.h"
#include "PlayerController.h"
#include "Common.h"

#include "../core/item.h"
#include "../core/bomb.h"

#include "config/SoraConfigParser.h"

namespace flower {
    
    ObjectController::ObjectController():
    mGravity(5.f) {
        
    }
    
    void ObjectController::throwObject(float x, float y, const sora::SoraVector& force, const std::string& which, ObjectType type) {
        ObjectConfigMap::const_iterator it = mObjectConfig.find(which);
        if(it != mObjectConfig.end()) {
            sora::SoraSprite* spr = ThemeController::Instance()->getSprite(it->second.Texture);
            if(spr) {
                Object* obj;
                if(type == OBJ_ITEM) {
                    obj = new Item(it->second.Weight,
                                          x,
                                          y,
                                          it->second.Score);
                } else {
                    obj = new Bomb(it->second.Weight,
                                   x,
                                   y,
                                   it->second.Score);
                }
                
                obj->setSprite(spr);
                obj->applyForce(force);
                
                mObjects.push_back(obj);
            }
        }
    }
    
    void ObjectController::update(float dt) {
        ObjectList::iterator it = mObjects.begin();
        ObjectList::iterator end = mObjects.end();
        
        for(; it != end; ++it) {
            Object* obj = (*it);
            
            if(obj->isDied()) {
                delete *it;
                it = mObjects.erase(it);
                continue;
            }
            
            obj->applyForce(sora::SoraVector(0.f, mGravity));
            obj->update(dt);
            
            if(!PositionCheck(obj->getPositionX(),
                              obj->getPositionY(), 
                              obj->getWidth(),
                              obj->getHeight())) {
                obj->die();
            } else {
                
                if(PlayerController::Instance()->testObject(obj))
                    obj->die();
                
            }
        }
    }
    
    void ObjectController::render() {
        ObjectList::const_iterator it = mObjects.begin();
        ObjectList::const_iterator end = mObjects.end();
        
        for(; it != end; ++it) {
            (*it)->render();
        }
    }
    
    bool ObjectController::loadConfig(const std::wstring& path) {
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
            if(parser.toNode("/objects")) {
                parser.toFirstChild();
                do {
                    ObjectConfig config;
                    
                    std::string tag = parser.getString("name");
                    config.Score = parser.getInt("score", 1);
                    config.Weight = parser.getFloat("weight", 1.f);
                    config.Texture = parser.getString("texture");
                    
                    if(!tag.empty() &&
                       !config.Texture.empty()) {
                        mObjectConfig.insert(std::make_pair(tag, config));
                    }
                    
                } while(parser.toNextChild());
    
                return true;
            }
        }
        return false;
    }
    
    void ObjectController::setGravity(float gravity) {
        mGravity = gravity;
    }
    
    float ObjectController::getGravity() const {
        return mGravity;
    }
    
} // namespace flower