//
//  vlcTestWindow.cpp
//  Sora
//
//  Created by Griffin Bu on 3/21/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "vlcTestWindow.h"


#include "SoraFont.h"

static MP_CTX ctx;

static sora::SoraSprite* pSpr;
static sora::SoraTexture* pTex;
static sora::SoraFont* pFont;


bool changed = false;

void* lock(void* data, void** pixels) {
    MP_CTX* ctx = (MP_CTX*)data;
    *pixels = ctx->pixels;
    
    ++ctx->frameCount;
    
    
    return 0;
}

void unlock(void* data, void* id, void* const* pixels) {
    changed = true;
}

void display(void* data, void* id) {
    (void) data;
}

bool vlcWindow::updateFunc() {
    return false;
}

bool vlcWindow::renderFunc() {
    sora::SORA->beginScene();
    
    pFont->print(0.f, getWindowHeight()-50.f, sora::FONT_ALIGNMENT_LEFT, L"Framerate: %f", sora::SORA->getFPS());
    pFont->print(0.f, getWindowHeight()-30.f, sora::FONT_ALIGNMENT_LEFT, L"FrameCount: %d", ctx.frameCount);
    
    
    if(changed) {
        if(pTex) {
            sora::SORA->releaseTexture((sora::HSORATEXTURE)pTex);
        }
        if(pSpr) {
            delete pSpr;
            pSpr = 0;
        }

        pTex = (sora::SoraTexture*)sora::SORA->createTextureFromRawData((uint32*)ctx.pixels, 848, 480);
        if(pTex) {
            pSpr = new sora::SoraSprite(pTex);
        }
        pSpr->setCenter(pSpr->getSpriteWidth()/2, pSpr->getSpriteHeight()/2);
        pSpr->render(getWindowWidth()/2, getWindowHeight()/2);
    
        changed = false;
    } else {
        if(pSpr) {
            pSpr->setCenter(pSpr->getSpriteWidth()/2, pSpr->getSpriteHeight()/2);
            pSpr->render(getWindowWidth()/2, getWindowHeight()/2);
        }
    }
    sora::SORA->endScene();
   
    return false;
}

void vlcWindow::init() {
    
    const char* vlc_argv[] = {
        "--plugin-path=./Plugins"
        "--ignore-config",
        "--vout", "vmem",
    };
    int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);
    
    sora::SORA->setFPS(24);
    
    ctx.pixels = (void*)malloc(848*480*4);
    ctx.frameCount = 0;
    
    vlcInstance = libvlc_new(vlc_argc, vlc_argv);
    media = libvlc_media_new_path(vlcInstance, "AMV_Scenario.mp4");
    mp = libvlc_media_player_new_from_media(media);
    
    //libvlc_media_release(media);
    
    libvlc_video_set_callbacks(mp, lock, unlock, display, &ctx);
    libvlc_video_set_format(mp, "RGBA", 848, 480, 848*4);
    libvlc_media_player_play(mp);
    
    pFont = sora::SORA->createFont(L"cour.ttf", 20);
    pFont->setColor(0xFFFFFFFF);
    
    pSpr = 0;
}