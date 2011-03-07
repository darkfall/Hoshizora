#ifndef SORA_PS_MAIN_WINDOW_H
#define SORA_PS_MAIN_WINDOW_H


#include "HoshiNoSora.h"
#include "SoraGUIChan/guichansetup.h"

#include "../../SoraParticleEditor/editorWindow.h"


class peMainWindow: public sora::SoraWindowInfoBase {
	
	bool updateFunc() { 
		pEditor->update();

		sora::GCN_GLOBAL->gcnLogic();
		return false;
	}

	bool renderFunc() {
		sora::SORA->beginScene();
		pEditor->render();
		
		sora::GCN_GLOBAL->gcnDraw();
		
		//sora::SORA->renderRect(0.f, 0.f, 100.f, 100.f, 100.f, 0xFFFFFFFF, -1.f);
		
		sora::SORA->endScene();
		return false;
	}

	void init() {
		sora::SORA->attachResourcePack(sora::SORA->loadResourcePack(L"resource.SoraResource"));
		
		sora::SORA->setFPS(60);

		if(!sora::GCN_GLOBAL->initGUIChan(L"ARIALN.ttf", 14)) {
			sora::SORA->messageBox("Error initializing GUIChan, exiting...", "Fatal Error", MB_OK | MB_ICONERROR);
		}
		sora::GCN_GLOBAL->createTop();

		pEditor = new editorWindow;
		pEditor->loadXML(L"editor.xml");
	}

	int32 getWindowWidth() { return 1024; }
	int32 getWindowHeight() { return 768; }
		
	int32 getWindowPosX() { return 0; }
	int32 getWindowPosY() { return 0; }
		
	SoraString getWindowName()  { return "SoraParticleEditor"; }
	SoraString getWindowId()  { return "MainWindow"; }
		
	bool isWindowed() { return true; }
	bool hideMouse() { return false; }
	
public:
	editorWindow* pEditor;
};

#endif