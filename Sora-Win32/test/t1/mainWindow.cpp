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
	if(sora->keyDown(SORA_KEY_Q))
		testEntity->setLife(90);
	if(sora->keyDown(SORA_KEY_W))
		testEntity->setLife(20);
	
	if(sora->keyDown(SORA_KEY_1))
		sora->messageBox("Error loading resource data", "Fatal Error", MB_OK | MB_ICONWARNING);
	if(sora->keyDown(SORA_KEY_2))
		sora->messageBox("Error loading resource data", "Fatal Error", MB_OKCANCEL | MB_ICONERROR);
	if(sora->keyDown(SORA_KEY_3))
		sora->messageBox("Error loading resource data", "Fatal Error", MB_OK | MB_ICONSTOP);
	if(sora->keyDown(SORA_KEY_4))
		sora->messageBox("Error loading resource data", "Fatal Error", MB_OK | MB_ICONINFORMATION);

	
	testEntity->update(0.f);
	
	return false;
}

bool mainWindow::renderFunc() {
	sora->beginScene();
	
	mainScenes->render();
	
	sora->endScene();
	
	sora->beginScene();
	//mainScenes->render();

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
	
	testEntity = new sora::BaseEntity;
	testEntity->setLife(100);
}

void mainWindow::onMenuClick(const menuEvent* mev) {
	//printf("%s, %d\n", mev->menu.c_str(), mev->item);
	if(mev->menu.compare("difficulty") == 0) {
		mainScenes->switchToScene(stg, 1.f);
	}
}