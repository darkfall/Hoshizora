//
//  vlcTestWindow.cpp
//  Sora
//
//  Created by Griffin Bu on 3/21/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "vlcTestWindow.h"
#include "SoraFont.h"


#include "SoraGuiChan/xmlgui.h"
#include "SoraGuiChan/SoraGUIResponserMap.h"

#include "SoraFileUtility.h"

static int keyUpDataGuard = 0;

bool vlcWindow::updateFunc() {
    if(keyUpDataGuard % 30 == 0 || keyUpDataGuard == 0) {
        if(sora::SORA->keyDown(SORA_KEY_LEFT)) {
            moviePlayer->setTime(moviePlayer->getTime()-100);
        }
        if(sora::SORA->keyDown(SORA_KEY_RIGHT)) {
            moviePlayer->setTime(moviePlayer->getTime()+100);
        }
        if(sora::SORA->keyDown(SORA_KEY_Q)) {
            moviePlayer->setPlayRate(2.f);
        }
        if(sora::SORA->keyDown(SORA_KEY_W)) {
            moviePlayer->setPlayRate(1.f);
        }
        ++keyUpDataGuard;

    }
    ++keyUpDataGuard;
    if(pslider)
        pslider->setValue(pslider->getScaleEnd() * ((float32)moviePlayer->getTime()/moviePlayer->getLength()));

    sora::GCN_GLOBAL->gcnLogic();

    return false;
}


void vlcWindow::action() {
    if(getID().compare("PlayerPos") == 0) {
        gcn::Slider* pslider = (gcn::Slider*)getSource();
        double pos = pslider->getValue();
        moviePlayer->setTime((s_int64)((pos/pslider->getScaleEnd())*moviePlayer->getLength()));
    } else if(getID().compare("Open") == 0) {
        char path[512];
        char title[128];
        
        moviePlayer->pause();
        if(pFileOpener->FileOpenDlg(0, path, title)) {
            moviePlayer->stop();
            
            moviePlayer->openMedia(sora::s2ws(path), 1024, 1024);
            moviePlayer->play();
        }
        moviePlayer->play();
        
    } else if(getID().compare("Stop") == 0) {
        moviePlayer->stop();
        
    } else if(getID().compare("Pause") == 0) {
        
        gcn::Button* pb = (gcn::Button*)getSource();
        if(moviePlayer->isPlaying()) {
            moviePlayer->pause();
            pb->setCaption("Resume");
        } else {
            moviePlayer->play();
            pb->setCaption("Pause");
        }
    }
    
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
    pFont->print(0.f, getWindowHeight()-110.f, sora::FONT_ALIGNMENT_LEFT, L"Rate: %f", 
                 moviePlayer->getPlayRate());
    pFont->print(0.f, getWindowHeight()-130.f, sora::FONT_ALIGNMENT_LEFT, L"Audio Track Num: %d, Audio Channel: %d", 
                 moviePlayer->getAudioTrackNum(),
                 moviePlayer->getAudioChannelNum());


    if(moviePlayer->isPlaying()) {
        if(moviePlayer->frameChanged()) {
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
            pSpr->setCenter(pSpr->getSpriteWidth()/2, 0);
            pSpr->render(getWindowWidth()/2, 0.f);
       
            moviePlayer->setFinish();
        } else if(pSpr) {
            pSpr->setCenter(pSpr->getSpriteWidth()/2, 0);
            pSpr->render(getWindowWidth()/2, 0.f);
        }
    } else if(pSpr) {
        pSpr->setCenter(pSpr->getSpriteWidth()/2, 0);
        pSpr->render(getWindowWidth()/2, 0.f);
    }
    
    sora::GCN_GLOBAL->gcnDraw();
    sora::SORA->endScene();
   
    return false;
}

void vlcWindow::_loadGUI() {
    sora::GCN_GLOBAL->initGUIChan(L"cour.ttf", 20);
    sora::GCN_GLOBAL->createTop();
    
    sora::SoraGUIResponserMap::Instance()->registerResponser("PlayerController", this);
    
    XmlGui* pguiloader = new XmlGui;
    
    if(!pguiloader->parse("./player.xml")) {
        sora::SORA->messageBox("Error loading gui xml conf file!", "Fatal Error", MB_OK | MB_ICONERROR);
        sora::SORA->shutDown();
    } else {
        sora::GCN_GLOBAL->getTop()->add(pguiloader->getWidget("PlayerWindow"));
        pslider = (gcn::Slider*)sora::GCN_GLOBAL->findWidget("PlayerPos");
    }
    
}

void vlcWindow::init() {
    pTex = 0;
    pSpr = 0;
    pFont = 0;
    
    pslider = 0;
    _loadGUI();
    
    pFileOpener = new FileDlg;
    pFileOpener->SetDefaultPath(sora::ws2s(sora::SoraFileUtility::getApplicationPath()).c_str());
    pFileOpener->SetFilter("mp4;rm;rmvb;mkv;avi;flv;f4v;");
    
    moviePlayer = new sora::SoraVlcMoviePlayer();
    moviePlayer->openMedia(L"./AMV_Scenario.mp4", 848, 499);
    moviePlayer->play();
    
    sora::SORA->setWindowTitle(L"AMV_Scenerio.mp4");
    sora::SORA->setFPS(999);
    
    pFont = sora::SORA->createFont(L"cour.ttf", 24);
    pFont->setColor(0xFFFFFFFF);
    
    pSpr = 0;
}