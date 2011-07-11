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

#include "SoraStringTokenlizer.h"

#include "SoraDirectoryIterator.h"

int main(int argc, char* argv[]) {
    
    sora::SoraDirectoryIterator itDir("./");
    while(!itDir.isEnd()) {
        printf("        %s\n", itDir.name().c_str());
        ++itDir;
    }
    
    
    sora::SoraStringTokenlizer tokens;
    tokens.parse(std::string("test, dsdsd, adsd;, ad-, asdsd2, adsd,   "), ",");;
    sora::SoraStringTokenlizer::iterator start = tokens.begin();
    while(start != tokens.end()) {
        printf("%s\n", (*start).c_str());
        ++start;
    }
    
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
