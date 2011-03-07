#include <iostream>


#include "SoraCore.h"

#include "SoraWindowInfo.h"
#include "SoraRenderTarget.h"

#include "SoraFont.h"

#include "SoraOGLRenderer/SoraOGLRenderer.h"
#include "SoraZipResourceManager/SoraZipResourceManager.h"
#include "SoraFreetypeFont/SoraFTFontManager.h"
#include "SoraOGLRenderer/SoraOGLInput.h"
#include "SoraQTSoundSystem/SoraQTSoundSystem.h"

#include "testMainWindow.h"
#include "rfMainWindow.h"
#include "mainWindow.h"

#include "SoraLocalizer.h"

//#include "Experimental/tcmalloc_test.h"

int main(int argc, char* argv[]) {
//	sora::tcmalloc_test::test();
	
	sora::SoraCore* sora = sora::SoraCore::Instance();
	sora->registerRenderSystem(new sora::SoraOGLRenderer);
	sora->registerResourceManager(new sora::SoraZipResourceManager);
	sora->registerFontManager(new sora::SoraFTFontManager);
	sora->registerInput(new sora::SoraOGLInput);
	

		
	sora::HSORARESOURCE res = sora->loadResourcePack(L"./data.rfResource");
	if(!res) {
		sora->messageBox("Error loading resource data", "Fatal Error", MB_OK | MB_ICONERROR);
		sora->shutDown();
	}
	sora->attachResourcePack(res);
	
	sora::SoraLocalizer* localizer = sora::SoraLocalizer::Instance();
	localizer->addLocaleConf(L"en.locale");
	localizer->addLocaleConf(L"cn.locale");
	
	
	//sora->registerTimer(new sora::SoraDefaultTimer);
	sora->setFPS(60.0);
	
//	sora->createWindowLua(L"resource/scripts/luamaintest.lua");
	sora->createWindow(new mainWindow);
	sora->start();
		
	return 0;
}
