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

#include "SoraHttpFile/SoraHttpFile.h"

#include "SoraNetwork/clsockets/ActiveSocket.h"
#include "SoraNetwork/clsockets/PassiveSocket.h"

#include "SoraFileChangeEvent.h"

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
	void onMenuEvent(sora::SoraMenuBarClickEvent* ev);
	void onDownloadEvent(sora::SoraHttpDownloadEvent* ev);
	void onFileChangeEvent(sora::SoraFileChangeEvent* ev);
    
    const char* getIcon() {
        return "./t1.png";
    }
    
    const char* getCursor() {
        return "./t1c.png";
    }
	
private:
	sora::SoraCore* sora;
	
	sora::SoraBaseCanvas* pCanvas;
                    
    sora::SoraSprite* pSpr;
	sora::SoraSprite* pSpr2;
	sora::SoraSprite* pressAnyKey;
	
	sora::SoraShader* shader;
					
    sora::SoraFont* pFont;
	sora::SoraParticleManager* ps;
	sora::SoraSprite* psSpr;
	sora::SoraBaseCanvas* canvas1;
	
	sora::SoraLuaObject obj;
	
	sora::SoraHttpFile file;
	
	CActiveSocket asocket;
	CPassiveSocket psocket;
};

#endif