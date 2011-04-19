/*
 *  mainWindow.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/29/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "mainWindow.h"
#include "SoraEventManager.h"
#include "SoraLocalizer.h"

#include "graphicEffects.h"

mainWindow::mainWindow() {
	sora = sora::SoraCore::Instance();
	
	registerEventFunc(this, &mainWindow::onMenuClick);
	
	sora::SORA_EVENT_MANAGER->registerEvent("MainWindow", this, 0);
}

mainWindow::~mainWindow() {
	delete mainScenes;
	delete title;
	delete stg;
}

sora::SoraSprite* pSpr2;

void mainWindow::loadShader() {
    shader = pSpr->attachShader(L"gray.ps", "simplePointLight", sora::FRAGMENT_SHADER);
    float lightColor[4] = {1.f, 1.f, 1.f, 0.5f};
    shader->setParameterfv("lightColor", lightColor, 4);
    float lightPos[2] = {0.5f, 0.5f};
    shader->setParameterfv("lightPos", lightPos, 2);
    float lightRadius = 0.6f;
    shader->setParameterfv("lightRadius", &lightRadius, 1);
    float intensity = 0.8f;
    shader->setParameterfv("lightIntensity", &intensity, 1);
    float treshold = 1.0f;
    shader->setParameterfv("lightTreshold", &treshold, 1);

//	shader->setTexture("sample22", p3->getTexture());
    
  //  shader->setTexture("texsample2", pSpr2->getTexture());
}
static float32 process = 0.0;


bool mainWindow::updateFunc() {
	mainScenes->update(sora->getDelta());
	
    
    sora::SoraKeyEvent kev;
    if(sora->getKeyEvent(kev)) {
        if(kev.key == SORA_KEY_ESCAPE && kev.type == SORA_INPUT_KEYDOWN)
            sora->shutDown();
    
        if(kev.key == SORA_KEY_1 && kev.type == SORA_INPUT_KEYDOWN) {
            bgm->stop();
        }
    
        if(kev.key == SORA_KEY_2 && kev.type == SORA_INPUT_KEYDOWN) {
            bgm->pause();
        }
        if(kev.key == SORA_KEY_3 && kev.type == SORA_INPUT_KEYDOWN) {
            bgm->resume();
        }
        if(kev.key == SORA_KEY_4 && kev.type == SORA_INPUT_KEYDOWN) {
            bgm->play();
        }
    
        if(kev.key == SORA_KEY_Q && kev.type == SORA_INPUT_KEYDOWN) {
            pSpr->detachShader(shader);
            loadShader();
            process = 0.0;
        }
    }
	
    return false;
}


static float32 lpos[2] = {0.f, 0.f};
static float32 attx = 0.01f, atty = 0.01f;

bool mainWindow::renderFunc() {
	sora->beginScene(0xFFFFFFFF);
	
	//mainScenes->render();
	//pSpr->render4V(100.f, 100.f, 700.f, 0.f, 700.f, 600.f, 100.f, 500.f);
   // sora->setViewPoint(0.f, 0.f, 1.f);
  //  p3->render(0.f, 0.f);
   // pSpr->render(0.f, 0.f);

    pSpr->render(0.f, 0.f);

//	pSpr->setRotation(0.5);
    
	lpos[0] = 0.5f + 0.3f*sinf(sora::DGR_RAD(sora::SORA->getFrameCount()));
    lpos[1] = 0.5f + 0.3f*cosf(sora::DGR_RAD(sora::SORA->getFrameCount()));

    process += 0.01;
//    shader->setParameterfv("process", &process, 1);
 //   shader->setParameterfv("lightPos", lpos, 2);
	
	pFont->print(0.f, 0.f, sora::FONT_ALIGNMENT_LEFT, L"FPS: %f time: %f\0\0", sora::SORA->getFPS(), sora::SORA->getDelta());

	sora->endScene();
    

	return false;
}


void mainWindow::init() {
	mainScenes = new projl::lSceneManager(0.f, 0.f, getWindowWidth(), getWindowHeight());
	mainScenes->setSwitchEffectEnabled(true);
	mainScenes->setSwitchColor(sora::SoraColorRGBA(1.f, 1.f, 1.f, 1.f));
	title = new titleScene;
	mainScenes->addScene(title);
	mainScenes->switchToScene(title, 0.f);
	
	stg = new stgScene;
	mainScenes->addScene(stg);
    
    sora::SORA->setFPS(60);
    
    pSpr = new sora::SoraSprite(NULL);
	pSpr->setTexture(sora::SORA->createTexture(L"titlebg2.png"));
//	pSpr->setTextureRect(0, 0, 500, 500);
 //   pSpr = new sora::SoraSprite(sora::SORA->createTextureWH(512, 512));
//    ulong32* pdata = pSpr->getPixelData();
 //   memset(pdata, 0, 4*512*512);
 //   pSpr->unlockPixelData();
    pSpr2 = sora::SORA->createSprite(L"sea.png");
	
    p3 = sora::SORA->createSprite(L"stgui.png");
    loadShader();
 //   pSpr->setColor(0xFF000000);
 //   p3->setZ(-0.4f);  p3->setBlendMode(BLEND_DEFAULT_Z);
 //   pSpr2->setZ(0.5f);
    //pSpr->setZ(-0.3f); pSpr->setBlendMode(BLEND_DEFAULT_Z);
 //   pSpr2->setColor(0x88FFFFFF);
	
	luaobj = new sora::SoraLuaObject();
	luaobj->doScript(L"test.txt");

	pFont = sora::SORA->createFont(L"ThonburiBold.ttf", 20);
	pFont->setColor(0x00000000);
//	pSpr2->setScale(0.5f, 0.5f);
	//shader = pSpr2->attachShader(L"C3E2v_varying.cg", "C3E2v_varying", sora::VERTEX_SHADER);
//	sora::SoraShader* s = pSpr2->attachShader(L"gray.ps", "simplePointLight", sora::FRAGMENT_SHADER);
    
    registerEventFunc(this, &mainWindow::onSoundEvent);

	bgm = sora::SORA->createMusicFile(L"01.ogg", false);
    bgm->registerEventHandler(this);

	bgm->play();
}

void mainWindow::onSoundEvent(const sora::SoraPlaybackEvent* event) {
    if(event->getSource() == bgm)
        sora::INT_LOG::debugPrintf("RECEIVED SOUND EVENT: evid: %d, evid: %lu, evname: %s1\n", event->getEventType(), event->getName(), sora::id2str(event->getName()));
}

void mainWindow::onMenuClick(const menuEvent* mev) {
	//printf("%s, %d\n", mev->menu.c_str(), mev->item);
	if(mev->menu.compare("difficulty") == 0) {
		mainScenes->switchToScene(stg, 1.f);
	}
}