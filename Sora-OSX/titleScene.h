/*
 *  titleScene.h
 *  Sora
 *
 *  Created by GriffinBu on 12/29/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include "SoraCore.h"
#include "lScene.h"
#include "menuSceneManager.h"
#include "backgroundScene.h"

class titleScene: public projl::lScene {
public:
	titleScene();
	~titleScene();
	
	void init();
	
	void render();
	uint32 update(float32 dt);
	
	void onSceneTimerEvent(const projl::lSceneTimerEvent* tev);
	void onTimerEvent(const sora::SoraTimerEvent* ev);
	
private:
	sora::SoraCore* sora;
	sora::SoraSprite* magicCircle;
	
	menuSceneManager* menus;
	backgroundScene* bg;
	
	uint32 titleMenuId, difficultyMenuId;
	
	bool bAvailable;
};


#endif