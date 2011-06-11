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
				defaultBg = config->getString("public", "defaultbg", "\0");
				
				texW = config->getInt("public", "texwidth", 32);
				texH = config->getInt("public", "texheight", 32);
				col = config->getInt("public", "col", 4);
				row = config->getInt("public", "row", 4);
				defaultps = config->getString("public", "particlesprite", "\0");
			}
			
			delete config;
			sora::SORA->freeResourceFile(data);
		}
		return true;
	};
	
	int32 getWidth() const { return mWidth; }
	int32 getHeight() const { return mHeight; }
	
	int32 getTexWidth() const { return texW; }
	int32 getTexHeight() const { return texH; }
	
	int32 getCol() const { return col; }
	int32 getRow() const { return row; }
	
	std::string getDefaultBg() const { return defaultBg; }
	std::string getDefaultParticleSprite() const { return defaultps; }
	
private:
	int32 mWidth;
	int32 mHeight;
	
	int32 texW;
	int32 texH;
	int32 col;
	int32 row;
	
	std::string defaultBg;
	std::string defaultps;
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
		peMainWindowLoader::Instance()->loadConfig();
		
		sora::GCN_GLOBAL->setGlobalBaseColor(0xFF999999);
	

		sora::SORA->attachResourcePack(sora::SORA->loadResourcePack(L"resource.SoraResource"));
		sora::SORA->setSystemFont(L"cour.ttf", 16);

		sora::SORA->setFPS(60);

		if(!sora::GCN_GLOBAL->initGUIChan(L"cour.ttf", 14)) {
			sora::SORA->messageBox("Error initializing GUIChan, exiting...", "Fatal Error", MB_OK | MB_ICONERROR);
			sora::SORA->shutDown();
		}

		pEditor = new editorWindow;
		if(!pEditor->loadXML(L"editor.xml")) {
			sora::SORA->messageBox("Error loading gui config, exiting...", "Fatal Error", MB_OK | MB_ICONERROR);
			sora::SORA->shutDown();
		}
		
		loadBGSprite(sora::s2ws(peMainWindowLoader::Instance()->getDefaultBg()));
		
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
	
	void onKeyEvent(sora::SoraKeyEvent* kev) {
		if(kev->key == SORA_KEY_1 && kev->type == SORA_INPUT_KEYDOWN) {			
			pEditor->loadXML(L"editor.xml");
		}
	}
	
public:
	editorWindow* pEditor;
};

#endif