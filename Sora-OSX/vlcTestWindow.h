//
//  vlcTestWindow.h
//  Sora
//
//  Created by Griffin Bu on 3/21/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//
#ifndef VLC_TEST_WINDOW_H_
#define VLC_TEST_WINDOW_H_

#include "SoraCore.h"
#include "vlc/vlc.h"
#include "libvlc/SoraVlcMoviePlayer.h"

#include "SoraGUIChan/SoraGUIResponser.h"
#include "SoraGUIChan/guichansetup.h"

#include "Darklib/FileDlgOsx.h"

#include "SoraLua/SoraLuaObject.h"
#include "SoraCanvas.h"

class vlcWindow: public sora::SoraWindowInfoBase, public sora::SoraGUIResponser {
public:
	vlcWindow() {}
	~vlcWindow() {}
	
	bool updateFunc();
	bool renderFunc();
	void init();
	
	int32 getWindowWidth() { return 1024; }
	int32 getWindowHeight() { return 768; }
	
	int32 getWindowPosX() { return 0; }
	int32 getWindowPosY() { return 0; }
	
	SoraString getWindowName() { return "VlcTest"; }
	SoraString getWindowId() { return "MainWindow"; }
	
	bool isWindowSubWindow() { return false; }	
	bool isWindowed() { return true; }
	bool hideMouse() { return false; }
    
    void action();
    
    inline void renderMovieImage();
	
	void onKeyEvent(const sora::SoraKeyEvent* kev);
		
private:
    void _loadGUI();
    
    sora::SoraVlcMoviePlayer* moviePlayer;
    
    sora::SoraLuaObject luaTest;

    
    FileDlg* pFileOpener;
    
    sora::SoraSprite* pSpr;
    sora::SoraSprite* pFogSpr;
    sora::SoraTexture* pTex;
    sora::SoraFont* pFont;
    
    gcn::Slider* pslider;
    
    sora::SoraShader* shader;
    
    sora::SoraBaseCanvas* canvas1;
    sora::SoraBaseCanvas* canvas2;
    
    ulong32 target;
    ulong32 target2;
};


#endif