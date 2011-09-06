//
//  SoraMainWindow.cpp
//  Hoshizora
//
//  Created by Ruiwei Bu on 7/16/11.
//  Copyright 2011 Robert Bu. All rights reserved.
//

#include "SoraMainWindow.h"
#include "SoraCore.h"
#include "SoraMath.h"

#include "SoraModifierAdapter.h"
#include "modifiers/SoraFontModifiers.h"

#include "SoraDelegate.h"
#include "SoraFunction.h"
#include "SoraShaderManager.h"

bool MainWindow::updateFunc() {
    mSprite->update(sora::SORA->getDelta());
    
    return false;
}

void MainWindow::onImageEffectEnd(const void* sender, int32& reserved) {
    sora::SORA->messageBox("Image Effect ended", "Notifcication", MB_OK | MB_ICONINFORMATION);
}

bool MainWindow::renderFunc() {
    sora::SoraCore* inst = sora::SoraCore::Instance();  
    inst->beginScene();

    if(mSprite)
        mSprite->render();
    
    if(mFont) {
        mFont->render(0.f, getWindowHeight()-100.f, sora::FONT_ALIGNMENT_LEFT, 
                      L"Usage: Press 1 to apple effect to font\n    Press 2 to attach a bloom shader for the fullscreen Buffer\n    Press 3 to attach a coloreffect shader to the fullscreen Buffer\n   Press 4 to detach the shaders\n     Press 5 to animate sprite with finish delegate");
        mFont->render(getWindowWidth()/2, getWindowHeight()/2, sora::FONT_ALIGNMENT_CENTER, L"Resource loaded from zip file");
    }
    

    sora::SORA->endScene();
    
    return false;
}

void MainWindow::onKeyEvent(sora::SoraKeyEvent* kev) {
    if(kev->isKeyPressed(SORA_KEY_1)) {
        if(mFont)
            sora::CreateModifierAdapter(mFont, new sora::SoraFontRotationModifier(0.f, 360.f, 5.f, true, true));
    }
    else if(kev->isKeyPressed(SORA_KEY_2)) {
        mBuffer->attachShader(mShader1);
    }
    else if(kev->isKeyPressed(SORA_KEY_3)) {
        mBuffer->attachShader(mShader2);
    } 
    else if(kev->isKeyPressed(SORA_KEY_4)) {
        mBuffer->clearShader();
    } 
    else if(kev->isKeyPressed(SORA_KEY_5)) {
        mSprite->addEffect(sora::CreateEffectListWithDelegate(new sora::SoraImageEffectTransitions(0.f, 0.f, 100.f, 0.f, 2.f),
                                                              new sora::SoraImageEffectTransitions(100.f, 0.f, 100.f, 100.f, 2.f),
                                                              new sora::SoraImageEffectTransitions(100.f, 100.f, 0.f, 100.f, 2.f),
                                                              new sora::SoraImageEffectTransitions(0.f, 100.f, 0.f, 0.f, 2.f),
                                                              new sora::SoraImageEffectColorTransitions(0xFFFFFFFF, 0x00000000, 2.f),
                                                              new sora::SoraImageEffectColorTransitions(0x00000000, 0xFFFFFFFF, 2.f),
                                                              sora::Delegate(this, &MainWindow::onImageEffectEnd)));
    }
}

void MainWindow::onScreenBufferRender(sora::SoraTextureHandle tex) {
    mBuffer->setTexture(tex);
    mBuffer->render();
}
#include "SoraPath.h"
#include "SoraOSXFileUtility.h"

void MainWindow::init() {
    sora::SORA->setFPS(60);    
    printf("%s\n", sora::ws2s(sora::osxApplicationPath()).c_str());

    registerEventFunc(this, &MainWindow::onKeyEvent);
    sora::SoraEventManager::Instance()->registerInputEventHandler(this);
    
    ulong32 resourcePack = sora::SORA->loadResourcePack(L"resource.zip");
    if(!resourcePack)
        THROW_SORA_EXCEPTION(RuntimeException, "Error loading resource pack");
    else
        sora::SORA->attachResourcePack(resourcePack);
    
    mFont = sora::SORA->createFont(L"ThonburiBold.ttf", 16);
    if(!mFont)
        THROW_SORA_EXCEPTION(RuntimeException, "Error creating font");
    
    mSprite = sora::SORA->createSprite(L"geotest.png");
    if(!mSprite)
        THROW_SORA_EXCEPTION(RuntimeException, "Error creating sprite");
    
    // creating a empty sprite for screen buffer
    mBuffer = new sora::SoraSprite(NULL);
    // attach a shader to it
    mShader1 = sora::CreateShader(L"bloom.cg", "Bloom", sora::FRAGMENT_SHADER);
    if(!mShader1)
        THROW_SORA_EXCEPTION(RuntimeException, "Error creating shader");
    mShader2 = sora::CreateShader(L"gray.cg", "Gray", sora::FRAGMENT_SHADER);
    if(!mShader2)
        THROW_SORA_EXCEPTION(RuntimeException, "Error creating shader");
    
    // enable fullscreen buffer
    sora::SORA->enableFullscreenBuffer(true);
    sora::SORA->registerFullscreenBufferDelegate(sora::Bind(this, &MainWindow::onScreenBufferRender));
}