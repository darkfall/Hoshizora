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

#include "Debug/SoraAutoProfile.h"

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
		if(sora::SORA->keyDown(SORA_KEY_ESCAPE)) {
			sora::SORA->shutDown();
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
        TCHAR path[512];
        TCHAR title[128];
        
        moviePlayer->pause();

		if(fileOpener->FileOpenDlg((HWND)sora::SORA->getMainWindowHandle(), path, title)) {
			moviePlayer->openMedia(path, "RV32");
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
    pSpr->setCenter(pSpr->getSpriteWidth()/2, 0);
    pSpr->render(getWindowWidth()/2, 0.f);
}

bool vlcWindow::renderFunc() {
  //  sora::PROFILE("movierender");
  //  ulong32 targ = sora::SORA->createTarget(getWindowWidth(), getWindowHeight(), false);
    sora::SORA->beginScene(/*0x00000000, targ*/);
    
    pFont->print(0.f, getWindowHeight()-50.f, sora::FONT_ALIGNMENT_LEFT, L"SoraCoreRenderFPS: %.2f, VideoFPS: %f", sora::SORA->getFPS(), moviePlayer->getFPS());
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
           /* if(pTex) {
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
            
           // pSpr->attachShader(shader);
            
            renderMovieImage();
       
            moviePlayer->setFinish();*/
			if(moviePlayer->getPixelData() != NULL) {
				ulong32* data = pSpr->getPixelData();
				if(data)
					memcpy(data, moviePlayer->getPixelData(), 4*moviePlayer->getWidth()*moviePlayer->getHeight());
				pSpr->unlockPixelData();
			}
        }
	}
	if(pSpr) {
        renderMovieImage();
    }

   // luaTest.render();
    
    sora::GCN_GLOBAL->gcnDraw();
    sora::SORA->endScene();
    
   /* sora::SoraSprite* ptest = new sora::SoraSprite((sora::SoraTexture*)(sora::SORA->getTargetTexture(targ)));
    sora::SORA->beginScene();
    ptest->render();
    if(pSpr) {
        pSpr->setCenter(pSpr->getSpriteWidth()/2, 0);
        pSpr->render(getWindowWidth()/2, 0.f);
    }
    sora::SORA->endScene();
    
    delete ptest;
    sora::SORA->freeTarget(targ);*/
   
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
    pTex = 0;
    pSpr = 0;
    pFont = 0;
    
    pslider = 0;
    _loadGUI();
    
	fileOpener = new FileDlg;
	fileOpener->SetFilter(TEXT("Movies(*.mp4,*.mkv,*.rmvb,*.wmv)\0*.mp4;*.mkv;*.rmvb;*.wmv\0\0"));

    moviePlayer = new sora::SoraVlcMoviePlayer();

    if(moviePlayer->openMedia(L"./AMV_Scenario.mp4", "RV32"))
		pSpr = sora::SORA->createSpriteTex(sora::SORA->createTextureWH(moviePlayer->getWidth(), moviePlayer->getHeight()));
    
	moviePlayer->play();

	    
    sora::SORA->setWindowTitle(L"AMV_Scenerio.mp4");
    sora::SORA->setFPS(999);
    
    pFont = sora::SORA->createFont(L"cour.ttf", 24);
    pFont->setColor(0xFFFFFFFF);
}