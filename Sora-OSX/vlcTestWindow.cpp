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
#include "graphicEffects.h"

#include "SoraLua/guichanwrapper.h"
#include "Debug/SoraDebugRenderer.h"
#include "Debug/SoraAutoProfile.h"

#include "SoraEventManager.h"

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
    
    float mousex, mousey;
    sora::SORA->getMousePos(&mousex, &mousey);
    float lightPos[2];
    lightPos[0] = (mousex - 100) / 848;
    lightPos[1] = (mousey)/499;
    if(lightPos[0] < 1.0 && lightPos[1] < 1.0 && lightPos[0] > 0.0 && lightPos[1] > 0.0)
        shader->setParameterfv("lightPos", lightPos, 2);
    

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

void vlcWindow::renderMovieImage() {
    if(pSpr) {
		pSpr->setScale(1.5f, 1.5f);

    //    pSpr->setCenter(pSpr->getSpriteWidth()/2, 0);

       pSpr->render(0.f, 0.f);
    }
}

bool vlcWindow::renderFunc() {
  //  sora::PROFILE("movierender");

    canvas1->beginRender();
	pFont->setScale(4.f);

    pFont->print(0.f, getWindowHeight()-50.f, sora::FONT_ALIGNMENT_LEFT, L"SoraCoreRenderFPS: %f, VideoFPS: %f", sora::SORA->getFPS(), moviePlayer->getFPS());
    pFont->print(0.f, getWindowHeight()-30.f, sora::FONT_ALIGNMENT_LEFT, L"FrameCount: %d", moviePlayer->getFrameCount());
    pFont->print(0.f, getWindowHeight()-70.f, sora::FONT_ALIGNMENT_LEFT, L"Time: %llu, Length: %llu", 
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
	
    
    canvas1->finishRender();

    if(moviePlayer->isPlaying()) {
        if(moviePlayer->frameChanged()) {
            ulong32* data = pSpr->getPixelData();
            if(data)
                memcpy(data, moviePlayer->getPixelData(), moviePlayer->getWidth()*moviePlayer->getHeight()*4);
            pSpr->unlockPixelData();
            
            moviePlayer->setFinish();
        }
    } 
    canvas2->beginRender();

    renderMovieImage();

    sora::GCN_GLOBAL->gcnDraw();

    canvas2->finishRender();
    
    sora::SORA->beginScene(0);
  //  renderMovieImage();
   // sora::GCN_GLOBAL->gcnDraw();


    canvas1->render();

   canvas2->render();

    sora::SORA->endScene();
       
    return false;
}

void vlcWindow::_loadGUI() {
    sora::GCN_GLOBAL->initGUIChan(L"cour.ttf", 20);
    sora::GCN_GLOBAL->createTop();
    
    sora::SoraGUIResponserMap::Instance()->registerResponser("PlayerController", this);

    sora::XmlGui* pguiloader = new sora::XmlGui;
    
    if(!pguiloader->parse(L"./player.xml")) {
        sora::SORA->messageBox("Error loading gui xml conf file!", "Fatal Error", MB_OK | MB_ICONERROR);
        sora::SORA->shutDown();
    } else {
        sora::GCN_GLOBAL->getTop()->add(pguiloader->getWidget("PlayerWindow"));
        pslider = (gcn::Slider*)sora::GCN_GLOBAL->findWidget("PlayerPos");
    }
    
}

void vlcWindow::init() {
    pSpr = new sora::SoraSprite(sora::SORA->createTextureWH(848, 499));;
    pFont = 0;
    
    pslider = 0;
    _loadGUI();
    
    guiwrapper::export_guilib(luaTest.getState());
    
    pFileOpener = new FileDlg;
    pFileOpener->SetDefaultPath(sora::ws2s(sora::SoraFileUtility::getApplicationPath()).c_str());
    pFileOpener->SetFilter("mp4;rm;rmvb;mkv;avi;flv;f4v;");
    
    moviePlayer = new sora::SoraVlcMoviePlayer();

    moviePlayer->openMedia(L"./AMV_Scenario.mp4", 848, 499);
    moviePlayer->play();
        
    sora::SORA->setWindowTitle(L"AMV_Scenerio.mp4");
    sora::SORA->setFPS(999);
    
    sora::DEBUG_RENDERER->addAABB(hgeVector(0.f, 0.f), hgeVector(100.f, 100.f), true, sora::COLOR_RED);
    
    shader = sora::SORA->createShader(L"gray.ps", "simplePointLight", sora::FRAGMENT_SHADER);
    float lightColor[4] = {1.f, 0.7f, 0.3f, 1.f};
    shader->setParameterfv("lightColor", lightColor, 4);
    float lightPos[2] = {0.f, 0.f};
    shader->setParameterfv("lightPos", lightPos, 2);
    float lightRadius = 0.5f;
    shader->setParameterfv("lightRadius", &lightRadius, 1);
    float intensity = 2.f;
    shader->setParameterfv("lightIntensity", &intensity, 1);
   // pSpr->attachShader(shader);
    
    pFont = sora::SORA->createFont(L"ThonburiBold.ttf", 24);
    pFont->setColor(0xFFFFFFFF);
    
    pFogSpr = sora::SORA->createSprite(L"titlebg2.png");
    shader->setTexture("texsample2", pFogSpr->getTexture());
    
    canvas1 = new sora::SoraBaseCanvas(1024, 768);
    canvas2 = new sora::SoraBaseCanvas(1024, 768);
	
	registerEventFunc(this, &vlcWindow::onKeyEvent);
	sora::SORA_EVENT_MANAGER->registerInputEventHandler(this);
}

void vlcWindow::onKeyEvent(const sora::SoraKeyEvent* kev) {
	printf("received key input event, key=%d, type=%d, flag=%d\n", kev->key, kev->type, kev->flags);
}