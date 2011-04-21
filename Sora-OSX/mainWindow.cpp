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
}

bool mainWindow::updateFunc() {
	
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
  	
    pSpr->render(0.f, 0.f);
	pSpr2->render(100.f, 100.f);
	float t = 0.56;
	//shader->setParameterfv("twisting", &t, 1);

	sora->endScene();
    

	return false;
}

void mainWindow::init() {
    pSpr = sora::SORA->createSprite(L"titlebg2.png");
	pSpr2 = sora::SORA->createSprite(L"sea.png");
	
	pSpr2->setScale(0.5f, 0.5f);
	//shader = pSpr2->attachShader(L"C3E2v_varying.cg", "C3E2v_varying", sora::VERTEX_SHADER);
	pSpr2->attachShader(L"gray.ps", "gray", sora::FRAGMENT_SHADER);
}

void mainWindow::onMenuClick(const menuEvent* mev) {
	//printf("%s, %d\n", mev->menu.c_str(), mev->item);

}