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

#include "SoraLua/SoraLuaObject.h"

class mainWindow: public sora::SoraWindowInfoBase {
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
	
	SoraString getWindowName() { return "Portal ´«ËÍÃÅ2"; }
	SoraString getWindowId() { return "MainWindow"; }
	
	bool isWindowSubWindow() { return false; }	
	bool isWindowed() { return true; }
	bool hideMouse() { return false; }

	const char* getIcon() { return "./t1.ico"; }
	const char* getCursor() { return "./RaiXPS_04_busy.ani"; }

	void onKeyEvent(const sora::SoraKeyEvent* kev);
	
private:
	sora::SoraCore* sora;
                    
    sora::SoraSprite* pSpr;
	sora::SoraShader* shader;
					
    sora::SoraFont* pFont;
	
	sora::SoraBaseCanvas* canvas1;

	sora::SoraLuaObject luaObject;
};

#endif