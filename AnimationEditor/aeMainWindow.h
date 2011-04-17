/*
 *  aeMainWindow.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/17/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef ANIMATION_EDITOR_H_
#define ANIMATION_EDITOR_H_

#include "HoshiNoSora.h"
#include "SoraGUIChan/guichansetup.h"

#include "editorWindow.h"

class aeMainWindow: public sora::SoraWindowInfoBase {
	
	bool updateFunc() { 
		pEditor->update();
		sora::GCN_GLOBAL->gcnLogic();
		return false;
	}
	
	bool renderFunc() {
		sora::SORA->beginScene();
		
		pEditor->render();
		sora::GCN_GLOBAL->gcnDraw();
		
		sora::SORA->endScene();
		return false;
	}
	
	void init() {		
		sora::SORA->setFPS(60);
		
		if(!sora::GCN_GLOBAL->initGUIChan(L"ARIALN.ttf", 16)) {
			sora::SORA->messageBox("Error initializing GUIChan, make sure ARIALN.ttf exists, exiting...", "Fatal Error", MB_OK | MB_ICONERROR);
			sora::SORA->shutDown();
		}
		sora::GCN_GLOBAL->createTop();
		
		pEditor = new animationEditorWindow;
		gcn::Widget* pWindow = pEditor->loadGUIConfig(L"editorWindow.json");
		if(pWindow) {
			sora::GCN_GLOBAL->addWidget(pWindow, "top");
		}
		
	}
	
	int32 getWindowWidth() { return 800; }
	int32 getWindowHeight() { return 600; }
	
	int32 getWindowPosX() { return 0; }
	int32 getWindowPosY() { return 0; }
	
	SoraString getWindowName()  { return "SoraAnimationEditor"; }
	SoraString getWindowId()  { return "MainWindow"; }
	
	bool isWindowed() { return true; }
	bool hideMouse() { return false; }
	
public:
	animationEditorWindow* pEditor;
	sora::SoraFont* pFont;
};

#endif