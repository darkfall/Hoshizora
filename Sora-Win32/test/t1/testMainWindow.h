/*
 *  testMainWindow.h
 *  Sora
 *
 *  Created by GriffinBu on 12/14/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef TEST_MAIN_WINDOW_H_
#define TEST_MAIN_WINDOW_H_

#include "lsAnimatedSprite.h"
#include "SoraMath.h"

#include "SoraParticleSystem/SoraParticleSystem.h"

#include "SoraFMODSound/fmod/fmod.hpp"


class testMainWindow: public sora::SoraWindowInfoBase {
	
	bool updateFunc();
	bool renderFunc();
	void init();
	
	int32 getWindowWidth() { return 1024; }
	int32 getWindowHeight() { return 800; }
	
	int32 getWindowPosX() { return 0; }
	int32 getWindowPosY() { return 0; }
	
	SoraString getWindowName() { return "HoshiNoSora"; }
	SoraString getWindowId() { return "MainWindow"; }
	
	bool isWindowSubWindow() { return false; }	
	bool isWindowed() { return true; }
	bool hideMouse() { return false; }
	
public:
	sora::SoraCore* CORE;
	sora::SoraFont* font;
	
	sora::SoraParticleSystem* par;
		
	ls::lsAnimatedSprite* pAnimatedSprite;
	
	/*FMOD_SOUND* bgm;
	FMOD_SYSTEM* sys;
	FMOD_CHANNEL* cha;
	void* psdata1;
	void* psdata2;
	unsigned int datapos1, datapos2;
	*/
	
	float32 rr, rp, ry;
	
};


#endif