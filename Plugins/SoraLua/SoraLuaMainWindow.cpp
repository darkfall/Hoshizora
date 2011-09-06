/*
 *  SoraLuaMainWindow.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/16/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraLuaMainWindow.h"

#include "SoraLuaFunctionHelper.h"
#include "SoraLuaObject.h"

namespace sora {

	SoraLuaMainWindow::SoraLuaMainWindow() {
	}
	
	SoraLuaMainWindow::SoraLuaMainWindow(const util::String& scriptPath) {
		mScriptObj.doScript(scriptPath);
	}
		
	SoraLuaMainWindow::~SoraLuaMainWindow() {
	}
	
	int32 SoraLuaMainWindow::getWindowWidth() {
		return callLuaFunc<int32>(mScriptObj.getState(), "getWindowWidth");
	}
	
	int32 SoraLuaMainWindow::getWindowHeight() {
		return callLuaFunc<int32>(mScriptObj.getState(), "getWindowHeight");
	}
	
	int32 SoraLuaMainWindow::getWindowPosX() {
		return callLuaFunc<int32>(mScriptObj.getState(), "getWindowPosX");
	}
	
	int32 SoraLuaMainWindow::getWindowPosY() {
		return callLuaFunc<int32>(mScriptObj.getState(), "getWindowPosY");
	}
	
	util::String SoraLuaMainWindow::getWindowName() {
		return callLuaFunc<util::String>(mScriptObj.getState(), "getWindowName");
	}
	
	util::String SoraLuaMainWindow::getWindowId() {
		return callLuaFunc<util::String>(mScriptObj.getState(), "getWindowId");
	}
	
	void SoraLuaMainWindow::init() {
		callLuaFunc<void>(mScriptObj.getState(), "init");
	}
	
	bool SoraLuaMainWindow::isWindowed() {
		return callLuaFunc<bool>(mScriptObj.getState(), "isWindowed");
	}
	
	bool SoraLuaMainWindow::hideMouse() {
		return callLuaFunc<bool>(mScriptObj.getState(), "hideMouse");
	}
	
	bool SoraLuaMainWindow::renderFunc() {
		return callLuaFunc<bool>(mScriptObj.getState(), "render");
	}
	
	bool SoraLuaMainWindow::updateFunc() {
		return callLuaFunc<bool>(mScriptObj.getState(), "update");
	}


} // namespace sora