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
    
    void SoraBaseCanvas::setFont(SoraFont* font) {
        pFont = font;
    }
        
    void SoraBaseCanvas::renderString(const SoraString& str, float32 x, float32 y) {
        if(!pFont) 
            SORA->log("SoraBaseCanvas: no font available");
        else 
            pFont->render(x, y, s2ws(str).c_str());
    }
    
    void SoraBaseCanvas::renderString(const SoraWString& str, float32 x, float32 y) {
        if(!pFont) 
            SORA->log("SoraBaseCanvas: no font available");
        else 
            pFont->render(x, y, str.c_str());
    }
    
    void SoraBaseCanvas::renderSprite(SoraSprite* pSpr, float32 x, float32 y) {
        if(x != 0.f || y != 0.f)
            pSpr->render(x, y);
        else 
            pSpr->render();
    }
    
    
    
} // namespace sora