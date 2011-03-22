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

typedef struct {
    void* pixels;
    int32 frameCount;
} MP_CTX;

class vlcWindow: public sora::SoraWindowInfoBase {
public:
	vlcWindow() {}
	~vlcWindow() {}
	
	bool updateFunc();
	bool renderFunc();
	void init();
	
	int32 getWindowWidth() { return 1280; }
	int32 getWindowHeight() { return 720; }
	
	int32 getWindowPosX() { return 0; }
	int32 getWindowPosY() { return 0; }
	
	SoraString getWindowName() { return "VlcTest"; }
	SoraString getWindowId() { return "MainWindow"; }
	
	bool isWindowSubWindow() { return false; }	
	bool isWindowed() { return true; }
	bool hideMouse() { return false; }
		
private:
        
    libvlc_instance_t* vlcInstance;
    libvlc_media_player_t* mp;
    libvlc_media_t* media;
    libvlc_media_track_info_t* pTrackInfo;
    
    
};


#endif