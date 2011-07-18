//
//  SoraLayer.cpp
//  Sora
//
//  Created by Ruiwei Bu on 7/18/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraLayer.h"

namespace sora {
    
    SoraLayer::SoraLayer(int32 layerDepth):
    mLayerDepth(layerDepth) {
        
    }
    
    SoraLayer::~SoraLayer() {
        
    }
    
    SoraLayer& SoraLayer::combine(const SoraLayer& layer) {
        SoraObject* obj = layer.getObjList();
        while(obj != NULL) {
            SoraObject::add(obj);
            obj = obj->next();
        }
    }
    
    SoraLayer& SoraLayer::removeDuplicate(const SoraLayer& layer) {
        SoraObject* obj = layer.getObjList();
        while(obj != NULL) {
            SoraObject::del(obj);
            obj = obj->next();
        }
    }
    
    void SoraLayer::setLayerDepth(int32 depth) {
        mLayerDepth = depth;
    }
    
    void SoraLayer::swap(SoraLayer& layer) {
        SoraObject* objA = getObjList();
        SoraObject* objB = layer.getObjList();
        
        delAll();
        layer.delAll();
        
        while(objA != NULL) {
            layer.add(objA);
            objA = objA->next();
        }
        
        while(objB != NULL) {
            SoraObject::add(objB);
            objB = objB->next();
        }
    }
    
    SoraLayer* SoraLayer::clone() {
        SoraLayer* layer = new SoraLayer(mLayerDepth);
        SoraObject* obj = getObjList();
        while(obj != NULL) {
            layer->add(obj);
            obj = obj->next();
        }
        return layer;
    }
    
} // namespace sora
