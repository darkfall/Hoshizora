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
#include "cmd/SoraConsole.h"

class MainWindow: public sora::SoraWindowInfoBase {
public:
    bool updateFunc();
	bool renderFunc();
	void init();
	
	int32 getWindowWidth() { return 800; }
	int32 getWindowHeight() { return 600; }
	
	int32 getWindowPosX() { return 0; }
	int32 getWindowPosY() { return 0; }
	
	SoraString getWindowName() { return "Tutorial 5"; }
	SoraString getWindowId() { return "Tutorial 5"; }
	
	bool isWindowSubWindow() { return false; }	
	bool isWindowed() { return true; }
	bool hideMouse() { return false; }
    
    void onKeyEvent(sora::SoraKeyEvent* kev);
    void onScreenBufferRender(sora::SoraTextureHandle tex);
    void onImageEffectEnd(const void* sender, int32& reserved);
    
private:
    sora::SoraFont* mFont;
    
    sora::SoraSprite* mSprite;
    sora::SoraShader* mShader1;
    sora::SoraShader* mShader2;
    
    sora::SoraSprite* mBuffer;
};

#endif
