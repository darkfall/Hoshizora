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
#include "SoraCanvas.h"
#include "SoraPlatform.h"
#include "SoraParticleSystem/SoraParticleSystem.h"
#include "SoraLua/SoraLuaObject.h"

#include "SoraFileChangeEvent.h"

#include "SoraScene.h"
#include "SoraWindowInfo.h"

class mainWindow: public sora::SoraWindowInfoBase {
public:
	mainWindow();
	~mainWindow();
	
	bool updateFunc();
	bool renderFunc();
	void init();
	
	int32 getWindowWidth() { return 1024; }
	int32 getWindowHeight() { return 768; }
	
	int32 getWindowPosX() { return 0; }
	int32 getWindowPosY() { return 0; }
	
	SoraString getWindowName() { return "Reflection"; }
	SoraString getWindowId() { return "MainWindow"; }
	
	bool isWindowSubWindow() { return false; }	
	bool isWindowed() { return true; }
	bool hideMouse() { return false; }
	
	void onKeyEvent(sora::SoraKeyEvent* kev);
    	
private:
	sora::SoraCore* sora;
    
    sora::Sora2DScene* mScene1;
    sora::Sora2DScene* mScene2;
    sora::Sora2DCamera* mCamera;
};

#endif