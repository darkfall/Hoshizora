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


#include "TemplateTransformer.h"

//#include "Experimental/tcmalloc_test.h"

#include "Experimental/state_test.h"
#include "SoraMemoryFile.h"

#include "SoraParticleEditor/peMainWindow.h"

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
	
	{
		sora::SoraMemoryFile file1(L"box.png");
		sora::SoraMemoryFile file2(L"box.png");
		sora::SoraMemoryFile file3(L"boxhl.png");
		sora::SoraMemoryFile file4(L"box2.png");
		sora::SoraMemoryFile file5(L"boxhl.png");
		sora::SoraMemoryFile file6(L"boxhl.png");
	}
	
	
	sora::SoraLocalizer* localizer = sora::SoraLocalizer::Instance();
	localizer->addLocaleConf(L"en.locale");
	localizer->addLocaleConf(L"cn.locale");
	
	sora->setFPS(60.0);
	
	sora->createWindow(new peMainWindow);
	sora->start();
		
	return 0;
}
