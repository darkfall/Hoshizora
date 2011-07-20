//
//  SoraMainWindow.h
//  Hoshizora
//
//  Created by Ruiwei Bu on 7/16/11.
//  Copyright 2011 Robert Bu. All rights reserved.
//

#ifndef Hoshizora_SoraMainWindow_h
#define Hoshizora_SoraMainWindow_h

#include "SoraWindowInfo.h"
#include "SoraFont.h"
#include "SoraShader.h"

#include "SoraEventManager.h"
#include "SoraKeyInfo.h"

#include "SoraScene.h"
#include "SoraCamera.h"

class MainWindow: public sora::SoraWindowInfoBase {
public:
    bool updateFunc();
	bool renderFunc();
	void init();
	
	int32 getWindowWidth() { return 800; }
	int32 getWindowHeight() { return 600; }
	
	int32 getWindowPosX() { return 0; }
	int32 getWindowPosY() { return 0; }
	
	SoraString getWindowName() { return "Tutorial 6"; }
	SoraString getWindowId() { return "Tutorial 6"; }
	
	bool isWindowSubWindow() { return false; }	
	bool isWindowed() { return true; }
	bool hideMouse() { return false; }
    
    void onKeyEvent(sora::SoraKeyEvent* kev);
    
private:
    sora::SoraFont* mFont;
    
    sora::SoraSprite* mSprite1;
    sora::SoraSprite* mSprite2;
    sora::SoraSprite* mSprite3;
    
    sora::SoraScene* mScene1;
    sora::SoraScene* mScene2;
    sora::SoraCamera* mCamera;
};

#endif
