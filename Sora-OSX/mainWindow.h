/*
 *  mainWindow.h
 *  Sora
 *
 *  Created by GriffinBu on 12/29/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include "SoraCore.h"
#include "titleScene.h"
#include "stgscene.h"

#include "Experimental/state_test.h"


class mainWindow: public sora::SoraWindowInfoBase,
				public sora::SoraEventHandler {
public:
	mainWindow();
	~mainWindow();
	
	bool updateFunc();
	bool renderFunc();
	void init();
	
	int32 getWindowWidth() { return 800; }
	int32 getWindowHeight() { return 600; }
	
	int32 getWindowPosX() { return 0; }
	int32 getWindowPosY() { return 0; }
	
	SoraString getWindowName() { return "Genso De L"; }
	SoraString getWindowId() { return "MainWindow"; }
	
	bool isWindowSubWindow() { return false; }	
	bool isWindowed() { return true; }
	bool hideMouse() { return false; }
	
	void onMenuClick(const menuEvent* mev);
	
private:
	sora::SoraCore* sora;
	
	projl::lSceneManager* mainScenes;
	titleScene* title;
	stgScene* stg;
                    
    sora::SoraSprite* pSpr;
					
	sora::BaseEntity* testEntity;
};

#endif