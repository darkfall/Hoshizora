/*
 *  menuSceneManager.h
 *  Sora
 *
 *  Created by GriffinBu on 12/29/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef L_MENU_SCENE_MANAGER_H_
#define L_MENU_SCENE_MANAGER_H_

#include "menuScene.h"
#include "lScene.h"

#include <vector>

class menuSceneManager {
public:
	menuSceneManager(float32 posx, float32 posy);
	~menuSceneManager();
	
	uint32 addMenuScene(lShineMenuScene* scene);
	
	void update(float32 dt);
	void render();
	
	uint32 getCurrActiveMenu();
	uint32 getCurrActiveMenuItem();
	
	void switchToMenu(uint32 id, float32 time);
	
	bool isActive();
	
private:
	projl::lSceneManager* sceneManager;
	
	typedef std::vector<lShineMenuScene*> SCENE_LIST;
	SCENE_LIST menus;
	
	sora::SoraCore* sora;
	
	uint32 currMenu;
};
	
#endif