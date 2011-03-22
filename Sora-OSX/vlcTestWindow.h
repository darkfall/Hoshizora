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


class vlcWindow: public sora::SoraWindowInfoBase {
public:
	vlcWindow() {}
	~vlcWindow() {}
	
	bool updateFunc();
	bool renderFunc();
	void init();
	
	int32 getWindowWidth() { return 960; }
	int32 getWindowHeight() { return 640; }
	
	int32 getWindowPosX() { return 0; }
	int32 getWindowPosY() { return 0; }
	
	SoraString getWindowName() { return "VlcTest"; }
	SoraString getWindowId() { return "MainWindow"; }
	
	bool isWindowSubWindow() { return false; }	
	bool isWindowed() { return true; }
	bool hideMouse() { return false; }
		
private:
    
    sora::SoraVlcMoviePlayer* moviePlayer;
};


#endif