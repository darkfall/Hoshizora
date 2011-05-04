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

#include "SoraParticleEditor/peMainWindow.h"

#include "SoraGUIChan/XmlGui.h"
#include "SoraGUIChan/Jsongui.h"

#include "Debug/SoraAutoProfile.h"

#include "../AnimationEditor/aeMainWindow.h"

//#include "GLFW3/SoraOGLInputGLFW3.h"
//#include "GLFW3/SoraOGLRenderer.h"
#include "SoraFMODSoundSystem/SoraFMODSoundSystem.h"
#include "SoraOGLRenderer/SoraOGLRenderer.h"
#include "SoraOGLRenderer/SoraOGLInput.h"

#include "../MEAD/meadBulletManager.h"

int main(int argc, char* argv[]) {
//	sora::tcmalloc_test::test();
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
    
	/*
	sora::SoraLocalizer* localizer = sora::SoraLocalizer::Instance();
	localizer->addLocaleConf(L"en.locale");
	localizer->addLocaleConf(L"cn.locale");
	*/
	sora->setFPS(60.0);
	sora->createWindow(new mainWindow);
	sora->start();
		
	return 0;
}
