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

#include "SoraLua/SoraLuaMainWindow.h"

#include "SoraGestureRecognizer/SoraGestureRecognizer.h"

#include "SoraPath.h"


int main(int argc, char* argv[]) {
    
    sora::SoraPath path = "./test.cpp";
    printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", 
           path.current().c_str(),
           path.home().c_str(),
           path.null().c_str(),
           path.temp().c_str(),
           path.toString().c_str(),
           path.toString(sora::SoraPath::PATH_WINDOWS).c_str(),
           path.absolute().toString().c_str(),
           path.getExtension().c_str(),
           path.getFileName().c_str(),
           path.getBaseName().c_str());
	
	sora::SoraCore* sora = sora::SoraCore::Instance();
	sora->registerRenderSystem(new sora::SoraOGLRenderer);
	sora->registerResourceManager(new sora::SoraZipResourceManager);
	sora->registerFontManager(new sora::SoraFTFontManager);
	sora->registerInput(new sora::SoraOGLInput);
  //  sora->registerSoundSystem(new sora::SoraFMODSoundSystem);
	
	
    sora::SoraGestureRecognizer::Instance();
	sora->createWindow(new mainWindow);
	sora->start();
		
	return 0;
}
