//
//  SoraPostEffect.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraPostEffect.h"
#include "SoraSprite.h"
#include "SoraCore.h"
#include "function/SoraBind.h"

namespace sora {
    
    SoraPostEffect::SoraPostEffect():
    mSprite(0) {
        mSprite = new SoraSprite(0);
        sora_assert(mSprite);
        
        attach();
        
        sora::SoraCore::Instance()->enableCoreFeature(SoraCore::FeatureRenderToBuffer);
    }
    
    SoraPostEffect::~SoraPostEffect() {
        if(mSprite)
            delete mSprite;
        sora::SoraCore::Instance()->unregisterFullscreenBufferDelegate();
    }
    
    void SoraPostEffect::attach() {
        sora::SoraCore::Instance()->registerFullscreenBufferDelegate(Bind(this, &SoraPostEffect::onBufferRender));
    }
    
    void SoraPostEffect::onBufferRender(SoraTextureHandle texture) {
        if(mSprite->getTexture() == 0)
            mSprite->setTexture(texture);
        
        mSprite->render(getPositionX(), getPositionY());
    }
    
    SoraShader* SoraPostEffect::attachFragmentShader(const StringType& file, const SoraString& entry) {
        return mSprite->attachFragmentShader(file, entry);
    }
    
    SoraShader* SoraPostEffect::attachVertexShader(const StringType& file, const SoraString& entry) {
        return mSprite->attachVertexShader(file, entry);
    }
    
    void SoraPostEffect::detachFragmentShader() {
        mSprite->detachFragmentShader();
    }
    
    void SoraPostEffect::detachVertexShader() {
        mSprite->detachVertexShader();
    }
    
    void SoraPostEffect::attachFragmentShader(SoraShader* shader) {
        mSprite->attachFragmentShader(shader);
    }
    
    void SoraPostEffect::attachVertexShader(SoraShader* shader) {
        mSprite->attachVertexShader(shader);
    }
    
    SoraSprite* SoraPostEffect::getSprite() const {
        return mSprite;
    }
    
} // namespace sora
