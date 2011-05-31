#ifndef SORA_PS_MAIN_WINDOW_H
#define SORA_PS_MAIN_WINDOW_H


#include "Hoshizora.h"
#include "SoraGUIChan/guichansetup.h"
#include "SoraEventManager.h"
#include "SoraINIFile/SoraINIFile.h"

#include "editorWindow.h"

class peMainWindowLoader: public sora::SoraSingleton<peMainWindowLoader> {
public:
	friend class sora::SoraSingleton<peMainWindowLoader>;
	peMainWindowLoader(): mWidth(1280), mHeight(800) {}
	
	bool loadConfig() {
		ulong32 size;
		void* data = sora::SORA->getResourceFile(L"config.ini", size);

		if(data) {
			sora::SoraINIFile* config = new sora::SoraINIFile;
			if(config->readFileMem(data, size)) {
				mWidth = config->getInt("public", "width", 1280);
				mHeight = config->getInt("public", "height", 800);
			}
		}
		return true;
	};
	
	int32 getWidth() const { return mWidth; }
	int32 getHeight() const { return mHeight; }
	
private:
	int32 mWidth;
	int32 mHeight;
};

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
		
		sora::SORA_EVENT_MANAGER->registerInputEventHandler(this);
		registerEventFunc(this, &peMainWindow::onKeyEvent);
	}

	int32 getWindowWidth() { return peMainWindowLoader::Instance()->getWidth(); }
	int32 getWindowHeight() { return peMainWindowLoader::Instance()->getHeight(); }
		
	int32 getWindowPosX() { return 0; }
	int32 getWindowPosY() { return 0; }
		
	SoraString getWindowName()  { return "SoraParticleEditor"; }
	SoraString getWindowId()  { return "MainWindow"; }
		
	bool isWindowed() { return true; }
	bool hideMouse() { return false; }
	
	void onKeyEvent(const sora::SoraKeyEvent* kev) {
		if(kev->key == SORA_KEY_1 && kev->type == SORA_INPUT_KEYDOWN) {			
			pEditor->loadXML(L"editor.xml");
		}
	}
	
public:
	editorWindow* pEditor;
};

#endif