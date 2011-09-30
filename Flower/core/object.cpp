//
//  object.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/26/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "object.h"
#include "../controller/common.h"

namespace flower {

    float Object::mSpeedThreshold;
    
    Object::Object(float weight, int score):
    mWeight(weight),
    mRotation(0.f),
    mIsDied(false),
    mScore(score),
    mSpeed(sora::SoraVector(0.f, 0.f)) {
        
    }
    
    int32 Object::update(float dt) {
        setPosition(getPositionX()+mSpeed.x * dt, getPositionY()+mSpeed.y * dt);
        return 0;
    }
    
    void Object::render() {
        if(mTexture) {
            mTexture->setRotation(mRotation);
            mTexture->render(getPositionX(), getPositionY());
        }
    }
    
    void Object::applyForce(const sora::SoraVector& force) {
        sora::SoraVector a = force / mWeight;
        mSpeed += a;
        
        enclose(mSpeed.x, mSpeedThreshold);
        enclose(mSpeed.y, mSpeedThreshold);
    }
    
    void Object::setSprite(sora::SoraSprite* sprite) {
        sora_assert(sprite);
        
        mTexture = sprite;
        mTexture->setCenter(mTexture->getTextureWidth()/2, mTexture->getTextureHeight()/2);
    }
    
    float Object::getWidth() const {
        if(mTexture) 
            return mTexture->getTextureWidth();
        return 0.f;
    }
    
    void Object::setScore(int32 score) {
        mScore = score;
    }
    
    int Object::getScore() const {
        return mScore;
    }
    
    float Object::getHeight() const {
        if(mTexture)
            return mTexture->getTextureHeight();
        return 0.f;
    }

    bool Object::isDied() const {
        return mIsDied;
    }
    
    void Object::setSpeedThreshold(float threshold) {
        mSpeedThreshold = threshold;
    }
    
    float Object::getSpeedThreshold() {
        return mSpeedThreshold;
    }
    
} // namespace flower