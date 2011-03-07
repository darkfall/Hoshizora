/*
 *  menuSceneManager.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/29/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "menuSceneManager.h"


menuSceneManager::menuSceneManager(float32 posx, float32 posy) {
	sora = sora::SoraCore::Instance();
	sceneManager = new projl::lSceneManager(posx, posy, sora->getScreenWidth()-posx, sora->getScreenHeight()-posy);
	sceneManager->setSwitchEffectEnabled(false);
}

menuSceneManager::~menuSceneManager() {
	delete sceneManager;
	sceneManager = 0;
	for(size_t i=0; i<menus.size(); ++i) {
		delete menus[i];
		menus[i] = 0;
	}
}

uint32 menuSceneManager::addMenuScene(lShineMenuScene* scene) {
	menus.push_back(scene);
	
	sceneManager->addScene(scene);
	
	currMenu = 0;
	return menus.size()-1;
}

void menuSceneManager::update(float32 dt) {
	sceneManager->update(dt);
}

void menuSceneManager::render() {
	sceneManager->render();
}

uint32 menuSceneManager::getCurrActiveMenu() {
	return currMenu;
}

uint32 menuSceneManager::getCurrActiveMenuItem() {
	return menus[currMenu]->getCurrItem();
}

void menuSceneManager::switchToMenu(uint32 id, float32 time) {
	if(id >= 0 && id < menus.size()) {
		currMenu = id;
		menus[currMenu]->setCurrItem(0);
		sceneManager->switchToScene(menus[currMenu], time);
	}
}

bool menuSceneManager::isActive() {
	return menus[currMenu]->isActive();
}