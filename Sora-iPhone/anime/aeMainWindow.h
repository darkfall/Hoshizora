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

#include "Hoshizora.h"
#include "SoraGUIChan/guichansetup.h"

#include "editorWindow.h"
#include "SoraZipResourceManager/SoraZipResourceManager.h"
#include "SoraFileUtility.h"
#include "SoraiOSFontManager/SoraiOSFontManager.h"
#include "SoraiOSGLRenderer/SoraiOSInput.h"

class aeMainWindow: public sora::SoraWindowInfoBase {
	
	bool updateFunc() { 
		pEditor->update();
		sora::GCN_GLOBAL->gcnLogic();
		return false;
	}
	
	bool renderFunc() {
		sora::SORA->beginScene();
		
		pEditor->render();
		pFont->print(0.f, 450.f, sora::FONT_ALIGNMENT_LEFT, L"FPS: %.2f", sora::SORA->getFPS());
		sora::GCN_GLOBAL->gcnDraw();
		
		sora::SORA->endScene();
		return false;
	}
	
	void init() {		
		sora::SORA->registerResourceManager(new sora::SoraZipResourceManager);
		sora::SORA->registerFontManager(new sora::SoraiOSFontManager);
		sora::SORA->registerInput(new sora::SoraiOSInput);
		
		sora::SORA->setFPS(60);sora::SORA->messageBox("Error initializing GUIChan, make sure ARIALN.ttf exists, exiting...", "Fatal Error", MB_OK | MB_ICONERROR);
		
		
		if(!sora::GCN_GLOBAL->initGUIChan(L"Courier", 20)) {
			sora::SORA->messageBox("Error initializing GUIChan, make sure ARIALN.ttf exists, exiting...", "Fatal Error", MB_OK | MB_ICONERROR);
			sora::SORA->shutDown();
		}
		sora::GCN_GLOBAL->createTop();
		
		pEditor = new animationEditorWindow;
		gcn::Widget* pWindow = pEditor->loadGUIConfig(L"editorWindow.json");
		if(pWindow) {
			sora::GCN_GLOBAL->addWidget(pWindow, "top");
		}
		
		pFont = sora::SORA->createFont(L"Courier", 18);
		
	}
	
	int32 getWindowWidth() { return 320; }
	int32 getWindowHeight() { return 480; }
	
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