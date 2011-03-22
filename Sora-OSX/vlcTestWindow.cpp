//
//  vlcTestWindow.cpp
//  Sora
//
//  Created by Griffin Bu on 3/21/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "vlcTestWindow.h"


#include "SoraFont.h"

static sora::SoraSprite* pSpr;
static sora::SoraTexture* pTex;
static sora::SoraFont* pFont;


bool vlcWindow::updateFunc() {
    return false;
}

bool vlcWindow::renderFunc() {
    sora::SORA->beginScene();
    
    pFont->print(0.f, getWindowHeight()-50.f, sora::FONT_ALIGNMENT_LEFT, L"SoraCoreRenderFPS: %f, VideoFPS: %f", sora::SORA->getFPS(), moviePlayer->getFPS());
    pFont->print(0.f, getWindowHeight()-30.f, sora::FONT_ALIGNMENT_LEFT, L"FrameCount: %d", moviePlayer->getFrameCount());
    pFont->print(0.f, getWindowHeight()-70.f, sora::FONT_ALIGNMENT_LEFT, L"Time: %llu, Lenght: %llu", 
                   moviePlayer->getTime(),
                   moviePlayer->getLength());
        
    pFont->print(0.f, getWindowHeight()-90.f, sora::FONT_ALIGNMENT_LEFT, L"Width: %u, Height: %u", 
                 moviePlayer->getWidth(),
                 moviePlayer->getHeight());
    pFont->print(0.f, getWindowHeight()-110.f, sora::FONT_ALIGNMENT_LEFT, L"AMV_Scenario.mp4");

    
  /// if(moviePlayer->frameChanged()) {
        if(pTex) {
            sora::SORA->releaseTexture((sora::HSORATEXTURE)pTex);
        }
        if(pSpr) {
            delete pSpr;
            pSpr = 0;
        }

        pTex = (sora::SoraTexture*)sora::SORA->createTextureFromRawData((uint32*)moviePlayer->getPixelData(), 
                                                                        moviePlayer->getWidth(), 
                                                                        moviePlayer->getHeight());
        if(pTex) {
            pSpr = new sora::SoraSprite(pTex);
        }
        pSpr->setCenter(pSpr->getSpriteWidth()/2, pSpr->getSpriteHeight()/2);
        pSpr->render(getWindowWidth()/2, getWindowHeight()/2-30.f);
  //  } else {
    //    if(pSpr) {
      //      pSpr->setCenter(pSpr->getSpriteWidth()/2, pSpr->getSpriteHeight()/2);
        //   pSpr->render(getWindowWidth()/2, getWindowHeight()/2);
       // }
   // }
    sora::SORA->endScene();
   
    return false;
}

void vlcWindow::init() {
    sora::SORA->setFPS(999);
    
    moviePlayer = new sora::SoraVlcMoviePlayer();
    moviePlayer->openMedia(L"./AMV_Scenario.mp4", 848, 480);
    moviePlayer->play();

    
    pFont = sora::SORA->createFont(L"cour.ttf", 24);
    pFont->setColor(0xFFFFFFFF);
    
    pSpr = 0;
}