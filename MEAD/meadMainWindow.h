/*
 *  meadMainWindow.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/14/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef MEAD_MAIN_WINDOW_H_
#define MEAD_MAIN_WINDOW_H_

#include "SoraWindowInfo.h"
#include "SoraLua/SoraLuaObject.h"
#include "SoraCore.h"

namespace mead {
	
	
	class meadMainWindow: public sora::SoraWindowInfoBase {
	public:
		meadMainWindow();
		~meadMainWindow();
		
		bool updateFunc();
		bool renderFunc();
		void init();
		
		int32 getWindowWidth() { return 800; }
		int32 getWindowHeight() { return 600; }
		
		int32 getWindowPosX() { return 0; }
		int32 getWindowPosY() { return 0; }
		
		SoraString getWindowName() { return "Project MEAD"; }
		SoraString getWindowId() { return "MainWindow"; }
		
		bool isWindowSubWindow() { return false; }	
		bool isWindowed() { return true; }
		bool hideMouse() { return false; }
		
		void onKeyEvent(const sora::SoraKeyEvent* kev);
		
	private:
		sora::SoraCore* sora;
		
		sora::SoraFont* pFont;
		sora::SoraLuaObject luaObject;
	};
	
} // namespace mead


#endif // MEAD_MAIN_WINDOW_H_