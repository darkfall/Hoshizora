/*
 *  SoraCanvas.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraCanvas.h"
#include "SoraCore.h"
#include "SoraModifierAdapter.h"
#include "SoraLogger.h"

namespace sora {
        
    SoraBaseCanvas::SoraBaseCanvas(int32 w, int32 h, bool bDepthBuffer) {
        canvasTarget = SORA->createTarget(w, h, bDepthBuffer);
		if(!canvasTarget) {
			log_mssg("SoraBaseCanvas: Error creating render target");
            return;
        }
        pCanvasSprite = new SoraSprite(0);
		  
		if(!canvasTarget || !pCanvasSprite) {
            log_mssg("SoraBaseCanvas: Error creating render target");
            return;
        }
    }
    
    SoraBaseCanvas::~SoraBaseCanvas() {
        if(canvasTarget)
            SORA->freeTarget(canvasTarget);
        if(pCanvasSprite)
            delete pCanvasSprite;
    }

	void SoraBaseCanvas::setZ(float z) {
        if(pCanvasSprite)
            pCanvasSprite->setZ(z);
	}

	void SoraBaseCanvas::setBlendMode(int32 mode) {
        if(pCanvasSprite)
            pCanvasSprite->setBlendMode(mode);
	}
    
    void SoraBaseCanvas::render() {
        if(pCanvasSprite) {
            pCanvasSprite->setTexture(SORA->getTargetTexture(canvasTarget));
            pCanvasSprite->render(getPositionX(), getPositionY());
        }
    }
    
    int32 SoraBaseCanvas::update(float dt) {
        if(pCanvasSprite)
            pCanvasSprite->update(dt);
		return 0;
    }
    
    
    SoraSprite* SoraBaseCanvas::getCanvasSprite() const { 
        if(pCanvasSprite)
            pCanvasSprite->setTexture(SORA->getTargetTexture(canvasTarget));
        return pCanvasSprite;
    }
    
    void SoraBaseCanvas::beginRender(ulong32 col) {
        if(canvasTarget) {
            bSceneBegan = true;
            SORA->beginScene(col, canvasTarget);
        }
    }
    
    void SoraBaseCanvas::finishRender() {
        if(canvasTarget) {
            if(!bSceneBegan)
                THROW_SORA_EXCEPTION(IllegalStateException, "Render not began");
            SORA->endScene();
        }
    }
    	
	void SoraBaseCanvas::addEffect(SoraImageEffect* effect) {
        if(pCanvasSprite)
            return pCanvasSprite->addEffect(effect);
	}
	
	void SoraBaseCanvas::stopEffect(SoraImageEffect* effect) {
        if(pCanvasSprite)
            return pCanvasSprite->stopEffect(effect);
	}
	
	void SoraBaseCanvas::clearEffects() {
        if(pCanvasSprite)
            return pCanvasSprite->clearEffects();
	}
	
	bool SoraBaseCanvas::hasEffect() const {
        if(pCanvasSprite)
            return pCanvasSprite->hasEffect();
        return false;
	}
    
    void SoraBaseCanvas::fadeTo(float to, float t) {
        if(pCanvasSprite)
            pCanvasSprite->addModifierAdapter(CreateModifierAdapter(pCanvasSprite, 
                                                                    CreateEffectFade(CGETA(pCanvasSprite->getColor())/255.f,
                                                                                        to,
                                                                                        t)));
    }
    
    void SoraBaseCanvas::rotateTo(float to, float t) {
        if(pCanvasSprite)
            pCanvasSprite->addModifierAdapter(CreateModifierAdapter(pCanvasSprite, 
                                                                    CreateEffectRotation(pCanvasSprite->getRotation(),
                                                                                            to,
                                                                                            t)));
    }
    
    void SoraBaseCanvas::scaleTo(float h, float v, float t) {
        if(pCanvasSprite)
            pCanvasSprite->addModifierAdapter(CreateModifierAdapter(pCanvasSprite, 
                                                                    CreateEffectScale(pCanvasSprite->getHScale(),
                                                                                        pCanvasSprite->getVScale(),
                                                                                        h,
                                                                                        v,
                                                                                        t)));
    }
    
    void SoraBaseCanvas::fadeToAndNotify(float to, float t, const NotificationFunc& func) {
        if(pCanvasSprite)
            pCanvasSprite->addModifierAdapter(CreateModifierAdapterWithNotification(pCanvasSprite, 
                                                                                    CreateEffectFade(CGETA(pCanvasSprite->getColor()),
                                                                                                        to,
                                                                                                        t),
                                                                                    func));
    }
    
    void SoraBaseCanvas::rotateTo(float to, float t, const NotificationFunc& func) {
        if(pCanvasSprite)
            pCanvasSprite->addModifierAdapter(CreateModifierAdapterWithNotification(pCanvasSprite, 
                                                                                    CreateEffectRotation(pCanvasSprite->getRotation(),
                                                                                                            to,
                                                                                                            t),
                                                                                    func));
    }
    
    void SoraBaseCanvas::scaleTo(float h, float v, float t, const NotificationFunc& func) {
        if(pCanvasSprite)
            pCanvasSprite->addModifierAdapter(CreateModifierAdapterWithNotification(pCanvasSprite, 
                                                                                    CreateEffectScale(pCanvasSprite->getHScale(),
                                                                                                        pCanvasSprite->getVScale(),
                                                                                                        h,
                                                                                                        v,
                                                                                                        t),
                                                                                    func));
    }
    
} // namespace sora