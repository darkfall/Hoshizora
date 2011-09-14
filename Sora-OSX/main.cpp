#include <iostream>

#include "SoraCore.h"

#include "SoraWindowInfo.h"
#include "SoraFont.h"

#include "SoraFMODSoundSystem/SoraFMODSoundSystem.h"
#include "SoraOGLRenderer/SoraOGLRenderer.h"
#include "SoraOGLRenderer/SoraOGLInput.h"
#include "SoraFreetypeFont/SoraFTFontManager.h"
#include "SoraZipResourceManager/SoraZipResourceManager.h"

#include "SoraEventFactory.h"

#include "../UnitTest/TestCases/Test_EventFactory.h"
#include "../UnitTest/TestCases/Test_EventWorld.h"
#include "../UnitTest/UnitTestSuite.h"

#include "mainWindow.h"

#include "SoraTimestamp.h"

void queryFunc(float w, float h) {
    printf("Video mode: %f, %f\n", w, h);
}

int main(int argc, char* argv[]) {    
    
    std::cout<<sora::GetUniqueStringId("SoraHotkeyEvent")<<std::endl<<sora::GetUniqueStringId("SoraMouseEvent")<<std::endl<<sora::GetUniqueStringId("SoraJoystickEvent");

	sora::SoraCore* sora = sora::SoraCore::Instance();
    
	sora->registerRenderSystem(new sora::SoraOGLRenderer);
	sora->registerResourceManager(new sora::SoraZipResourceManager);
	sora->registerFontManager(new sora::SoraFTFontManager);
	sora->registerInput(new sora::SoraOGLInput);
  //  sora->registerSoundSystem(new sora::SoraFMODSoundSystem);
    
    float w, h;
    sora->getDesktopResolution(&w, &h);
    printf("%f, %f\n", w, h);
    
    sora->setQueryVideoModeCallback(queryFunc);
    

	sora->createWindow(new mainWindow);
  //  sora::SORA->enableFullscreenBuffer(true);

	sora->start();
		
	return 0;
}
