/*
 *  rftdMainWindow.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/14/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RFTD_MAIN_WINDOW_H_
#define RFTD_MAIN_WINDOW_H_

#include "SoraWindowInfo.h"
#include "SoraCore.h"
#include "SoraFont.h"

#include "rftdMap.h"

namespace rftd {
	
	class rftdMainWindow: public sora::SoraWindowInfoBase {
	public:
		rftdMainWindow();
		~rftdMainWindow();
		
		bool updateFunc();
		bool renderFunc();
		void init();
		
		int32 getWindowWidth() { return 1024; }
		int32 getWindowHeight() { return 768; }
		
		int32 getWindowPosX() { return 0; }
		int32 getWindowPosY() { return 0; }
		
		SoraString getWindowName() { return "Reflection TD"; }
		SoraString getWindowId() { return "MainWindow"; }
		
		bool isWindowSubWindow() { return false; }	
		bool isWindowed() { return true; }
		bool hideMouse() { return false; }
		
		void onKeyEvent(const sora::SoraKeyEvent* kev);
		
	private:
		sora::SoraCore* sora;
		
		sora::SoraFont* pFont;

		rftd::rftdMap* mtmap;
		rftd::MapConf conf;
		
	};
	
}


#endif // RFTD_MAIN_WINDOW_H_