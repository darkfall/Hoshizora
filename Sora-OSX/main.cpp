#include <iostream>


#include "SoraCore.h"

#include "SoraWindowInfo.h"

#include "SoraFont.h"

#include "SoraZipResourceManager/SoraZipResourceManager.h"
#include "SoraFreetypeFont/SoraFTFontManager.h"

#include "testMainWindow.h"
#include "rfMainWindow.h"
#include "mainWindow.h"

#include "SoraLocalizer.h"

#include "SoraMemoryFile.h"

#include "vlcTestWindow.h"

#include "SoraGUIChan/XmlGui.h"
#include "SoraGUIChan/Jsongui.h"

#include "Debug/SoraAutoProfile.h"

#include "../AnimationEditor/aeMainWindow.h"
#include "../SoraParticleEditor/peMainWindow.h"

//#include "GLFW3/SoraOGLInputGLFW3.h"
//#include "GLFW3/SoraOGLRenderer.h"
#include "SoraFMODSoundSystem/SoraFMODSoundSystem.h"
#include "SoraOGLRenderer/SoraOGLRenderer.h"
#include "SoraOGLRenderer/SoraOGLInput.h"

#include "../mead/meadMainWindow.h"
#include "../ReflectionTD/rftdMainWindow.h"

void mainWindow::init() {
	myTarget = sora::SORA->createTarget(0, 0, true);
	
	mySprite = new sora::SoraSprite(0);
}

bool mainWindow::render() {
	sora::SORA->beginScene(0, myTarget);
	sora::SORA->renderBox(0.f, 
						  0.f, 
						  getWindowWidth(), 
						  getWindowHeight(), 
						  0xFFFFFFFF, 
						  0.f);
	sora::SORA->endScene();
	
	sora::SORA->beginScene();
	
	mySprite->setTexture(sora::SORA->getTargetTexture(myTarget));
	mySprite->render();
	
	sora::SORA->endScene();
}

int main(int argc, char* argv[]) {

	
	sora::SoraCore* sora = sora::SoraCore::Instance();
	sora->registerRenderSystem(new sora::SoraOGLRenderer);
	sora->registerResourceManager(new sora::SoraZipResourceManager);
	sora->registerFontManager(new sora::SoraFTFontManager);
	sora->registerInput(new sora::SoraOGLInput);
    sora->registerSoundSystem(new sora::SoraFMODSoundSystem);
	
	sora::HSORARESOURCE res = sora->loadResourcePack(L"./data.rfResource");
	if(!res) {
		sora->messageBox("Error loading resource data", "Fatal Error", MB_OK | MB_ICONERROR);
		sora->shutDown();
	}
	sora->attachResourcePack(res);
	
	peMainWindowLoader::Instance()->loadConfig();
	sora->createWindow(new peMainWindow);
	sora->start();
		
	return 0;
}
