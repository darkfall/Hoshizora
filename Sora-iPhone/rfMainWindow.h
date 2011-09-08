/*
 *  rfMainWindow.h
 *  Sora
 *
 *  Created by GriffinBu on 12/14/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */


#ifndef RF_MAIN_WINDOW_H_
#define RF_MAIN_WINDOW_H_

#include "rfMapRenderer.h"

#include "SoraiOSDeviceHelper.h"
#include "SoraiOSGLRenderer/SoraiOSMainWindow.h"
#include "guichansetup.h"
#include "gcnImageButton2.h"

//#include "lsAnimatedSprite.h"

enum RF_STATE {
	STATE_LOGO = 0,
	STATE_FAILED = 1,
	STATE_SUCCEED = 2,
	STATE_GAMING = 3,
	STATE_RV_RETRY = 4,
	STATE_RV_CONT = 5,
	STATE_PAUSE_MENU = 6,
};

typedef struct {
	std::wstring mapPath;
	uint16 maxMirror;
	uint16 maxTime;
} RF_MAP_INFO;

class rfMainWindow: public sora::SoraiOSMainWindow {
public:
	~rfMainWindow();
	
	bool renderFunc();	
	bool updateFunc();
	void init();
	
	void renderState();
	
	void fail();
	void success();
	
	void tonext();
	void retry();
	
	int32 loadMapList(const wchar_t* script);
	int32 loadMap(const wchar_t* map, uint16 maxMirror, uint16 maxTime=0);
	int32 toNextMap();
	
	size_t getAvailableMapCount() const;
	
private:
	sora::SoraSprite* pBG;
	sora::SoraSprite* pBlackBG;
	
	int32 rfState;
	float32 stateCount;
	sora::SoraSprite* pState;
	
	gcn::ImageButton2* pretry;
	gcn::ImageButton2* pext;
	gcn::ImageButton2* pcontinue;
	
	float32 logoCount;
//	ls::lsAnimatedSprite* pLogoAnim;
	
	sora::SoraCore* CORE;
    sora::SoraFont* pFont;
	
	rf::rfMapRenderer* prfMapRenderer;
	
	typedef std::vector<RF_MAP_INFO> RF_MAP_CONT;
	RF_MAP_CONT availMaps;
	int32 currMapIndex;
    
    ulong32 fbo;
};


#endif