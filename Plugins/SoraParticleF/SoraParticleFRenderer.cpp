//
//  SoraParticleFRenderer.cpp
//  SoraF
//
//  Created by Robert Bu on 2/26/12.
//  Copyright (c) 2012 Game Master Studio. All rights reserved.
//

#include "SoraParticleFRenderer.h"
#include "SoraForEach.h"
#include "SoraCore.h"

namespace sora {
    
    SoraParticleFRenderer::SoraParticleFRenderer() {
        
    }
    
    SoraParticleFRenderer::~SoraParticleFRenderer() {
        
    }
    
    const SoraString& SoraParticleFRenderer::getName() const {
        static SoraString name = "default";
        return name;
    }
    
    SoraTextureHandle SoraParticleFRenderer::getTexture() const {
        return mSprite.getTexture();
    }
    
    void SoraParticleFRenderer::setTeture(SoraTextureHandle texture) {
        mSprite.setTexture(texture);
        mSprite.setCenter(mSprite.getTextureWidth() / 2,
                          mSprite.getTextureHeight() / 2);
        mSprite.enable3D(true);
    }
    
    SoraRect SoraParticleFRenderer::getTextureRect() const {
        return mSprite.getTextureRect();
    }
    
    void SoraParticleFRenderer::setTextureRect(const SoraRect& rect) {
        mSprite.setTextureRect(rect.x1, 
                               rect.y1,
                               rect.x2,
                               rect.y2);
        mSprite.setCenter(rect.getWidth() / 2, 
                          rect.getHeight() / 2);
    }
    
    void SoraParticleFRenderer::doRender(const std::vector<SoraParticleFNode>& particles) {
        float dt = SoraCore::Ptr->getDelta();
        
        SORA_FOR_EACH(SoraParticleFNode& node, particles) {
            node.update(dt);
            
            mSprite.setTransform(node.transform);
            mSprite.setColor(node.color.getHWColor());
            mSprite.render();
        }
    }
}
