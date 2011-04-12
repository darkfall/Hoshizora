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

bool mainWindow::updateFunc() {
	mainScenes->update(sora->getDelta());
	
	if(sora->keyDown(SORA_KEY_ESCAPE))
		sora->shutDown();
	if(sora->keyDown(SORA_KEY_F))
		sora->setFullscreen(false);
    
    if(sora->keyDown(SORA_KEY_1))
       sora->snapshot("test.bmp");

	
    return false;
}

sora::SoraSprite* pSpr2;

bool mainWindow::renderFunc() {
	sora->beginScene();
	
	//mainScenes->render();
	//pSpr->render4V(100.f, 100.f, 700.f, 0.f, 700.f, 600.f, 100.f, 500.f);
   // sora->setViewPoint(0.f, 0.f, 1.f);
    p3->render(0.f, 0.f);
    pSpr->render(0.f, 0.f);

    pSpr2->render(0.f, 0.f);
    
	float t = 0.56;
	//shader->setParameterfv("twisting", &t, 1);

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
    
    pSpr = sora::SORA->createSprite(L"titlebg2.png");
	pSpr2 = sora::SORA->createSprite(L"sea.png");
    p3 = sora::SORA->createSprite(L"stgui.png");
    p3->setZ(-0.4f);  p3->setBlendMode(BLEND_DEFAULT_Z);
    pSpr2->setZ(0.5f);
    pSpr->setZ(-0.3f); pSpr->setBlendMode(BLEND_DEFAULT_Z);
    pSpr2->setColor(0x88FFFFFF);
	
	luaobj = new sora::SoraLuaObject();
	luaobj->doScript(L"test.txt");
//	pSpr2->setScale(0.5f, 0.5f);
	//shader = pSpr2->attachShader(L"C3E2v_varying.cg", "C3E2v_varying", sora::VERTEX_SHADER);
	sora::SoraShader* s = pSpr2->attachShader(L"gray.ps", "simplePointLight", sora::FRAGMENT_SHADER);
}

void mainWindow::onMenuClick(const menuEvent* mev) {
	//printf("%s, %d\n", mev->menu.c_str(), mev->item);
	if(mev->menu.compare("difficulty") == 0) {
		mainScenes->switchToScene(stg, 1.f);
	}
}