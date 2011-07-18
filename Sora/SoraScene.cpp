//
//  SoraScene.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/17/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraScene.h"
#include "SoraCore.h"

namespace sora {
    
    SoraScene::SoraScene(int32 width, int32 height):
    mRealWidth(0.f),
    mRealHeight(0.f),
    mRotation(0.f),
    mHScale(1.f),
    mVScale(1.f),
    mParentScene(NULL) {
        adjustSize(width, height);
    }
    
    SoraScene::~SoraScene() {
        LayerMap::iterator itLayer = mLayers.begin();
        while(itLayer != mLayers.end()) {
            delete itLayer->second;
            ++itLayer;
        }
        mLayers.clear();
    }
    
    float32 SoraScene::getWidth() const {
        return mWidth;
    }
    
    float32 SoraScene::getHeight() const {
        return mHeight;
    }

    void SoraScene::adjustSize(int32 width, int32 height) {
        if(width == 0 || height == 0)
            THROW_SORA_EXCEPTION("Attempt to create a empty scene with width or height = 0");
        
        mWidth = mRealWidth = width;
        mHeight = mRealHeight = height;
    }
     
    void SoraScene::add(SoraObject* obj) {
        SoraScene* subScene = dynamic_cast<SoraScene*>(obj);
        if(subScene == NULL)
            add(obj, 0);
        else
            SoraObject::add(subScene);
    }

    
    void SoraScene::setParent(SoraObject* obj) {
        if(obj == NULL) {
            mParentScene = NULL;
            adjustSize(mRealWidth, mRealHeight);
        }
        SoraObject::setParent(obj);
        
        SoraScene* parentScene = dynamic_cast<SoraScene*>(obj);
        if(parentScene != NULL) {
            mParentScene = parentScene;
            int32 maxRight = (int32)mParent->getPositionX() + mParentScene->getWidth();
            int32 maxBottom = (int32)mParent->getPositionY() + mParentScene->getHeight();
            if(getPositionX() + mWidth > maxRight)
                mWidth = (getPositionX() + mWidth) - maxRight;
            if(getPositionY() + mHeight > maxBottom)
                mHeight = (getPositionY() + mHeight) - maxBottom;
        }
    }
    
    void SoraScene::add(SoraObject* obj, int32 layerDepth) {
        LayerMap::iterator itLayer = mLayers.find(layerDepth);
        if(itLayer != mLayers.end()) {
            SoraLayer* layer = itLayer->second;
            if(layer->getLayerDepth() == layerDepth) {
                layer->add(obj);
            }
        } else {
            SoraLayer* layer = addLayer(layerDepth);
            if(layer)
                layer->add(obj);
        }
    }
    
    SoraLayer* SoraScene::addLayer(int32 layerDepth) {
        SoraLayer* layer = new SoraLayer(layerDepth);
        if(!layer)
            THROW_SORA_EXCEPTION("Error creating new layer");
        else {
            mLayers.insert(std::make_pair(layerDepth, layer));
            layer->setPosition(getPositionX(), getPositionY());
        }
        return layer;
    }
    
    void SoraScene::removeLayer(int32 layerDepth) {
        LayerMap::iterator itLayer = mLayers.find(layerDepth);
        if(itLayer != mLayers.end()) {
            SoraLayer* layer = itLayer->second;
            delete layer;
            mLayers.erase(itLayer);
        }
    }
    
    void SoraScene::setLayerDepth(int32 layerDepth, int32 newDepth) {
        LayerMap::iterator itLayer = mLayers.find(layerDepth);
        if(itLayer != mLayers.end()) {
            mLayers.insert(std::make_pair(newDepth, itLayer->second));
            itLayer->second->setLayerDepth(newDepth);
            mLayers.erase(itLayer);
        }
    }
    
    void SoraScene::combineLayer(int32 layer1, int32 layer2) {
        LayerMap::iterator itLayerA = mLayers.find(layer1);
        LayerMap::iterator itLayerB = mLayers.find(layer2);
        if(itLayerA != mLayers.end() &&
           itLayerB != mLayers.end()) {
            SoraLayer* layerA = itLayerA->second;
            SoraLayer* layerB = itLayerB->second;
            
            mLayers.erase(itLayerA);
            mLayers.erase(itLayerB);
            
            mLayers.insert(std::make_pair(layerA->getLayerDepth(), layerA));
            mLayers.insert(std::make_pair(layerB->getLayerDepth(), layerB));
        } else {
            DebugPtr->error(vamssg("Error combining layer %d and %d", layer1, layer2));
        }
    }
    
    SoraLayer* SoraScene::getLayer(int32 layerDepth) const {
        LayerMap::const_iterator itLayer = mLayers.find(layerDepth);
        if(itLayer != mLayers.end()) {
            return itLayer->second;
        }
        return NULL;
    }
    
    SoraLayer* SoraScene::operator[](int32 layerDepth) const {
        LayerMap::const_iterator itLayer = mLayers.find(layerDepth);
        if(itLayer != mLayers.end()) {
            return itLayer->second;
        }
        return NULL;
    }
    
    void SoraScene::render() {
        sora::SORA->pushTransformMatrix();
        sora::SORA->setTransform(0.f, 
                                 0.f, 
                                 getPositionX(), 
                                 getPositionY(),
                                 mRotation,
                                 mHScale,
                                 mVScale);
        sora::SORA->pushClippingMatrix();
        sora::SORA->setClipping(getPositionX(), getPositionY(), mWidth, mHeight);
        
        LayerMap::iterator itLayer = mLayers.begin();
        while(itLayer != mLayers.end()) {
            itLayer->second->render();
            ++itLayer;
        }
        
        /**
         *  Render SubScenes(If Available)
         **/
        SoraObject::render();
        
        sora::SORA->popTransformMatrix();
        sora::SORA->popClippingMatrix();
    }
    
    uint32 SoraScene::update(float32 dt) {
        LayerMap::iterator itLayer = mLayers.begin();
        while(itLayer != mLayers.end()) {
            itLayer->second->update(dt);
        }
        
        /**
         *  Update SubScenes(If Available)
         **/
        SoraObject::update(dt);
        return 0;
    }
    
    void SoraScene::setRotation(float32 rot) {
        mRotation = rot;
    }
    
    float32 SoraScene::getRotation() const {
        return mRotation;
    }
    
    void SoraScene::setScale(float32 scaleh, float32 scalev) {
        mHScale = scaleh;
        mVScale = scalev;
    }
    
    void SoraScene::getScale(float32* scaleh, float32* scalev) {
        *scaleh = mHScale;
        *scalev = mVScale;
    }
} // namespace sora