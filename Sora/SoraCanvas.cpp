/*
 *  SoraCanvas.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraCanvas.h"

namespace sora {
        
    SoraBaseCanvas::SoraBaseCanvas(int32 w, int32 h, bool bDepthBuffer) {
        canvasTarget = SORA->createTarget(w, h, bDepthBuffer);
        pCanvasSprite = new SoraSprite(SORA->getTargetTexture(canvasTarget));
        if(!canvasTarget || !pCanvasSprite) 
            throw SORA_EXCEPTION("Error creating render target");
    }
    
    SoraBaseCanvas::~SoraBaseCanvas() {
        SORA->freeTarget(canvasTarget);
        delete pCanvasSprite;
    }
    
    void SoraBaseCanvas::render() {
        pCanvasSprite->setTexture(SORA->getTargetTexture(canvasTarget));
        pCanvasSprite->render(getPositionX(), getPositionY());
    }
    
    uint32 SoraBaseCanvas::update(float32 dt) {
        pCanvasSprite->update(dt);
		return 0;
    }
    
    void SoraBaseCanvas::beginRender() {
        bSceneBegan = true;
        SORA->beginScene(0xFFFFFF00, canvasTarget);
    }
    
    void SoraBaseCanvas::finishRender() {
        if(!bSceneBegan)
            throw SORA_EXCEPTION("Render not began");
        SORA->endScene();
    }
    
	void SoraBaseCanvas::attachShader(SoraShader* shader) {
		pCanvasSprite->attachShader(shader);
	}
	void SoraBaseCanvas::detachShader(SoraShader* shader) {
		pCanvasSprite->detachShader(shader);
	}
	
	SoraShader* SoraBaseCanvas::attachShader(const SoraWString& shaderPath, const SoraString& entry, SORA_SHADER_TYPE type) {
		return pCanvasSprite->attachShader(shaderPath, entry, type);
	}
	
	bool SoraBaseCanvas::hasShader() const {
		return pCanvasSprite->hasShader();
	}
	
	void SoraBaseCanvas::clearShader() {
		return pCanvasSprite->clearShader();
	}
	
	void SoraBaseCanvas::addEffect(SoraImageEffect* effect) {
		return pCanvasSprite->addEffect(effect);
	}
	
	void SoraBaseCanvas::stopEffect(SoraImageEffect* effect) {
		return pCanvasSprite->stopEffect(effect);
	}
	
	void SoraBaseCanvas::clearEffects() {
		return pCanvasSprite->clearEffects();
	}
	
	bool SoraBaseCanvas::hasEffect() const {
		return pCanvasSprite->hasEffect();
	}
    
} // namespace sora